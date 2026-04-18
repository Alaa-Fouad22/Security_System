/**
* Keypad.h
 *
 *  Created on: 4/22/2025
 *  Author    : AbdallahDarwish
 */

#ifndef KEYPAD_H
#define KEYPAD_H

#include "STD_TYPES.h"
#include "Gpio.h"

/*Keypad_Init*/
#define Keypad_R0 GPIO_D, 0
#define Keypad_R1 GPIO_D, 1
#define Keypad_R2 GPIO_D, 2
#define Keypad_R3 GPIO_D, 3

#define Keypad_C0 GPIO_D, 6
#define Keypad_C1 GPIO_D, 7
#define Keypad_C2 GPIO_D, 8
#define Keypad_C3 GPIO_D, 9


#define KEYPAD_NUM_ROWS 4
#define KEYPAD_NUM_COLS 4

void Keypad_Init(void);

uint8 Keypad_Scan(void);

#endif //KEYPAD_H
