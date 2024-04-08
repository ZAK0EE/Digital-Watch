/*
 ============================================================================
 Name        : RCC.h
 Author      : Omar Medhat Mohamed
 Description : Header File for the RCC Driver
 Date        : 10/2/2024
 ============================================================================
 */
#ifndef RCC_H_
#define RCC_H_
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include "LIB/std_types.h"
#include "LIB/Mask32.h"
#include "LIB/Error.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define CLOCK_HSI BIT0_MASK
#define CLOCK_HSE BIT16_MASK
#define CLOCK_PLL BIT24_MASK

#define READY_HSI BIT1_MASK
#define READY_HSE BIT17_MASK
#define READY_PLL BIT25_MASK

#define SysClk_HSI_MASK 0x00000000
#define SysClk_HSE_MASK BIT0_MASK
#define SysClk_PLL_MASK BIT1_MASK

#define PLL_SRC_HSI 0X00000000
#define PLL_SRC_HSE BIT22_MASK

/************AHB1_BUS_Peripheral_Masks ************/
#define GPIOA 	0x00000001
#define GPIOB 	0x00000002
#define GPIOC 	0x00000004
#define GPIOD 	0x00000008
#define GPIOE 	0x00000010
#define GPIOH 	0x00000080
#define CRC 	0x00001000
#define DMA1 	0x00200000
#define DMA2 	0x00400000
/************AHB2_BUS_Peripheral_Masks ************/
#define OTGFS 	0x20000080
/************APB1_BUS_Peripheral_Masks ************/
#define TIM2 	0x40000001
#define TIM3 	0x40000002
#define TIM4 	0x40000004
#define TIM5 	0x40000008
#define WWDG 	0x40000800
#define SPI2 	0x40004000
#define SPI3 	0x40008000
#define USART2 	0x40020000
#define I2C1 	0x40200000
#define I2C2 	0x40400000
#define I2C3 	0x40800000
#define PWR 	0x50000000
/************APB2_BUS_Peripheral_Masks ************/
#define TIM1 	0x60000001
#define USART1 	0x60000010
#define USART6 	0x60000020
#define ADC1 	0600000100
#define SDIO 	0x60000800
#define SPI1 	0x60001000
#define SPI4 	0x60002000
#define SYSCFG 	0x60004000
#define TIM9 	0x60010000
#define TIM10 	0x60020000
#define TIM11 	0x60040000
/************APB_High_Speed_BUS_Prescaler_Masks ************/
#define APB_H_1 0x00000000
#define APB_H_2 0x00008000
#define APB_H_4 0x0000A000
#define APB_H_8 0x0000C000
#define APB_H_16 0x0000E000
/************APB_LOW_Speed_BUS_Prescaler_Masks ************/
#define APB_L_1 0x00000000
#define APB_L_2 0x00001000
#define APB_L_4 0x00001400
#define APB_L_8 0x00001800
#define APB_L_16 0x00001C00
/************AHB_BUS_Prescaler_Masks ************/
#define AHB_1 0x00000000
#define AHB_2 0x00000080
#define AHB_4 0x00000090
#define AHB_8 0x000000A0
#define AHB_16 0x000000B0
#define AHB_64 0x000000C0
#define AHB_128 0x000000D0
#define AHB_256 0x000000E0
#define AHB_512 0x000000f0

/*******************************************************************************
 *                  	    Functions Prototypes                               *
 *******************************************************************************/
/*
 * @brief    : Set Clock ON.
 * @param[in]: Copy_Clock The clock source to be set on. It can be CLOCK_HSI, CLOCK_HSE, or CLOCK_PLL.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of setting the clock on.
 * @details  : This function turns on the specified clock source.
			   It enables the clock according to the provided clock source.
 */
Error_enumStatus_t RCC_SET_Clock_ON(uint32_t Copy_Clock);
/*
 * @brief    : Set Clock OFF.
 * @param[in]: Copy_Clock The clock source to be turned off. It can be CLOCK_HSI, CLOCK_HSE, or CLOCK_PLL.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of turning the clock off.
 * @details  : This function turns off the specified clock source.
			   It disables the clock according to the provided clock source.
 */
Error_enumStatus_t RCC_SET_Clock_OFF(uint32_t Copy_Clock);
/*
 * @brief    : Read if the Clock is Ready or not
 * @param[in]:  Copy_Clock The clock source to be checked. It can be CLOCK_HSI, CLOCK_HSE, or CLOCK_PLL.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of selecting the system clock.
 */

Error_enumStatus_t RCC_READ_ClockReadyState(uint32_t Copy_ReadyClock);

/*
 * @brief    : Select System Clock.
 * @param[in]: Copy_SysClk The system clock source to be selected. It can be SysClk_HSI_MASK, SysClk_HSE_MASK, or SysClk_PLL_MASK.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of selecting the system clock.
 * @details  : This function selects the system clock source among the available options: HSI, HSE, or PLL.
 */
Error_enumStatus_t RCC_Select_Sysclk(uint32_t Copy_SysClk);
/*
 * @brief    : Configure PLL Source.
 * @param[in]: Copy_PLLSrc The PLL source to be configured. It can be PLL_SRC_HSI or PLL_SRC_HSE.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of configuring PLL source.
 * @details  : This function configures the PLL source to be either HSI or HSE.
 */
