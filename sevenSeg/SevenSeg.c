#include "SevenSeg.h"
#include "Utils.h"

static const uint8 sevenSegMap[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

static uint8 seg_pins[7][2] = {
    {SEGMENT_A}, {SEGMENT_B}, {SEGMENT_C}, {SEGMENT_D},
    {SEGMENT_E}, {SEGMENT_F}, {SEGMENT_G}
};

static uint8 currentTens  = 0;
static uint8 currentUnits = 0;
static uint8 currentDigit = 0;   /* 0 = tens, 1 = units */

static void writeSegments(uint8 digit) {
    for (uint8 i = 0; i < 7; i++) {
        uint8 val = (sevenSegMap[digit] >> i) & 0x01;
        Gpio_WritePin(seg_pins[i][0], seg_pins[i][1], val);
    }
}

void sevenSegInit(void) {
    /* Segment pins as output */
    for (uint8 i = 0; i < 7; i++) {
        Gpio_Init(seg_pins[i][0], seg_pins[i][1], GPIO_OUTPUT, GPIO_PUSH_PULL);
    }
    /* Common pins as output — start both OFF (HIGH = digit disabled) */
    Gpio_Init(GPIO_B, 15, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(GPIO_B, 14, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_WritePin(GPIO_B, 15, HIGH);
    Gpio_WritePin(GPIO_B, 14, HIGH);
}

void sevenSegDisplay(uint8 number) {
    if (number > 99) return;
    currentTens  = number / 10;
    currentUnits = number % 10;
}

/* Call this repeatedly in the main loop — alternates between digits */
void sevenSegRefresh(void) {
    if (currentDigit == 0) {
        /* Show tens digit */
        Gpio_WritePin(GPIO_B, 14, HIGH);   /* disable units */
        writeSegments(currentTens);
        Gpio_WritePin(GPIO_B, 15, LOW);    /* enable tens */
        currentDigit = 1;
        delay_ms(5);                       /* Give Proteus time to render the LED */
    } else {
        /* Show units digit */
        Gpio_WritePin(GPIO_B, 15, HIGH);   /* disable tens */
        writeSegments(currentUnits);
        Gpio_WritePin(GPIO_B, 14, LOW);    /* enable units */
        currentDigit = 0;
        delay_ms(5);                       /* Give Proteus time to render the LED */
    }
}