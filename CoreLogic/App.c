#include "App.h"
#include "Keypad.h"
#include "Gpio.h"
#include "SevenSeg.h"
#include "Led.h"
#include "Exti.h"
#include "Utils.h"

/* ─── Secret Password ─────────────────────────────────────────── */
static const uint8 SECRET[PASSWORD_LENGTH] = {'1', '2', '3', '4'};

/* ─── Internal State ──────────────────────────────────────────── */
static SystemState_t currentState;
static uint8         inputBuffer[PASSWORD_LENGTH];
static uint8         inputIndex;
static uint8         failureCount;

/* ─── ISR Flags ───────────────────────────────────────────────── */
static volatile uint8 flag_Emergency;
static volatile uint8 flag_DoorBell;

/* ─── Private Helpers ─────────────────────────────────────────── */
static void resetInput(void) {
    inputIndex = 0;
    for (uint8 i = 0; i < PASSWORD_LENGTH; i++) inputBuffer[i] = 0;
    Led_ClearProgress();
}

static uint8 isSequenceValid(void) {
    for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
        if (inputBuffer[i] != SECRET[i]) return 0;
    }
    return 1;
}

/* ─── ISR Callbacks ───────────────────────────────────────────── */
void App_EmergencyReset_ISR(void) {
    flag_Emergency = 1;
}

void App_DoorBell_ISR(void) {
    flag_DoorBell = 1;
}

/* ─── State: LOCKED ───────────────────────────────────────────── */
static void handleLocked(SystemEvent_t event, uint8 digit) {
    switch (event) {
        case EVENT_DIGIT_INPUT: {
            /* 1. Store the entered digit unconditionally */
            inputBuffer[inputIndex] = digit;

            /* 2. ONLY light up the LED if THIS specific digit is correct */
            if (digit == SECRET[inputIndex]) {
                Led_SetProgress(inputIndex, 1);
            } else {
                Led_SetProgress(inputIndex, 0); /* Keep it off if wrong */
            }

            inputIndex++;

            /* 3. Wait until all digits are entered before deciding Success/Fail */
            if (inputIndex == PASSWORD_LENGTH) {
                if (isSequenceValid()) {
                    // Password is CORRECT
                    delay_ms(500);
                    Led_ClearProgress();
                    Led_SetSuccess(1);
                    failureCount = 0;
                    sevenSegDisplay(failureCount);
                    currentState = STATE_UNLOCKED;
                } else {
                    // Password is WRONG
                    failureCount++;
                    sevenSegDisplay(failureCount);
                    resetInput(); /* Clears the LEDs and resets index to 0 */

                    // Check if they hit the limit
                    if (failureCount >= LOCKOUT_THRESHOLD) {
                        Led_SetAlarm(1);
                        currentState = STATE_ALARM;
                    }
                }
            }
            break;
        }
        case EVENT_DOORBELL: {
            Led_PulseBell();
            break;
        }
        default:
            break;
    }
}

/* ─── State: UNLOCKED ─────────────────────────────────────────── */
static void handleUnlocked(SystemEvent_t event) {
    switch (event) {
        case EVENT_LOCK_COMMAND: {
            Led_SetSuccess(0);
            resetInput();
            currentState = STATE_LOCKED;
            break;
        }
        case EVENT_DOORBELL: {
            Led_PulseBell();
            break;
        }
        default:
            break;
    }
}

/* ─── State: ALARM ────────────────────────────────────────────── */
static void handleAlarm(SystemEvent_t event) {
    switch (event) {
        case EVENT_EMERGENCY: {
            Led_SetAlarm(0);
            resetInput();
            failureCount = 0;
            sevenSegDisplay(failureCount);
            currentState = STATE_LOCKED;
            break;
        }
        default:
            break;
    }
}

/* ─── Public API ──────────────────────────────────────────────── */
void App_Init(void) {
    currentState   = STATE_LOCKED;
    inputIndex     = 0;
    failureCount   = 0;
    flag_Emergency = 0;
    flag_DoorBell  = 0;

    for (uint8 i = 0; i < PASSWORD_LENGTH; i++) inputBuffer[i] = 0;

    // Buttons Init (Pull Up so they are Active LOW)
    Gpio_Init(LOCK_BTN_PORT, LOCK_BTN_PIN, GPIO_INPUT, GPIO_PULL_UP);
    Gpio_Init(EMERGENCY_BTN_PORT, EMERGENCY_BTN_PIN, GPIO_INPUT, GPIO_PULL_UP);
    Gpio_Init(DOORBELL_BTN_PORT, DOORBELL_BTN_PIN, GPIO_INPUT, GPIO_PULL_UP);

    // EXTI Init (Falling Edge due to Pull-Up)
    Exti_Init(EMERGENCY_BTN_PIN, EXTI_PORT_B, EXTI_EDGE_FALLING, App_EmergencyReset_ISR);
    Exti_SetPriority(EMERGENCY_BTN_PIN, 0); // High Priority
    Exti_Enable(EMERGENCY_BTN_PIN);

    Exti_Init(DOORBELL_BTN_PIN, EXTI_PORT_B, EXTI_EDGE_FALLING, App_DoorBell_ISR);
    Exti_SetPriority(DOORBELL_BTN_PIN, 1); // Lower Priority
    Exti_Enable(DOORBELL_BTN_PIN);

    // Hardware Init
    Keypad_Init();
    Led_Init();
    sevenSegInit();
    sevenSegDisplay(0);
}

void App_Update(void) {
    /* 1. ALWAYS refresh the display first thing, no matter what! */
    sevenSegRefresh();

    SystemEvent_t event = EVENT_NONE;
    uint8 digit = 0;

    /* ── Doorbell: turn LED ON while button held, OFF when released ── */
    if (flag_DoorBell) {
        flag_DoorBell = 0;
        Led_SetBell(1);                                   /* LED ON immediately */

        /* Keep refreshing display while stuck waiting for button release! */
        while (Gpio_ReadPin(DOORBELL_BTN_PORT, DOORBELL_BTN_PIN) == LOW) {
            sevenSegRefresh();
        }

        Led_SetBell(0);                                   /* LED OFF on release */

        /* Route event through state machine WITHOUT disturbing other state */
        switch (currentState) {
            case STATE_LOCKED:
            case STATE_UNLOCKED:
            case STATE_ALARM:
                /* doorbell is a side-effect only; state never changes */
                break;
        }
        return;   /* done — don't process any other event this cycle */
    }

    /* ── All other events ──────────────────────────────────────── */
    if (flag_Emergency) {
        flag_Emergency = 0;
        event = EVENT_EMERGENCY;
    }
    else if (Gpio_ReadPin(LOCK_BTN_PORT, LOCK_BTN_PIN) == LOW) {
        /* Keep refreshing display while stuck waiting for button release! */
        while (Gpio_ReadPin(LOCK_BTN_PORT, LOCK_BTN_PIN) == LOW) {
            sevenSegRefresh();
        }
        event = EVENT_LOCK_COMMAND;
    }
    else {
        digit = Keypad_Scan();
        if (digit != KEY_IDLE) {
            event = EVENT_DIGIT_INPUT;
        }
    }

    if (event == EVENT_NONE) return;

    switch (currentState) {
        case STATE_LOCKED:   handleLocked(event, digit); break;
        case STATE_UNLOCKED: handleUnlocked(event);      break;
        case STATE_ALARM:    handleAlarm(event);         break;
    }
}