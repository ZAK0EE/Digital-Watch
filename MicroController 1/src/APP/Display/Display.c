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
} CursorPos;




/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
static char frameBuffer[DISPLAY_HEIGHT][DISPLAY_WIDTH] = {0};
static CursorPos currentCurPos = {.row = 0, .col = 0};

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
                LCD_writeStringAsync(LCD1, frameBuffer[frameIdx], DISPLAY_WIDTH);
                state++;
                break;
            case 4:
                LCD_state = LCD_getState(LCD1);
                if(LCD_state == LCD_STATE_READY) state++;         
                break;                
            case 5:
                frameIdx++;
                if(frameIdx >= DISPLAY_HEIGHT)
                    frameIdx = 0;
                state = 0;
                break;
        }
        
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
    uint8_t col = currentCurPos.col;

    /* Calculating the center position */
    uint8_t centerPos = (DISPLAY_WIDTH - len) / 2;
    currentCurPos.col = centerPos;
    /* Truncation to prevent over rendering */
    uint8_t cpyLen = MIN(len, DISPLAY_WIDTH - centerPos);

    /* Copying the content to the frame buffer */
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