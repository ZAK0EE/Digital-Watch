/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "Comm.h"
#include "../Mode/Mode.h"
#include "../DateAndTime/DateAndTime.h"
#include "../../HAL/HUSART/HUSART.h"
#include "../Frames_cfg.h"
#include "../StopWatch/StopWatch.h"
#include "../../HAL/LCD/LCD.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/


static u8 RxBuffer[1] = {0};
static u8 TxBuffer[1] = {0};

void Comm_collect(void);

HUSART_UserReq_t recieveReq = {.USART_ID = HUSART1_ID, .Ptr_buffer = RxBuffer, .Buff_Len = 1, .Buff_cb = Comm_collect};
HUSART_UserReq_t sendReq = {.USART_ID = HUSART1_ID, .Ptr_buffer = TxBuffer, .Buff_Len = 1, .Buff_cb = NULLPTR};

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

Comm_errorStatus_t Comm_publish(u8 frame)
{
    Comm_errorStatus_t errorState = Comm_Nok;

    if(frame == FRAME_SWITCH_NO1 ||
       frame == FRAME_SWITCH_NO2 || 
       frame == FRAME_SWITCH_NO3 || 
       frame == FRAME_SWITCH_NO1_AND_NO2)
       {
            TxBuffer[0] = frame;
            HUART_SendBuffAsync(&sendReq);
            errorState = Comm_Ok;
       } 

       return errorState;
}

void Comm_collect(void)
{
    Comm_errorStatus_t errorState = Comm_Nok;
    Mode_modeType_t currMode = getMode();
    u8 frame = RxBuffer[0];

    if (frame == FRAME_SWITCH_NO1 ||
        frame == FRAME_SWITCH_NO2 ||
        frame == FRAME_SWITCH_NO3 ||
        frame == FRAME_SWITCH_NO1_AND_NO2)
    {
        errorState = Comm_Ok;
    }
    else
    {
        errorState = Comm_InvalidRxFrame;
    }

    if (errorState == Comm_Ok)
    {
        switch (currMode)
        {
        case Mode_EditDateAndTime:
        LCD_Clear_Async(LCD1);
            edit_DateTime(frame);
            break;

        case Mode_StopWatch:
        LCD_Clear_Async(LCD1);
            controlSW(frame);
            break;

        case Mode_DateAndTime:
        {
            switch (frame)
            {
            case FRAME_SWITCH_NO1:
                /* Do Nothing */
                break;

            case FRAME_SWITCH_NO2:
            LCD_Clear_Async(LCD1);
                changeMode(frame);
                break;
            
            case FRAME_SWITCH_NO3:
            LCD_Clear_Async(LCD1);
                enterEditDateAndTime();
                break;

            default:
                break;
            }
        }
        default:
            break;
        }
    }
}