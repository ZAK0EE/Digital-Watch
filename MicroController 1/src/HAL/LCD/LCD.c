/**
 * @file LCD.c
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
#include "assertparam.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define LCD_TIMEMS_WAKEUP 31UL
#define LCD_TIMEMS_SEND   1UL
#define LCD_TIMEMS_FUNCTIONSET 1UL
/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef enum
{
    LCD_SEND_CMD,
    LCD_SEND_DATA,
}SendType_t;
typedef enum
{
    LCD_PHS_OFF,
    LCD_PHS_INIT,
    LCD_PHS_OPERATION,
}
PhaseState_t;

typedef enum
{
    LCD_OPERATION_NONE,
    LCD_OPERATION_WRITE_STRING,
    LCD_OPERATION_SETCURSOR_POS, 
    LCD_OPERATION_CLEAR_SCREEN,   
}
OperationState_t;

typedef enum
{
    LCD_INIT_PINS,
    LCD_INIT_FUNCTION_SET,
    LCD_INIT_DISPLAY_CONTROL,
    LCD_INIT_DISPLAY_CLEAR,
    LCD_INIT_ENTRYMODE_SET,
}
InitStep_t;

typedef enum
{
    LCD_WRITELCD_READY,
    LCD_WRITELCD_WRITEPINS_4BIT,
    LCD_WRITELCD_TRIGGER,
    LCD_WRITELCD_TRIGGER_4BIT,
}WriteLCDState_t;

typedef enum
{
    GENERALSTATE_DONE,
    GENERALSTATE_N_DONE,
}GeneralState_t;

typedef struct
{
    uint8_t *Buffer;
    uint32_t len;
    uint32_t index;
}StringRequest_t;

typedef struct 
{
    uint8_t row;
    uint8_t col;
}CursorPos;

typedef struct
{
    StringRequest_t StringRequest;
    CursorPos       CursorPos;
}UserRequest_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
static PhaseState_t CurrentPhase[_NUM_OF_LCDS] = {LCD_PHS_OFF};
static OperationState_t CurrentOperation[_NUM_OF_LCDS] = {LCD_OPERATION_NONE};
static InitStep_t CurrentInitStep[_NUM_OF_LCDS] = {LCD_INIT_PINS};
static WriteLCDState_t CurrentWriteCommandState[_NUM_OF_LCDS] = {LCD_WRITELCD_READY};
static uint32_t elapsedTimeMS[_NUM_OF_LCDS] = {0};

static UserRequest_t UserRequest[_NUM_OF_LCDS] = {0};

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

/**
 * @brief Initializes the LCD.
 */
static void Init(LCD_ID ID);
static void Operate(LCD_ID ID);

/* Initialization functions */
static void PinsInit(LCD_ID ID);

static void WriteLCD(LCD_ID LCD_ID, uint8_t Command, SendType_t SendType);
static void WritePins(LCD_ID LCD_ID, uint8_t value);
/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

