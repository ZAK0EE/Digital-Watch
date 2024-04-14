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
char frameBuffer[DISPLAY_WIDTH][DISPLAY_HEIGHT] = {0};
CursorPos currentCurPos = {.row = 0, .col = 0};

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/



void Display_task(void)
{

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

/**
 * @brief Prints the content of the buffer centered on the display asynchronously.
 * 
 * @param buffer Pointer to the buffer containing the content to be printed.
 * @param len Length of the content in the buffer.
 */
void Display_printCenteredAsync(char *buffer, uint8_t len)
{
    uint8_t row = currentCurPos.row;
    uint8_t col = currentCurPos.col;

    /* Calculating the center position */
    uint8_t centerPos = (DISPLAY_WIDTH - len) / 2;

    /* Truncation to prevent over rendering */
    uint8_t cpyLen = MIN(len, DISPLAY_WIDTH - centerPos);

    /* Copying the content to the frame buffer */
    strncpy(&frameBuffer[row][centerPos], buffer, cpyLen);
}

/**
 * @brief Sets the cursor position on the display.
 * 
 * @param column The column position (0-based index) where the cursor should be set.
 * @param row The row position (0-based index) where the cursor should be set.
 */
void Display_setCursorAsync(uint8_t column, uint8_t row)
{
    currentCurPos.row = MIN(DISPLAY_HEIGHT - 1, row);
    currentCurPos.col = MIN(DISPLAY_WIDTH - 1, column);
}

/**
 * @brief Clears the content displayed on the screen.
 */
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
}