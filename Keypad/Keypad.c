/**
* Keypad.c
 *
 *  Created on: 4/22/2025
 *  Author    : AbdallahDarwish
 */

#include "Keypad.h"
#include "Gpio.h"
#include "Utils.h"

uint8 KeypadDataMap[KEYPAD_NUM_ROWS][KEYPAD_NUM_COLS] = {
    {'7', '8', '9', '0'},
    {'4', '5', '6', '0'},
    {'1', '2', '3', '0'},
    {'0', '0', '0', '0'}
};

uint8 KeypadRowsPins[KEYPAD_NUM_ROWS][2] = {
    {Keypad_R0},
    {Keypad_R1},
    {Keypad_R2},
    {Keypad_R3}
};

uint8 KeypadColsPins[KEYPAD_NUM_COLS][2] = {
    {Keypad_C0},
    {Keypad_C1},
    {Keypad_C2},
{Keypad_C3}
};


void Keypad_Init(void) {
    uint8 i = 0;
    // init for rows as output
    for (i = 0; i < KEYPAD_NUM_ROWS; i++) {
        Gpio_Init(KeypadRowsPins[i][0], KeypadRowsPins[i][1], GPIO_OUTPUT, GPIO_PUSH_PULL);
    }

    // init for cols as input
    for (i = 0; i < KEYPAD_NUM_COLS; i++) {
        Gpio_Init(KeypadColsPins[i][0], KeypadColsPins[i][1], GPIO_INPUT, GPIO_PULL_UP);
    }

    // set all rows to high(reset state)
    for (i = 0; i < KEYPAD_NUM_ROWS; i++) {
        Gpio_WritePin(KeypadRowsPins[i][0], KeypadRowsPins[i][1], HIGH);
    }
}


uint8 Keypad_Scan(void) {
    uint8 i = 0, j = 0, key = 0xff;

    for (i = 0; i < KEYPAD_NUM_ROWS; i++) {
        Gpio_WritePin(KeypadRowsPins[i][0], KeypadRowsPins[i][1], LOW);

        for (j = 0; j < KEYPAD_NUM_COLS; j++) {
            if (Gpio_ReadPin(KeypadColsPins[j][0], KeypadColsPins[j][1]) == LOW) {
                delay_ms(170);
                if (Gpio_ReadPin(KeypadColsPins[j][0], KeypadColsPins[j][1]) == LOW) {
                    key = KeypadDataMap[i][j];
                }
            }
        }

        Gpio_WritePin(KeypadRowsPins[i][0], KeypadRowsPins[i][1], HIGH);
    }

    return key;
}