static void WritePins(LCD_ID LCD_ID, uint8_t value)
{
 	LCD_Config_t const *CurrentLCD = &(LCD_Config[LCD_ID]);

	GPIO_Port_t		    PortID;
	GPIO_Pin_t		    PinNum;
	GPIO_PinState_t    PinState;

	uint8_t LCDPinCounter = 0;
	uint8_t NumOfPins = (CurrentLCD->DataLength == LCD_DL_8BIT) ? 8 : 4;

	for(LCDPinCounter = 0; LCDPinCounter < NumOfPins; LCDPinCounter++)
	{
		PortID		= CurrentLCD->Pins[LCDPinCounter].PortID;
		PinNum		= (GPIO_Pin_t)CurrentLCD->Pins[LCDPinCounter].PinNum;
		PinState = (((value  >> LCDPinCounter) & 1 ) == 1) ? GPIO_PINSTATE_SET : GPIO_PINSTATE_RESET;

		GPIO_setPinValue(PortID, PinNum, PinState);	
	}
   
}
static void WriteLCD(LCD_ID LCD_ID, uint8_t Command, SendType_t SendType)
{
    LCD_Config_t const *CurrentLCD = &(LCD_Config[LCD_ID]);

    GPIO_Port_t		RSPortID  = (GPIO_Port_t)CurrentLCD->RSPin.PortID;
    GPIO_Pin_t		RSPinNum  = (GPIO_Pin_t)CurrentLCD->RSPin.PinNum;
    GPIO_PinState_t RSPinState =  SendType; 

	GPIO_Port_t		ENPortID  = (GPIO_Port_t)CurrentLCD->EnablePin.PortID;
	GPIO_Pin_t		ENPinNum  = (GPIO_Pin_t)CurrentLCD->EnablePin.PinNum;
    switch(CurrentWriteCommandState[LCD_ID])
    {        
        case LCD_WRITELCD_READY:
        {
            /* RS is low for command, high for data*/
            GPIO_setPinValue(RSPortID, RSPinNum, RSPinState);
            
            if(CurrentLCD->DataLength == LCD_DL_8BIT)
            {
                WritePins(LCD_ID, Command);

            }
            else
            {
                WritePins(LCD_ID, Command >> 4);
                
            }

            /* Start Send */
            GPIO_setPinValue(ENPortID, ENPinNum, GPIO_PINSTATE_SET);
            
            CurrentWriteCommandState[LCD_ID] = LCD_WRITELCD_TRIGGER;      
            break;
        }
        case LCD_WRITELCD_TRIGGER:
        {
            /* Send is done*/
            GPIO_setPinValue(ENPortID, ENPinNum, GPIO_PINSTATE_RESET);

            if(CurrentLCD->DataLength == LCD_DL_4BIT)
            {
                CurrentWriteCommandState[LCD_ID] = LCD_WRITELCD_WRITEPINS_4BIT;
            }
            else
            {
                CurrentWriteCommandState[LCD_ID] = LCD_WRITELCD_READY;
            }
            break;
        }        
        case LCD_WRITELCD_WRITEPINS_4BIT:
        {
            WritePins(LCD_ID, Command);
            GPIO_setPinValue(ENPortID, ENPinNum, GPIO_PINSTATE_SET);
            CurrentWriteCommandState[LCD_ID] = LCD_WRITELCD_TRIGGER_4BIT;

            break;
        }

        case LCD_WRITELCD_TRIGGER_4BIT:
        {
            /* Send is done*/
            GPIO_setPinValue(ENPortID, ENPinNum, GPIO_PINSTATE_RESET);            
            CurrentWriteCommandState[LCD_ID] = LCD_WRITELCD_READY;
            break;
        }

    }

}


static void PinsInit(LCD_ID ID)
{
    LCD_Config_t const *CurrentLCD = &(LCD_Config[ID]);

    /* Initializing current LCD pins' direction */
    uint8_t LCDPinCounter = 0;
    uint8_t NumOfPins = (CurrentLCD->DataLength == LCD_DL_8BIT) ? 8 : 4;
    GPIO_PinConfig_t PinConfig;
    for(LCDPinCounter = 0; LCDPinCounter < NumOfPins; LCDPinCounter++)
    {
        PinConfig.Port	        = (GPIO_Port_t)CurrentLCD->Pins[LCDPinCounter].PortID;
        PinConfig.PinNumber		= (GPIO_Pin_t)CurrentLCD->Pins[LCDPinCounter].PinNum;
        PinConfig.PinSpeed      = GPIO_SPEED_MEDIUM;
        PinConfig.PinMode       = GPIO_MODE_OUTPUT_PUSHPULL_NOPULL;
        GPIO_initPin(&PinConfig);
    }

    /* Initializing current RS pin direction */
    PinConfig.Port		    = (GPIO_Port_t)CurrentLCD->RSPin.PortID;
    PinConfig.PinNumber		= (GPIO_Pin_t)CurrentLCD->RSPin.PinNum;
    GPIO_initPin(&PinConfig);


    /* Initializing current Enable pin direction */
    PinConfig.Port		    = (GPIO_Port_t)CurrentLCD->EnablePin.PortID;
    PinConfig.PinNumber		= (GPIO_Pin_t)CurrentLCD->EnablePin.PinNum;
    GPIO_initPin(&PinConfig);

}

