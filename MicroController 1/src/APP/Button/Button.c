/**
 * @file Button.c
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "Button.h"
#include "Button_cfg.h"
#include "HAL/HSwitch/HSwitch.h"
#include "HAL/HUART/HUART.h"    
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define BUTTON_HOLD_REPEAT_TIMEMS     5000
#define BUTTON_HOLD_REPEAT_COUNT     (BUTTON_HOLD_REPEAT_TIMEMS / BUTTON_PERIODIC_TIMEMS)


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef struct
{
    Button_state_t CurrentRead;
    Button_state_t LastRead;
    uint32_t IsRepeated;
    Button_holdState_t IsHeld;
}
Button_status_t;


/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
static Button_status_t Button_status[_NUM_OF_BUTTONS] = {0};
static uint8_t UART_TX_frame = 0;
static uint8_t UART_RX_frame = 0;

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
void on_UART_Receive(void);

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

void on_UART_Receive(void)
{
    Button_ID_t ID = 0;
    for(ID = 0; ID < _NUM_OF_BUTTONS; ID++)
    {
        Button_status[ID].CurrentRead = (UART_RX_frame >> ID & 1);
    }

}

void Button_init()
{

    HUSART_UserReq_t HUART_RxReq = 
    {
        .USART_ID = USART1_ID,
        .Ptr_buffer= &UART_RX_frame,
        .Buff_Len = 1,
        .Buff_cb = on_UART_Receive,
    };

    
    HUART_ReceiveBuffAsync(&HUART_RxReq);

}

Button_state_t Button_isPressed(Button_ID_t ButtonID)
{
    return Button_status[ButtonID].CurrentRead;
}
Button_holdState_t Button_isPressedAndHeld(Button_ID_t ButtonID)
{
    return Button_status[ButtonID].IsHeld;
}

void Button_task(void)
{
    Button_ID_t ID = 0;
    for(ID = 0; ID < _NUM_OF_BUTTONS; ID++)
    {
        /* Check if button was pressed last time */
        if(Button_status[ID].CurrentRead == BUTTON_IS_PRESSED && Button_status[ID].LastRead == BUTTON_IS_PRESSED)
        {
            Button_status[ID].IsRepeated++;
        }
        else
        {
            Button_status[ID].IsRepeated = 0;
        }
    
        /* Check if Button is held */
        if(Button_status[ID].IsRepeated >= BUTTON_HOLD_REPEAT_COUNT)
        {
            Button_status[ID].IsHeld = BUTTON_IS_HELD;
        }
        else
        {
            Button_status[ID].IsHeld = BUTTON_IS_NOT_HELD;
        }

        Button_status[ID].LastRead = Button_status[ID].CurrentRead;
    }

    
    /* Encode Buttons reads */
    uint8_t status = 0;
    UART_TX_frame = 0;

    HSwitch_Get_Status(Increment, &status);
    UART_TX_frame |= (status << Increment);

    HSwitch_Get_Status(Mode, &status);
    UART_TX_frame |= (status<< Mode);

    HSwitch_Get_Status(Edit, &status);
    UART_TX_frame |= (status << Edit); 

    /* Send the UART frame */
    HUSART_UserReq_t HUART_TxReq =
    {
        .USART_ID = USART1_ID,
        .Ptr_buffer= &UART_TX_frame,
        .Buff_Len = 1,
        .Buff_cb = 0,
    };
    HUART_SendBuffAsync(&HUART_TxReq);
}
