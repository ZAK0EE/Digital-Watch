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
static CursorPos_t currentCurPos = {.row = 0, .col = 0};
static BlinkChar_t BlinkingChar = {
    .isBlinking = 0,
    .charPos = {0, 0},
    .charBuffer = 0,
    .blinkRateMS = 250,
    .blinkTimerMS = 0,
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

    LCD_State_t LCD_state = 0; 



        switch(state)
        {
            case 0:
                LCD_state = LCD_getState(LCD1);
                if(LCD_state == LCD_STATE_READY) state++;
                break;
            case 1:
                LCD_setCursorPositionAsync(LCD1, frameIdx, 0);
                state++;
                break;
            case 2:
                LCD_state = LCD_getState(LCD1);
                if(LCD_state == LCD_STATE_READY) state++;         
                break;       
            case 3:
                if(refreshTimerMS <= 0)
                {
                    /* Handling blinking logic */
                    if(BlinkingChar.isBlinking && BlinkingChar.charPos.row == frameIdx &&BlinkingChar.blinkTimerMS <= 0)
                    {
                        if(BlinkingChar.isAppear)
                        {
                            frameBuffer[BlinkingChar.charPos.row][BlinkingChar.charPos.col] = ' ';                                
                        }
                        else
                        {
                            frameBuffer[BlinkingChar.charPos.row][BlinkingChar.charPos.col] = BlinkingChar.charBuffer;
                        }
                        BlinkingChar.isAppear = !BlinkingChar.isAppear;
                        BlinkingChar.blinkTimerMS = BlinkingChar.blinkRateMS;
                    }                        
                    LCD_writeStringAsync(LCD1, frameBuffer[frameIdx], DISPLAY_WIDTH);
                    state++;
                }
                break;
            case 4:
                LCD_state = LCD_getState(LCD1);
                if(LCD_state == LCD_STATE_READY) state++;         
                break;                
            case 5:
                frameIdx++;
                if(frameIdx >= DISPLAY_HEIGHT)
                {
                    frameIdx = 0;
                    refreshTimerMS = DISPLAY_REFRESH_RATEMS;
                }
                state = 0;
                break;
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

    /* Update blinking buffer */
    if(BlinkingChar.isBlinking && BlinkingChar.charPos.row == row &&
        BlinkingChar.charPos.col >= col && BlinkingChar.charPos.col <= (col + cpyLen - 1))
    {
        BlinkingChar.charBuffer = frameBuffer[BlinkingChar.charPos.row][BlinkingChar.charPos.col];
    }



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

    /* Copying the content to the frame buffer */
    strncpy(&frameBuffer[row][currentCurPos.col], buffer, cpyLen);

    /* Update blinking buffer */
    if(BlinkingChar.isBlinking && BlinkingChar.charPos.row == row &&
        (BlinkingChar.charPos.col >= currentCurPos.col && BlinkingChar.charPos.col <= (currentCurPos.col + cpyLen - 1)))
    {
        BlinkingChar.charBuffer = frameBuffer[BlinkingChar.charPos.row][BlinkingChar.charPos.col];
    }    
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
    
    if(BlinkingChar.isBlinking == 0)
    {
        BlinkingChar.isBlinking = 1;
        BlinkingChar.charPos = (CursorPos_t){row, col};
        BlinkingChar.charBuffer = frameBuffer[row][col];
        BlinkingChar.isAppear = 0;
        BlinkingChar.blinkTimerMS = BlinkingChar.blinkRateMS;
    }
    if(BlinkingChar.isBlinking == 1)
    {
        /* if blinking occurs in different place, else do nothing */
        if(BlinkingChar.charPos.col != col || BlinkingChar.charPos.row != row)
        {
            /* Restore last blinking character */
            frameBuffer[BlinkingChar.charPos.row][BlinkingChar.charPos.col] = BlinkingChar.charBuffer;

            BlinkingChar.charPos = (CursorPos_t){row, col};
            BlinkingChar.charBuffer = frameBuffer[row][col];
            BlinkingChar.isAppear = 0;
            BlinkingChar.blinkTimerMS = BlinkingChar.blinkRateMS;

        }
    }

    
}

void Display_stopBlinkChar(void)
{
    BlinkingChar.isBlinking = 0;
    if(!BlinkingChar.isAppear)
        frameBuffer[BlinkingChar.charPos.row][BlinkingChar.charPos.col] = BlinkingChar.charBuffer;
}
