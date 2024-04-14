/**
 * @file LCD_Cfg.c
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "LCD.h"
#include "LCD_Cfg.h"
#include "MCAL/GPIO/GPIO.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/




LCD_Config_t LCD_Config[_NUM_OF_LCDS]=
{
	[LCD1]=
	{
		.DataLength = LCD_DL_8BIT,
		.Font= LCD_FONT_5X10,
		.CursorState = LCD_CURSOR_STATE_OFF,
		.CursorBlinkingState = LCD_CURSOR_BLINKING_OFF,
 		.RSPin=
		{
			.PortID= GPIO_GPIOA,
			.PinNum= GPIO_PIN0,
		},
		.EnablePin = 
		{
			.PortID= GPIO_GPIOA,
			.PinNum= GPIO_PIN1,	
		},
		.Pins =
		{
			{
				.PortID= GPIO_GPIOA,
				.PinNum= GPIO_PIN2,
			},
			{
				.PortID= GPIO_GPIOA,
				.PinNum= GPIO_PIN3,	
			},
			{
				.PortID= GPIO_GPIOA,
				.PinNum= GPIO_PIN4,
			},
			{
				.PortID= GPIO_GPIOA,
				.PinNum= GPIO_PIN5,
			},
			{
				.PortID= GPIO_GPIOA,
				.PinNum= GPIO_PIN6,
			},
			{
				.PortID= GPIO_GPIOA,
				.PinNum= GPIO_PIN7,
			},
			{
				.PortID= GPIO_GPIOA,
				.PinNum= GPIO_PIN8,
			},
			{
				.PortID= GPIO_GPIOA,
				.PinNum= GPIO_PIN9,
			},
																			
		}

	},



};
