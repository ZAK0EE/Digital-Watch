/*
 ============================================================================
 Name        : HSwitch_CGF.c
 Author      : Omar Medhat Mohamed
 Description : Source Configuration file for the HSwitch driver (Solve Debounce problem)
 Date        : 15/3/2024
 ============================================================================
 */
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include "MCAL/GPIO/GPIO.h"
#include "HAL/HSwitch/HSwitch_Cfg.h"
#include "HAL/HSwitch/HSwitch.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/********************Macros for the HSwitch Ports********************/
#define HSwitch_PORTA  	GPIO_GPIOA
#define HSwitch_PORTB	GPIO_GPIOB
#define HSwitch_PORTC  	GPIO_GPIOC
#define HSwitch_PORTD 	GPIO_GPIOD
#define HSwitch_PORTE 	GPIO_GPIOE
#define HSwitch_PORTH	GPIO_GPIOH

/********************Macros for the HSwitch Pins********************/
#define HSwitch_PIN0 	GPIO_PIN0
#define HSwitch_PIN1 	GPIO_PIN1
#define HSwitch_PIN2 	GPIO_PIN2
#define HSwitch_PIN3 	GPIO_PIN3
#define HSwitch_PIN4 	GPIO_PIN4
#define HSwitch_PIN5 	GPIO_PIN5
#define HSwitch_PIN6 	GPIO_PIN6
#define HSwitch_PIN7 	GPIO_PIN7
#define HSwitch_PIN8 	GPIO_PIN8
#define HSwitch_PIN9	GPIO_PIN9
#define HSwitch_PIN10 	GPIO_PIN10
#define HSwitch_PIN11 	GPIO_PIN11
#define HSwitch_PIN12 	GPIO_PIN12
#define HSwitch_PIN13 	GPIO_PIN13
#define HSwitch_PIN14 	GPIO_PIN14
#define HSwitch_PIN15 	GPIO_PIN15

/********************Macros for the HSwitch type of connection********************/
#define HSwitch_INPUT_FL					 GPIO_MODE_INPUT_NOPULL
#define HSwitch_INPUT_PL					 GPIO_MODE_INPUT_PULLUP
#define HSwitch_INPUT_PD					 GPIO_MODE_INPUT_PULLDOWN
/*******************************************************************************
 *                             Implementation   				                *
 *******************************************************************************/
/*Global array to set HSwitchs configuration
 I make an extra extern here beacuse of linker issue when declare and constant global array 
 It implicitly make it static fri this file so the other files can't see it */
 const  HSwitch_Config_t HSwitchS[_HSwitch_Num] = {
   /*Ex : Set HSwitch1 Configuration*/
    [Increment] = {
        .Pin = HSwitch_PIN0,
        .Port = HSwitch_PORTB,
        .Connection = HSwitch_INPUT_PL
    },
   [Mode] = {
        .Pin = HSwitch_PIN1,
        .Port = HSwitch_PORTB,
        .Connection = HSwitch_INPUT_PL
    },
   [Edit] = {
        .Pin = HSwitch_PIN6,
        .Port = HSwitch_PORTB,
        .Connection = HSwitch_INPUT_PL
    }


};
