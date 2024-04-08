/*
 ============================================================================
 Name        : Stm32F401cc.h
 Author      : Omar Medhat Mohamed
 Description : Header File for the NVIC Driver
 Date        : 3/3/2024
 ============================================================================
 */

#ifndef STM32F401CC_H_
#define STM32F401CC_H_

/*******************************************************************************
 *                        	  Types Declaration                                 *
 *******************************************************************************/
typedef enum
{
    WWDG_IRQ=0,
    EXTI16_PVD_IRQ,
    EXTI21_TAMP_STAMP_IRQ,
    EXTI22_RTC_WKUP_IRQ,
    FLASH_IRQ,
    RCC_IRQ,
    EXTI0_IRQ,
    EXTI1_IRQ,
    EXTI2_IRQ,
    EXTI3_IRQ,
    EXTI4_IRQ,
    DMA1_Stream0_IRQ,
    DMA1_Stream1_IRQ,
    DMA1_Stream2_IRQ,
    DMA1_Stream3_IRQ,
    DMA1_Stream4_IRQ,
    DMA1_Stream5_IRQ,
    DMA1_Stream6_IRQ,
    ADC_IRQ,
    EXTI9_5_IRQ = 23,
    TIM1_BRK_TIM9_IRQ,
    TIM1_UP_TIM10_IRQ,
    TIM1_TRG_COM_TIM11_IRQ,
    TIM1_CC_IRQ,
    TIM2_IRQ,
    TIM3_IRQ,
    TIM4_IRQ,
    I2C1_EV_IRQ,
    I2C1_ER_IRQ,
    I2C2_EV_IRQ,
    I2C2_ER_IRQ,
    SPI1_IRQ,
    SPI2_IRQ,
    USART1_IRQ,
    USART2_IRQ,
    EXTI15_10_IRQ=40,
    EXTI17_IRQ,
    EXTI18_IRQ,
    DMA1_Stream7_IRQ=47,
    SDIO_IRQ=49,
    TIM5_IRQ,
    SPI3_IRQ,
    DMA2_Stream0_IRQ=56,
    DMA2_Stream1_IRQ,
    DMA2_Stream2_IRQ,
    DMA2_Stream3_IRQ,
    DMA2_Stream4_IRQ,
    OTG_FS_IRQ=67,
    DMA2_Stream5_IRQ,
    DMA2_Stream6_IRQ,
    DMA2_Stream7_IRQ,
    USART6_IRQ,
    I2C3_EV_IRQ,
    I2C3_ER_IRQ,
    FPU_IRQ = 81,
    SPI4_IRQ = 84,
    _INT_Num
} IRQn_t;

#endif