void LCD_task(void)
{
    LCD_ID LCD_ID = 0;
    for(LCD_ID = 0; LCD_ID < _NUM_OF_LCDS; LCD_ID++)  
    {
        switch(CurrentPhase[LCD_ID])
        {
            case LCD_PHS_OFF:
                break;
            case LCD_PHS_INIT:
                Init(LCD_ID);
                break;
            
            case LCD_PHS_OPERATION:
                Operate(LCD_ID);
                break;
        }
        elapsedTimeMS[LCD_ID]++;

    }   

}

void Init(LCD_ID ID)
{

    switch(CurrentInitStep[ID])
    {
        case LCD_INIT_PINS:
        {

            PinsInit(ID);
            elapsedTimeMS[ID] = 0;
            CurrentInitStep[ID]++;

            break;
        }
        case LCD_INIT_FUNCTION_SET:
        {
            if(elapsedTimeMS[ID] >= LCD_TIMEMS_WAKEUP)
            { 
                
                LCD_ID LCD_ID = 0;
                for(LCD_ID = 0; LCD_ID < _NUM_OF_LCDS; LCD_ID++)        
                {
                    LCD_Config_t const *CurrentLCD = &(LCD_Config[LCD_ID]);        
                    uint8_t FunctionSetCommand = 0;

                    FunctionSetCommand = (1 << 5) | (CurrentLCD->DataLength << 4) | (1 << 3) | (CurrentLCD->Font << 2);
                    //FunctionSetCommand = 0x30;
                    if(CurrentLCD->DataLength == LCD_DL_4BIT)
                    {
                        WriteLCD(LCD_ID, 0b0010, LCD_SEND_CMD);
                    }
                    WriteLCD(LCD_ID, FunctionSetCommand, LCD_SEND_CMD);
                    
                }
                if(CurrentWriteCommandState[0] == LCD_WRITELCD_READY && elapsedTimeMS[ID] > 8)
                {
                    CurrentInitStep[ID]++;
                    elapsedTimeMS[ID] = 0;

                }                        

            }

            break;
        }

        case LCD_INIT_DISPLAY_CONTROL:
        {
            /* Display ON/OFF Control */
            LCD_ID LCD_ID = 0;
            for(LCD_ID = 0; LCD_ID < _NUM_OF_LCDS; LCD_ID++)
            {
                LCD_Config_t const *CurrentLCD = &(LCD_Config[LCD_ID]);

                uint8_t cmd = 0;
                cmd = (1 << 3) | (1 << 2) | (CurrentLCD->CursorState << 1) | (CurrentLCD->CursorBlinkingState >> 0);
                WriteLCD(LCD_ID, cmd, LCD_SEND_CMD);                
            }        
            if(CurrentWriteCommandState[0] == LCD_WRITELCD_READY && elapsedTimeMS[ID] > 8)
            {
                CurrentInitStep[ID]++;
                elapsedTimeMS[ID] = 0;

            }

            break;
        }
        case LCD_INIT_DISPLAY_CLEAR:
        {
             /* Display Clear */
            LCD_ID LCD_ID = 0;
            for(LCD_ID = 0; LCD_ID < _NUM_OF_LCDS; LCD_ID++)
            {
                uint8_t cmd = 0;
                cmd = 1;
                WriteLCD(LCD_ID, cmd, LCD_SEND_CMD);                
            }        
            
            if(CurrentWriteCommandState[0] == LCD_WRITELCD_READY && elapsedTimeMS[ID] > 8)
            {
                CurrentInitStep[ID]++;
                elapsedTimeMS[ID] = 0;

            }           
            break;
        }
        case LCD_INIT_ENTRYMODE_SET:
        {
            /* Display Clear */
            LCD_ID LCD_ID = 0;
            for(LCD_ID = 0; LCD_ID < _NUM_OF_LCDS; LCD_ID++)
            {
                uint8_t cmd = 0;
                cmd |= (1 << 2) | (1 << 1) | (0 << 0);
                WriteLCD(LCD_ID, cmd, LCD_SEND_CMD);                
            }        
            
            if(CurrentWriteCommandState[0] == LCD_WRITELCD_READY && elapsedTimeMS[ID] > 5)
            {
                CurrentPhase[ID] = LCD_PHS_OPERATION;
                elapsedTimeMS[ID] = 0;

            }           
            break;
        }
    }

 
}

