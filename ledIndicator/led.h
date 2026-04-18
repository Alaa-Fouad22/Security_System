#ifndef LED_H
#define LED_H

#include "STD_TYPES.h"

#define LED_PROGRESS_COUNT      4

void Led_Init(void);
void Led_SetProgress(uint8 index, uint8 state);
void Led_ClearProgress(void);
void Led_SetSuccess(uint8 state);
void Led_SetAlarm(uint8 state);
void Led_PulseBell(void);

#endif /* LED_H */