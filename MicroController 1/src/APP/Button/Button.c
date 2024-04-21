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

typedef enum
{
    UARTMSG_INC_BUTTON_IS_PRESSED = 0x10,
    UARTMSG_MODE_BUTTON_IS_PRESSED =  0x20,
    UARTMSG_EDIT_BUTTON_IS_PRESSED =  0x30,
    UARTMSG_MODE_EDIT_BUTTON_IS_HELD =  0x40,
}Button_UART_MSG_t;

typedef enum
{
    BUTTONUART_INCREMENT = 3,        /**< Button for incrementing a value. */
    BUTTONUART_MODE,             /**< Button for changing the mode. */
    BUTTONUART_EDIT,             /**< Button for entering edit mode. */
    _NUM_OF_BUTTONSUART,          /**< Number of buttons. */
} ButtonUART_ID_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
static Button_status_t Button_status[_NUM_OF_BUTTONS + (_NUM_OF_BUTTONSUART - _NUM_OF_BUTTONS)] = {0};

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
    if(UART_RX_frame == UARTMSG_INC_BUTTON_IS_PRESSED)
    {
        Button_status[Increment].CurrentRead = BUTTON_IS_PRESSED;
    }
    else if(UART_RX_frame == UARTMSG_EDIT_BUTTON_IS_PRESSED)
    {
        Button_status[Edit].CurrentRead = BUTTON_IS_PRESSED;
    }   
    else if(UART_RX_frame == UARTMSG_MODE_BUTTON_IS_PRESSED)
    {
        Button_status[Mode].CurrentRead = BUTTON_IS_PRESSED;
    }    
    else if(UART_RX_frame == UARTMSG_MODE_EDIT_BUTTON_IS_HELD)
    {
        Button_status[Edit].IsHeld = BUTTON_IS_HELD;
        Button_status[Mode].IsHeld = BUTTON_IS_HELD;
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
    Button_state_t state = 3; 
    if(ButtonID < _NUM_OF_BUTTONS)
    {
        state = Button_status[ButtonID].CurrentRead;
        Button_status[ButtonID].CurrentRead = BUTTON_IS_NOT_PRESSED;
    }
    else
        state = Button_status[ButtonID].CurrentRead == BUTTON_IS_PRESSED && Button_status[ButtonID].LastRead == BUTTON_IS_NOT_PRESSED;
    return state;
}
Button_holdState_t Button_isPressedAndHeld(Button_ID_t ButtonID)
{
    return Button_status[ButtonID].IsHeld;
}

void Button_task(void)
{
    ButtonUART_ID_t ID = BUTTONUART_INCREMENT;
    for(; ID < _NUM_OF_BUTTONSUART; ID++)
    {
        Button_status[ID].LastRead = Button_status[ID].CurrentRead;
        
        HSwitch_Get_Status(ID - BUTTONUART_INCREMENT, &Button_status[ID].CurrentRead);

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

        
    }

    

    /* Send the UART frame */
    UART_TX_frame = 0;
    if(Button_isPressed(BUTTONUART_INCREMENT))
    {
        UART_TX_frame = UARTMSG_INC_BUTTON_IS_PRESSED;
    }
    else if(Button_isPressed(BUTTONUART_EDIT))
    {
        UART_TX_frame = UARTMSG_EDIT_BUTTON_IS_PRESSED;
    }   
    else if(Button_isPressed(BUTTONUART_MODE))
    {
        UART_TX_frame = UARTMSG_MODE_BUTTON_IS_PRESSED;
    }    
    else if(Button_isPressedAndHeld(BUTTONUART_MODE) && Button_isPressedAndHeld(BUTTONUART_EDIT))
    {
        UART_TX_frame = UARTMSG_MODE_EDIT_BUTTON_IS_HELD;
    }    

    if(UART_TX_frame != 0)
    {
        HUSART_UserReq_t HUART_TxReq =
        {
            .USART_ID = USART1_ID,
            .Ptr_buffer= &UART_TX_frame,
            .Buff_Len = 1,
            .Buff_cb = 0,
        };
        HUART_SendBuffAsync(&HUART_TxReq);
    }
}
