/*
 ============================================================================
 Name        : HUART_Cfg.c
 Author      : Omar Medhat Mohamed
 Description : Source Configuration file for the HUART driver
 Date        : 12/4/2024
 ============================================================================
 */
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include "../../MCAL/GPIO/GPIO.h"
#include "../../MCAL/USART/USART_cfg.h"
#include "HUSART.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/********************Macros for the HUART Ports********************/
#define HUART_PORTA  	(void *)(0x40020000)
#define HUART_PORTB	(void *)(0x40020400)
#define HUART_PORTC  	(void *)(0x40020800)
#define HUART_PORTD 	(void *)(0x40020C00)
#define HUART_PORTE 	(void *)(0x40021000)
#define HUART_PORTH	(void *)(0x40021C00)

/********************Macros for the HUART Pins********************/
#define HUART_PIN0 	0x00000000
#define HUART_PIN1 	0x00000001
#define HUART_PIN2 	0x00000002
#define HUART_PIN3 	0x00000003
#define HUART_PIN4 	0x00000004
#define HUART_PIN5 	0x00000005
#define HUART_PIN6 	0x00000006
#define HUART_PIN7 	0x00000007
#define HUART_PIN8 	0x00000008
#define HUART_PIN9	0x00000009
#define HUART_PIN10 	0x0000000A
#define HUART_PIN11 	0x0000000B
#define HUART_PIN12 	0x0000000C
#define HUART_PIN13 	0x0000000D
#define HUART_PIN14 	0x0000000E
#define HUART_PIN15 	0x0000000F
/********************Macros for the HUART AF Num********************/
#define HUART_AF_0						0x00000000
#define HUART_AF_1						0x00000001
#define HUART_AF_2						0x00000002
#define HUART_AF_3						0x00000003
#define HUART_AF_4						0x00000004
#define HUART_AF_5						0x00000005
#define HUART_AF_6						0x00000006
#define HUART_AF_7						0x00000007
#define HUART_AF_8						0x00000008
#define HUART_AF_9						0x00000009
#define HUART_AF_10						0x0000000A
#define HUART_AF_11						0x0000000B
#define HUART_AF_12						0x0000000C
#define HUART_AF_13						0x0000000D
#define HUART_AF_14						0x0000000E
#define HUART_AF_15						0x0000000F
/*******************************************************************************
 *                             Implementation   				                *
 *******************************************************************************/
/*Global array to set USARTs configuration*/
const  HUSART_PINConfig_t HUARTS[_USART_Num] =
{
 [UASART_1]={.USART_ID = HUSART1_ID,.TX_PORT = HUART_PORTA , .TX_PIN = HUART_PIN9 , .TX_AF_ID = HUART_AF_7 ,
            .RX_PORT = HUART_PORTA , .RX_PIN = HUART_PIN10 , .RX_AF_ID = HUART_AF_7  }


};