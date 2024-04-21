/*
 * StdTypes.h
 *
 * Created: 2/13/2023 2:10:05 PM
 *  Author: abdelrahman
 */

/*
 * @file: stdtypes.h
 * @Description: This file defines standard data types and constants for use in embedded systems.
 */

#ifndef STDTYPES_H_
#define STDTYPES_H_

/* Define standard types for 8-bit unsigned and signed integers */
typedef unsigned char u8;
typedef signed char s8;

/* Define standard types for 16-bit unsigned and signed integers */
typedef unsigned short int u16;
typedef signed short int s16;

/* Define standard types for 32-bit unsigned and signed integers */
typedef unsigned int u32;
typedef signed int s32;

/* Define standard types for 64-bit unsigned and signed integers */
typedef unsigned long long int u64;
typedef signed long long int s64;



/* Define a boolean type using an enumeration with TRUE and FALSE values */
typedef enum {
	TRUE = 0x55,
	FALSE = 0xAA
} bool_t;

/* Define a NULL pointer constant */
#define NULLPTR ((void *)0)

/* Define constants for maximum and minimum values of 8-bit unsigned and signed integers */
#define MAX_U8 ((u8)255)
#define MIN_U8 ((u8)0)

#define MAX_S8 ((s8)127)
#define MIN_S8 ((s8)-128)
#define ZERO_S8 ((s8)0)

#endif /* STDTYPES_H_ */
