//
// Created by alaaa on 4/15/2026.
//

#ifndef PROJECT2_SEVENSEG_H
#define PROJECT2_SEVENSEG_H
#include "../Lib/STD_TYPES.h"
#include "Gpio.h"

#define SEGMENT_A  GPIO_C, 0
#define SEGMENT_B  GPIO_C, 1
#define SEGMENT_C  GPIO_C, 2
#define SEGMENT_D  GPIO_C, 3
#define SEGMENT_E  GPIO_C, 4
#define SEGMENT_F  GPIO_C, 5
#define SEGMENT_G  GPIO_C, 6

/* Common enable pins — active LOW */
#define SEG_COM_TENS   GPIO_B, 15   /* left digit  */
#define SEG_COM_UNITS  GPIO_B, 14   /* right digit */

void sevenSegInit(void);
void sevenSegDisplay(uint8 number);  /* 0–99 */
void sevenSegRefresh(void);           /* call every ~5ms in main loop */


#endif //PROJECT2_SEVENSEG_H