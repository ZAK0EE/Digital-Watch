/**
 * @file Display.c
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "Display.h"
#include "Display_cfg.h"
#include "LIB/std_types.h"

#include "HAL/LCD/LCD.h"
#include <string.h>

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/**
 * @brief Structure representing the position of the cursor on the display.
 */
typedef struct
{
    uint8_t row; /**< Row index of the cursor position. */
    uint8_t col; /**< Column index of the cursor position. */
} CursorPos_t;


typedef struct 
{
    uint8_t isBlinking;
    uint8_t isAppear;
    CursorPos_t charPos ;
    uint8_t charBuffer;
    uint32_t blinkRateMS;
    int32_t blinkTimerMS;
}BlinkChar_t;



/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
static char frameBuffer[DISPLAY_HEIGHT][DISPLAY_WIDTH] = {0};
static char LCDBuffer[DISPLAY_HEIGHT][DISPLAY_WIDTH] = {0};

static CursorPos_t currentCurPos = {.row = 0, .col = 0};
static BlinkChar_t BlinkingChar = {
    .isBlinking = 0,
    .charPos = {0, 0},
    .blinkRateMS = 500,
    .blinkTimerMS = 500,
}; 
/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/


void Display_task(void)
{
    static uint8_t state = 0;
    static uint8_t frameIdx = 0;
    static int32_t refreshTimerMS = DISPLAY_REFRESH_RATEMS;

    switch(state)
    {
        case 0:
            if(LCD_getState(LCD1) == LCD_STATE_READY)
            {
                /* Takes 2MS */
                LCD_setCursorPositionAsync(LCD1, frameIdx, 0);
                state++;
            }
            break;       
        case 1:
            if(refreshTimerMS <= 0)
            {
                if(frameIdx == 0)
                {
                    strncpy(LCDBuffer[0], frameBuffer[0], DISPLAY_WIDTH);
                    strncpy(LCDBuffer[1], frameBuffer[1], DISPLAY_WIDTH);
                    
                    /* Handling blinking logic */
                    if(BlinkingChar.isBlinking && BlinkingChar.blinkTimerMS <= 0)
                    {
                        if(!BlinkingChar.isAppear)
                        {
                            LCDBuffer[BlinkingChar.charPos.row][BlinkingChar.charPos.col] = ' ';                                
                        }
                        else
                        {
                            LCDBuffer[BlinkingChar.charPos.row][BlinkingChar.charPos.col] = frameBuffer[BlinkingChar.charPos.row][BlinkingChar.charPos.col];
                        }
                        BlinkingChar.isAppear = !BlinkingChar.isAppear;
                        BlinkingChar.blinkTimerMS = BlinkingChar.blinkRateMS;
                        
                    }                     
                }



                /* Takes 32MS */                
                LCD_writeStringAsync(LCD1, LCDBuffer[frameIdx], DISPLAY_WIDTH);
        
                state = 0;
                frameIdx++;
                if(frameIdx >= DISPLAY_HEIGHT)
                {
                    frameIdx = 0;
                    refreshTimerMS = DISPLAY_REFRESH_RATEMS;


                }
                break;                
            }         

    }
    BlinkingChar.blinkTimerMS -= DISPLAY_PERIODIC_CALLMS;
    refreshTimerMS -= DISPLAY_PERIODIC_CALLMS;
        
}



void Display_init(void)
{
    /* Filling the frame with spaces [empty frame] */
    for(int i = 0; i < DISPLAY_HEIGHT; i++)
    {
        for(int j = 0; j < DISPLAY_WIDTH; j++)
        {
            frameBuffer[i][j] = ' ';
        }
    }
}


void Display_printAsync(char *buffer, uint8_t len)
{   
    uint8_t row = currentCurPos.row;
    uint8_t col = currentCurPos.col;
    
    /* Truncation to prevent over rendering */
    uint8_t cpyLen = MIN(len, DISPLAY_WIDTH - col);

    strncpy(&frameBuffer[row][col], buffer, cpyLen);

    currentCurPos.col += cpyLen;
}


void Display_printCenteredAsync(char *buffer, uint8_t len)
{
    uint8_t row = currentCurPos.row;

    /* Calculating the center position */
    uint8_t centerPos = (DISPLAY_WIDTH - len) / 2;
    currentCurPos.col = centerPos;
    /* Truncation to prevent over rendering */
    uint8_t cpyLen = MIN(len, DISPLAY_WIDTH - centerPos);

    strncpy(&frameBuffer[row][currentCurPos.col], buffer, cpyLen);

    currentCurPos.col += cpyLen;

}


void Display_setCursorAsync(uint8_t row, uint8_t column)
{
    currentCurPos.row = MIN(DISPLAY_HEIGHT - 1, row);
    currentCurPos.col = MIN(DISPLAY_WIDTH - 1, column);
}


void Display_clearScreenAsync(void)
{
    /* Filling the frame with spaces [empty frame] */
    for(int i = 0; i < DISPLAY_HEIGHT; i++)
    {
        for(int j = 0; j < DISPLAY_WIDTH; j++)
        {
            frameBuffer[i][j] = ' ';
        }
    }
    
    currentCurPos.row = 0;
    currentCurPos.col = 0;
}

void Display_blinkChar(uint8_t row, uint8_t col)
{
    BlinkingChar.isBlinking = 1;
    BlinkingChar.charPos = (CursorPos_t){row, col};
}

void Display_stopBlinkChar(void)
{
    BlinkingChar.isBlinking = 0;
}
