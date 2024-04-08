/*
 ============================================================================
 Name        : CLK_CONTROL.h
 Author      : Omar Medhat Mohamed
 Description : Header File for the CLK_CONTROL Module
 Date        : 5/3/2024
 ============================================================================
 */
#ifndef CLK_CONTROL_H_
#define CLK_CONTROL_H_
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include  	"LIB/std_types.h"
#include 	"LIB/Mask32.h"
#include 	"LIB/Error.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/************AHB1_BUS_Peripheral_Masks ************/
#define GPIOA 0x00000001
#define GPIOB 0x00000002
#define GPIOC 0x00000004
#define GPIOD 0x00000008
#define GPIOE 0x00000010
#define GPIOH 0x00000080
#define CRC 0x00001000
#define DMA1 0x00200000
#define DMA2 0x00400000
/************AHB2_BUS_Peripheral_Masks ************/
#define OTGFS 0x20000080
/************APB1_BUS_Peripheral_Masks ************/
#define TIM2 0x40000001
#define TIM3 0x40000002
#define TIM4 0x40000004
#define TIM5 0x40000008
#define WWDG 0x40000800
#define SPI2 0x40004000
#define SPI3 0x40008000
#define USART2 0x40020000
#define I2C1 0x40200000
#define I2C2 0x40400000
#define I2C3 0x40800000
#define PWR 0x50000000
/************APB2_BUS_Peripheral_Masks ************/
#define TIM1 0x60000001
#define USART1 0x60000010
#define USART6 0x60000020
#define ADC1 0600000100
#define SDIO 0x60000800
#define SPI1 0x60001000
#define SPI4 0x60002000
#define SYSCFG 0x60004000
#define TIM9 0x60010000
#define TIM10 0x60020000
#define TIM11 0x60040000
/*******************************************************************************
 *                  	    Functions Prototypes                               *
 *******************************************************************************/
/*
 * @brief    : Set Clock ON For Certain GPIO Port.
 * @param[in]: Copy_PortName Port Name.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of setting the clock on.
 * @details  : This function turns on the specified clock source for specific GPIO portt. 
 */
Error_enumStatus_t Set_Clock_ON (uint32_t Copy_PortName);


#endif
