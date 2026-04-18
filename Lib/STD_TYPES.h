//
// Created by alaaa on 4/15/2026.
//

#ifndef PROJECT2_BIT_MATH_H
#define PROJECT2_BIT_MATH_H

#include <stdint.h>

/* signed integer types */
typedef int8_t   sint8;
typedef int16_t   sint16;
typedef int32_t     sint32;


/* unsigned integer types */
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t   uint32;


/* Boolean Type */
typedef uint8  boolean;
#define TRUE    ((boolean)1)
#define FALSE   ((boolean)0)

/* Standard return type */
typedef enum {
    STD_SUCCESS = 0,
    STD_FAILURE = 1
} STD_ReturnType;


/* Generic Pointer type */
typedef void * VoidPtr ;

/* Generic Const Pointer type for read-only access  */
typedef const void * ConstVoidPtr;




#endif