Error_enumStatus_t RCC_Config_PLLSrc(uint32_t Copy_PLLSrc);
/*
 * @brief    : Configure PLL Parameters.
 * @param[in]: PLLM, PLLN, PLLP, PLLQ The parameters to configure PLL.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of configuring PLL parameters.
 * @details  : This function configures the parameters (M, N, P, Q) for the PLL.
 * @note     : Ranges of Paramters 2<=PLLM<=63 ,192<=PLLN<=432 , 2<=PLLQ<=15 , PLLP = 2,4,6,8
 */
Error_enumStatus_t RCC_Config_PLLParamters(uint32_t PLLM, uint32_t PLLN, uint32_t PLLP, uint32_t PLLQ);

/*
 * @brief    : Configure APB high-speed prescaler BUS Prescaler APB2.
 * @param[in]: CopyPreScalerValue  Prescaler from Prescaler  ptions in previous definition.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of setting prescaler value.
 */
Error_enumStatus_t RCC_Config_APBH_BusPrescaler(uint32_t CopyPreScalerValue);
/*
 * @brief    : Configure APB Low speed prescaler BUS Prescaler APB1.
 * @param[in]: CopyPreScalerValue  Prescaler from Prescaler  ptions in previous definitio.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of setting prescaler value.
 */
Error_enumStatus_t RCC_Config_APBL_BusPrescaler(uint32_t CopyPreScalerValue);
/*
 * @brief    : Configure AHB prescalerr BUS Prescaler AHB.
 * @param[in]: CopyPreScalerValue  Prescaler from Prescaler  ptions in previous definitio.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of setting prescaler value.
 */
Error_enumStatus_t RCC_Config_AHB_BusPrescaler(uint32_t CopyPreScalerValue);

/*
 * @brief    : Enable AHB1 Peripheral.
 * @param[in]: Copy_AHB1PeripheralName The AHB1 peripheral to be enabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of enabling the AHB1 peripheral.
 * @details  : This function enables the specified peripheral connected to the AHB1 bus.
 */
Error_enumStatus_t RCC_Enable_AHB1_Peripheral(uint32_t Copy_AHB1PeripheralName);
/*
 * @brief    : Enable AHB2 Peripheral.
 * @param[in]: Copy_AHB2PeripheralName The AHB2 peripheral to be enabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of enabling the AHB2 peripheral.
 * @details  : This function enables the specified peripheral connected to the AHB2 bus.
 */
Error_enumStatus_t RCC_Enable_AHB2_Peripheral(uint32_t Copy_AHB2PeripheralName);
/*
 * @brief    Enable APB1 Peripheral.
 * @param[in]: Copy_APB1PeripheralName The APB1 peripheral to be enabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of enabling the APB1 peripheral.
 * @details  : This function enables the specified peripheral connected to the APB1 bus.
 */
Error_enumStatus_t RCC_Enable_APB1_Peripheral(uint32_t Copy_APB1PeripheralName);
/*
 * @brief    Enable APB2 Peripheral.
 * @param[in]: Copy_APB2PeripheralName The APB2 peripheral to be enabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of enabling the APB2 peripheral.
 * @details  : This function enables the specified peripheral connected to the APB2 bus.
 */
Error_enumStatus_t RCC_Enable_APB2_Peripheral(uint32_t Copy_APB2PeripheralName);
/*
 * @brief    Disable AHB1 Peripheral.
 * @param[in]: Copy_AHB1PeripheralName The AHB1 peripheral to be disabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of disabling the AHB1 peripheral.
 * @details  : This function disables the specified peripheral connected to the AHB1 bus.
 */
Error_enumStatus_t RCC_Disable_AHB1_Peripheral(uint32_t Copy_AHB1PeripheralName);
/*
 * @brief    Disable AHB2 Peripheral.
 * @param[in]: Copy_AHB2PeripheralName The AHB2 peripheral to be disabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of disabling the AHB2 peripheral.
 * @details  : This function disables the specified peripheral connected to the AHB2 bus.
 */
Error_enumStatus_t RCC_Disable_AHB2_Peripheral(uint32_t Copy_AHB2PeripheralName);
/*
 * @brief    Disable APB1 Peripheral.
 * @param[in]: Copy_APB1PeripheralName The APB1 peripheral to be disabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of disabling the APB1 peripheral.
 * @details  : This function disables the specified peripheral connected to the APB1 bus.
 */
Error_enumStatus_t RCC_Disable_APB1_Peripheral(uint32_t Copy_APB1PeripheralName);
/*
 * @brief    Disable APB2 Peripheral.
 * @param[in]: Copy_APB2PeripheralName The APB2 peripheral to be disabled.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of disabling the APB2 peripheral.
 * @details  : This function disables the specified peripheral connected to the APB2 bus.
 */
Error_enumStatus_t RCC_Disable_APB2_Peripheral(uint32_t Copy_APB2PeripheralName);

#endif /* RCC_H_ */
