//
// Created by alaaa on 4/15/2026.
//

#ifndef PROJECT2_UTILS_H
#define PROJECT2_UTILS_H

#include "STD_TYPES.h"

void delay_ms(uint32 ms);
#define REG32(ADDR)  ( *(volatile uint32 *)(ADDR) )
#endif //PROJECT2_UTILS_H