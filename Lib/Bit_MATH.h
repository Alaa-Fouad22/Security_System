//
// Created by alaaa on 4/15/2026.
//

#ifndef BIT_MATH_H
#define BIT_MATH_H


// Single-bit operations
#define SET_BIT(REG, BIT)     ((REG) |= ( 1u << (BIT) ))
#define CLR_BIT(REG, BIT)     ((REG) &= ~( 1u << (BIT) ))
#define TOG_BIT(REG, BIT)     ((REG) ^= ( 1u << (BIT) ))
#define READ_BIT(REG, BIT)    (((REG) & ( 1u << (BIT) ))>>(BIT))

/* Set all bits covered by MASK */
#define SET_BITS(REG, MASK)     ((REG) |= (MASK))

/* Clear all bits covered by MASK */
#define CLR_BITS(REG, MASK)     ((REG) &= ~(MASK))

/* Write VAL into the bit-field starting at POS with WIDTH bits */
#define SET_FIELD(REG, POS, WIDTH, VAL)                          \
((REG) = (((REG) & ~(((1u << (WIDTH)) - 1u) << (POS)))  |   \
(((VAL) &  ((1u << (WIDTH)) - 1u)) << (POS))))

/* Read / extract the bit-field starting at POS with WIDTH bits */
#define GET_FIELD(REG, POS, WIDTH)                               \
(((REG) >> (POS)) & ((1u << (WIDTH)) - 1u))



#endif //PROJECT2_BIT_MATH_H