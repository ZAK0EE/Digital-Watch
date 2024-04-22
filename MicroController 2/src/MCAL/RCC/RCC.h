#ifndef RCC_H_
#define RCC_H_

#include "../../lib/StdTypes.h"
typedef enum
{

    ERROR_Nok,
    ERROR_Ok,
    ERROR_Invalid_Argument,
    Error_Out_Of_Range,
    ERROR_Invalid_IRQ,
    ERROR_NULL_Pointer,

}Error_t;

//Error_t Validate_NULL_Pointer(void *Ptr);

/* Enable SYSTEM CLOCK */
#define RCC_ENABLE_SYSTEM_CLOCK_HSI     0X00000001UL
#define RCC_ENABLE_SYSTEM_CLOCK_HSE     0x00010000UL
#define RCC_ENABLE_SYSTEM_CLOCK_PLL     0x01000000UL

/* SET MAIN SYSTEM CLOCK */
#define RCC_SYSTEM_CLOCK_HSI     0X00000000UL
#define RCC_SYSTEM_CLOCK_HSE     0x00000001UL
#define RCC_SYSTEM_CLOCK_PLL     0x00000002UL

/* Peripherals AHB1 */
#define PERIPHERAL_AHB1_DMA1    0x00200000UL
#define PERIPHERAL_AHB1_DMA2    0x00400000UL
#define PERIPHERAL_AHB1_GPIOH   0x00000080UL
#define PERIPHERAL_AHB1_GPIOE   0x00000010UL
#define PERIPHERAL_AHB1_GPIOD   0x00000008UL
#define PERIPHERAL_AHB1_GPIOC   0x00000004UL
#define PERIPHERAL_AHB1_GPIOB   0x00000002UL
#define PERIPHERAL_AHB1_GPIOA   0x00000001UL

/* Peripherals APB1 */
#define PERIPHERAL_APB1_I2C3    0x00800000UL
#define PERIPHERAL_APB1_I2C2    0x00400000UL
#define PERIPHERAL_APB1_I2C1    0x00200000UL
#define PERIPHERAL_APB1_UART2   0x00020000UL
#define PERIPHERAL_APB1_SPI3    0x00008000UL
#define PERIPHERAL_APB1_SPI2    0x00004000UL
#define PERIPHERAL_APB1_TIM5    0x00000008UL
#define PERIPHERAL_APB1_TIM4    0x00000004UL
#define PERIPHERAL_APB1_TIM3    0x00000002UL
#define PERIPHERAL_APB1_TIM2    0x00000001UL

/* Peripherals APB2 */
#define PERIPHERAL_APB2_TIM11   0x00040000UL
#define PERIPHERAL_APB2_TIM10   0x00020000UL
#define PERIPHERAL_APB2_TIM9    0x00010000UL
#define PERIPHERAL_APB2_SPI4    0x00002000UL
#define PERIPHERAL_APB2_SPI1    0x00001000UL
#define PERIPHERAL_APB2_ADC1    0x00000100UL
#define PERIPHERAL_APB2_UART6   0x00000020UL
#define PERIPHERAL_APB2_UART1   0x00000010UL
#define PERIPHERAL_APB2_TIM1    0x00000001UL

/**
 * @brief Enable System Clock.
 * @param RCC_ENABLE_SYSTEM_CLOCK: System Clock to be HSI, HSE, or PLL.
 * @return Error State.
 * @note Function just enables the clock, it does not check any flags or logic.
 */
Error_t RCC_Enable_Clock(u32 RCC_ENABLE_SYSTEM_CLOCK);

/**
 * @brief Configure PLL.
 * @param PLL_P_Factor: PLL P Factor.
 * @param PLL_N_Factor: PLL N Factor.
 * @param PLL_M_Factor: PLL M Factor.
 * @return Error State.
 */
Error_t RCC_Confg_PLL(u32 PLL_P_Factor, u32 PLL_N_Factor, u32 PLL_M_Factor);

/**
 * @brief Set Main System Clock.
 * @param RCC_SYSTEM_CLOCK: System Clock to be HSI, HSE, or PLL.
 * @return Error State.
 */
Error_t RCC_Set_Main_System_Clock(u32 RCC_SYSTEM_CLOCK);

/**
 * @brief Set PLL Input Clock.
 * @param RCC_SYSTEM_CLOCK: System Clock to be HSI, HSE, or PLL.
 * @return Error State.
 */
Error_t RCC_Set_PLL_Input_Clock(u32 RCC_SYSTEM_CLOCK);

/**
 * @brief Set APB2 Prescaler.
 * @param PreScaler_Value: Prescaler Value.
 * @return Error State.
 */
Error_t RCC_Set_APB2_PreScaler(u32 PreScaler_Value);

/**
 * @brief Set AHB Prescaler.
 * @param PreScaler_Value: Prescaler Value.
 * @return Error State.
 */
Error_t RCC_Set_AHB_PreScaler(u32 PreScaler_Value);

/**
 * @brief Enable AHB1 Peripheral Clock.
 * @param Peripheral: Peripheral to enable.
 * @return Error State.
 */
Error_t RCC_Enable_AHB1_Peripheral_Clock(u32 Peripheral);

/**
 * @brief Disable AHB1 Peripheral Clock.
 * @param Peripheral: Peripheral to disable.
 * @return Error State.
 */
Error_t RCC_Disable_AHB1_Peripheral_Clock(u32 Peripheral);

/**
 * @brief Enable APB1 Peripheral Clock.
 * @param Peripheral: Peripheral to enable.
 * @return Error State.
 */
Error_t RCC_Enable_APB1_Peripheral_Clock(u32 Peripheral);

/**
 * @brief Disable APB1 Peripheral Clock.
 * @param Peripheral: Peripheral to disable.
 * @return Error State.
 */
Error_t RCC_Disable_APB1_Peripheral_Clock(u32 Peripheral);

/**
 * @brief Enable APB2 Peripheral Clock.
 * @param Peripheral: Peripheral to enable.
 * @return Error State.
 */
Error_t RCC_Enable_APB2_Peripheral_Clock(u32 Peripheral);

/**
 * @brief Disable APB2 Peripheral Clock.
 * @param Peripheral: Peripheral to disable.
 * @return Error State.
 */
Error_t RCC_Disable_APB2_Peripheral_Clock(u32 Peripheral);

#endif