static void Operate(LCD_ID ID)
{

    switch(CurrentOperation[ID])
    {
        case LCD_OPERATION_WRITE_STRING:
        {
            uint32_t index = UserRequest[ID].StringRequest.index;
            uint8_t *buffer = UserRequest[ID].StringRequest.Buffer;

            WriteLCD(ID, buffer[index], LCD_SEND_DATA);

            if(CurrentWriteCommandState[ID] == LCD_WRITELCD_READY)
            {
                UserRequest[ID].StringRequest.index++;
            }

            if((CurrentWriteCommandState[ID] == LCD_WRITELCD_READY) && UserRequest[ID].StringRequest.index == UserRequest[ID].StringRequest.len)
            {
                CurrentOperation[ID] = LCD_OPERATION_NONE;
            }
            break;
        }
        case LCD_OPERATION_SETCURSOR_POS:
        {
            uint8_t cmd = 0;
            cmd |= (1 << 7);

            cmd |= (UserRequest[ID].CursorPos.row * 0x40) + UserRequest[ID].CursorPos.col;
            WriteLCD(ID, cmd, LCD_SEND_CMD);

            if(CurrentWriteCommandState[ID] == LCD_WRITELCD_READY)
            {
                CurrentOperation[ID] = LCD_OPERATION_NONE;
            }
            break;
        }
        case LCD_OPERATION_CLEAR_SCREEN:
        {
            uint8_t cmd = 0;
            cmd = 0x01;

            WriteLCD(ID, cmd, LCD_SEND_CMD);

            if(CurrentWriteCommandState[ID] == LCD_WRITELCD_READY)
            {
                CurrentOperation[ID] = LCD_OPERATION_NONE;
            }
            break;
        }
        case LCD_OPERATION_NONE:break;
    }        

}

void LCD_init(LCD_ID ID)
{
    CurrentPhase[ID] = LCD_PHS_INIT;
}
LCD_State_t LCD_getState(LCD_ID ID)
{
    return ((CurrentPhase[ID] == LCD_PHS_OPERATION) && (CurrentOperation[ID] == LCD_OPERATION_NONE)) ? LCD_STATE_READY : LCD_STATE_BUSY;
}

void LCD_clearScreenAsync(LCD_ID ID)
{
    assert_param(ID < _NUM_OF_LCDS);
    if(CurrentOperation[ID] == LCD_OPERATION_NONE)
    {
        CurrentOperation[ID] = LCD_OPERATION_SETCURSOR_POS;
    }  
}
void LCD_setCursorPositionAsync(LCD_ID ID, uint8_t row, uint8_t col)
{
    assert_param(ID < _NUM_OF_LCDS);
     if(CurrentOperation[ID] == LCD_OPERATION_NONE)
    {
        UserRequest[ID].CursorPos.row = row;
        UserRequest[ID].CursorPos.col = col;

        CurrentOperation[ID] = LCD_OPERATION_SETCURSOR_POS;
    }   

}
void LCD_writeStringAsync(LCD_ID ID, char* str, uint32_t len)
{
    assert_param(ID < _NUM_OF_LCDS);

    if(CurrentOperation[ID] == LCD_OPERATION_NONE)
    {
        UserRequest[ID].StringRequest.Buffer = (uint8_t*)str;
        UserRequest[ID].StringRequest.len = len;
        UserRequest[ID].StringRequest.index = 0;

        CurrentOperation[ID] = LCD_OPERATION_WRITE_STRING;
    }
}