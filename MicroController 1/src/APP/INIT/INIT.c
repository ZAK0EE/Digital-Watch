/*
 ============================================================================
 Name        : INIT.c
 Author      : Omar Medhat Mohamed
 Description : Source File for the INIT Driver
 Date        : 15/4/2024
 ============================================================================
 */
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include "APP/INIT/INIT.h"
/*******************************************************************************
 *                             Implementation   				                *
 *******************************************************************************/
/**
 * @brief    : Initializes the system components.
 * @details  : This function initializes the GPIO clock for GPIOA, initializes the hardware UART (HUART), 
 *             initializes the hardware switch (HSwitch), initializes the LCD (LCD1), and initializes the display.
 * @param    : None
 * @return   : None
 **/
void InitSys (void) 
{
    /* Enable clock for GPIOA */
    Set_Clock_ON(GPIOA);
    Set_Clock_ON(GPIOB);
    Set_Clock_ON(USART1);
    /* Initialize hardware UART */
    HUART_Init();
    
    /* Initialize hardware switch */
    HSwitch_Init();
    
    /* Initialize LCD1 */
    LCD_init(LCD1);
    
    /* Initialize display */
    Display_init();

    /* Initialize Buttons */
    Button_init();

}