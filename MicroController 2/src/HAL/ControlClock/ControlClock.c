#include "ControlClock.h"

void Control_Clock_Init(Control_Clock_Pin_Confg_t *Pin_Confg)
{
    switch (Pin_Confg->port)
    {
    case GPIO_PORT_A:
        RCC_Enable_AHB1_Peripheral_Clock(PERIPHERAL_AHB1_GPIOA);
        break;
    case GPIO_PORT_B:
        RCC_Enable_AHB1_Peripheral_Clock(PERIPHERAL_AHB1_GPIOB);
        break;
    case GPIO_PORT_C:
        RCC_Enable_AHB1_Peripheral_Clock(PERIPHERAL_AHB1_GPIOC);
        break;
    
    default:
        break;
    }

    switch (Pin_Confg->AF)
    {
    case GPIO_AF07:
        RCC_Enable_APB1_Peripheral_Clock(PERIPHERAL_APB1_UART2);
        RCC_Enable_APB2_Peripheral_Clock(PERIPHERAL_APB2_UART1);
        break;
    case GPIO_AF08:
        RCC_Enable_APB2_Peripheral_Clock(PERIPHERAL_APB2_UART6);
        break;
    
    default:
        break;
    }

    GPIO_Init_Pin(&Pin_Confg);
}

void Control_Clock_Set_Pin_State(u8 PORT, u8 PIN, u8 State)
{
    GPIO_Set_Pin_State(PORT, PIN, State);
}

void Control_Clock_Get_Pin_State(u8 PORT, u8 PIN, u8 *State)
{
    GPIO_Get_Pin_State(PORT, PIN, State);
}