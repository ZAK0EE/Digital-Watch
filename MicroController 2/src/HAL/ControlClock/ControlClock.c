#include "ControlClock.h"

void Enable_HAL_ClockControl(void)
{
    #if (CLOCK_CONTROL_GPIOA==ON)
        RCC_Enable_AHB1_Peripheral_Clock(PERIPHERAL_AHB1_GPIOA);
    #endif

    #if(CLOCK_CONTROL_GPIOB==ON)
        RCC_Enable_AHB1_Peripheral_Clock(PERIPHERAL_AHB1_GPIOB);
    #endif

    #if(CLOCK_CONTROL_GPIOC==ON)
        RCC_Enable_AHB1_Peripheral_Clock(PERIPHERAL_AHB1_GPIOC);
    #endif

    #if(CLOCK_CONTROL_UART1==ON)
        RCC_Enable_APB2_Peripheral_Clock(PERIPHERAL_APB2_UART1);
    #endif

    #if(CLOCK_CONTROL_UART2==ON)
        RCC_Enable_APB1_Peripheral_Clock(PERIPHERAL_APB1_UART2);
    #endif

    #if(CLOCK_CONTROL_UART6==ON)
        RCC_Enable_APB2_Peripheral_Clock(PERIPHERAL_APB2_UART6);
    #endif
}