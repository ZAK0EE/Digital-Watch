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
#include "MCAL/GPIO/GPIO.h"
#include "MCAL/UART/USART_Cfg.h"
#include "HAL/HUART/HUART.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/********************Macros for the HUART Ports********************/
#define HUART_PORTA  	GPIO_GPIOA
#define HUART_PORTB	    GPIO_GPIOB
#define HUART_PORTC  	GPIO_GPIOC
#define HUART_PORTD 	GPIO_GPIOD
#define HUART_PORTE 	GPIO_GPIOE
#define HUART_PORTH	    GPIO_GPIOH

/********************Macros for the HUART Pins********************/
#define HUART_PIN0  	GPIO_PIN0
#define HUART_PIN1 	    GPIO_PIN1
#define HUART_PIN2 	    GPIO_PIN2
#define HUART_PIN3 	    GPIO_PIN3
#define HUART_PIN4 	    GPIO_PIN4
#define HUART_PIN5 	    GPIO_PIN5
#define HUART_PIN6 	    GPIO_PIN6
#define HUART_PIN7 	    GPIO_PIN7
#define HUART_PIN8 	    GPIO_PIN8
#define HUART_PIN9	    GPIO_PIN9
#define HUART_PIN10 	GPIO_PIN10
#define HUART_PIN11 	GPIO_PIN11
#define HUART_PIN12 	GPIO_PIN12
#define HUART_PIN13 	GPIO_PIN13
#define HUART_PIN14 	GPIO_PIN14
#define HUART_PIN15 	GPIO_PIN15
	
/********************Macros for the HUART AF Num********************/
#define HUART_AF_0     GPIO_AF_NUM_0
#define HUART_AF_1     GPIO_AF_NUM_1
#define HUART_AF_2     GPIO_AF_NUM_2
#define HUART_AF_3     GPIO_AF_NUM_3
#define HUART_AF_4     GPIO_AF_NUM_4
#define HUART_AF_5     GPIO_AF_NUM_5
#define HUART_AF_6     GPIO_AF_NUM_6
#define HUART_AF_7     GPIO_AF_NUM_7
#define HUART_AF_8     GPIO_AF_NUM_8
#define HUART_AF_9     GPIO_AF_NUM_9
#define HUART_AF_10    GPIO_AF_NUM_10
#define HUART_AF_11    GPIO_AF_NUM_11
#define HUART_AF_12    GPIO_AF_NUM_12
#define HUART_AF_13    GPIO_AF_NUM_13
#define HUART_AF_14    GPIO_AF_NUM_14
#define HUART_AF_15    GPIO_AF_NUM_15

/*******************************************************************************
 *                             Implementation   				                *
 *******************************************************************************/
/*Global array to set USARTs configuration*/
const  HUSART_PINConfig_t HUARTS[_USART_Num] =
{
 [UASART_1]={.USART_ID = HUSART1_ID,.TX_PORT = HUART_PORTA , .TX_PIN = HUART_PIN9 , .TX_AF_ID = HUART_AF_7 ,
            .RX_PORT = HUART_PORTA , .RX_PIN = HUART_PIN10 , .RX_AF_ID = HUART_AF_7  }


};
