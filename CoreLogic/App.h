#ifndef APP_H
#define APP_H


#define LOCK_BTN_PORT       GPIO_B
#define LOCK_BTN_PIN        4

#define EMERGENCY_BTN_PORT  GPIO_B
#define EMERGENCY_BTN_PIN   7

#define DOORBELL_BTN_PORT   GPIO_B
#define DOORBELL_BTN_PIN    10

#define SYSTICK_CTRL  (*((volatile uint32_t *)0xE000E010))
#define SYSTICK_LOAD  (*((volatile uint32_t *)0xE000E014))
#define SYSTICK_VAL   (*((volatile uint32_t *)0xE000E018))

/* ─── Configuration ───────────────────────────────────────────── */
#define PASSWORD_LENGTH         4
#define LOCKOUT_THRESHOLD       10
#define KEY_IDLE 0xFF
/* ─── States ──────────────────────────────────────────────────── */
typedef enum {
    STATE_LOCKED   = 0,
    STATE_UNLOCKED = 1,
    STATE_ALARM    = 2
} SystemState_t;

/* ─── Events ──────────────────────────────────────────────────── */
typedef enum {
    EVENT_NONE         = 0,
    EVENT_DIGIT_INPUT  = 1,
    EVENT_LOCK_COMMAND = 2,
    EVENT_DOORBELL     = 3,
    EVENT_EMERGENCY    = 4
} SystemEvent_t;

/* ─── Public API ──────────────────────────────────────────────── */
void App_Init(void);
void App_Update(void);
void App_EmergencyReset_ISR(void);
void App_DoorBell_ISR(void);

#endif /* APP_H */