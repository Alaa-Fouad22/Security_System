#include "Led.h"
#include "Gpio.h"

/* ─── Pin Mapping (matches Proteus schematic) ─────────────────
 *
 *  Progress LEDs  : PA0, PA1, PA2, PA3
 *  Success LED    : PE6  (GREEN)
 *  Alarm LED      : PE7  (RED)
 *  Door Bell LED  : PE8  (YELLOW)
 *
 * ─────────────────────────────────────────────────────────── */

/* Progress LEDs on GPIOA */
#define PROGRESS_LED_PORT       GPIO_A
#define PROGRESS_LED_PIN0       0
#define PROGRESS_LED_PIN1       1
#define PROGRESS_LED_PIN2       2
#define PROGRESS_LED_PIN3       3

/* Status LEDs on GPIOE */
#define STATUS_LED_PORT         GPIO_E
#define SUCCESS_LED_PIN         6
#define ALARM_LED_PIN           7
#define BELL_LED_PIN            8

static const uint16 progressPins[LED_PROGRESS_COUNT] = {
    PROGRESS_LED_PIN0,
    PROGRESS_LED_PIN1,
    PROGRESS_LED_PIN2,
    PROGRESS_LED_PIN3
};

void Led_Init(void)
{
    Gpio_Init(PROGRESS_LED_PORT, PROGRESS_LED_PIN0, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(PROGRESS_LED_PORT, PROGRESS_LED_PIN1, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(PROGRESS_LED_PORT, PROGRESS_LED_PIN2, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(PROGRESS_LED_PORT, PROGRESS_LED_PIN3, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(STATUS_LED_PORT,   SUCCESS_LED_PIN,   GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(STATUS_LED_PORT,   ALARM_LED_PIN,     GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(STATUS_LED_PORT,   BELL_LED_PIN,      GPIO_OUTPUT, GPIO_PUSH_PULL);

    Led_ClearProgress();
    Led_SetSuccess(0);
    Led_SetAlarm(0);
    Gpio_WritePin(STATUS_LED_PORT, BELL_LED_PIN, LOW);
}

void Led_SetProgress(uint8 index, uint8 state)
{
    if (index < LED_PROGRESS_COUNT) {
        Gpio_WritePin(PROGRESS_LED_PORT, progressPins[index],
                      state ? HIGH : LOW);
    }
}

void Led_ClearProgress(void)
{
    for (uint8 i = 0; i < LED_PROGRESS_COUNT; i++) {
        Gpio_WritePin(PROGRESS_LED_PORT, progressPins[i], LOW);
    }
}

void Led_SetSuccess(uint8 state)
{
    Gpio_WritePin(STATUS_LED_PORT, SUCCESS_LED_PIN, state ? HIGH : LOW);
}

void Led_SetAlarm(uint8 state)
{
    Gpio_WritePin(STATUS_LED_PORT, ALARM_LED_PIN, state ? HIGH : LOW);
}

void Led_PulseBell(void)
{
    Gpio_WritePin(STATUS_LED_PORT, BELL_LED_PIN, HIGH);
    for (volatile uint32 d = 0; d < 200000UL; d++){};
    Gpio_WritePin(STATUS_LED_PORT, BELL_LED_PIN, LOW);
}