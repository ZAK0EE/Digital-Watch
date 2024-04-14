/**
 * @file Display.h
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef APP_DISPLAY_DISPLAY_H_
#define APP_DISPLAY_DISPLAY_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include <stdint.h>

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/


/**
 * @brief Task function to be called by the scheduler to manage display operations.
 */
void Display_task(void);

/**
 * @brief Prints the content of the buffer asynchronously.
 * 
 * @param buffer Pointer to the buffer containing the content to be printed.
 * @param len Length of the content in the buffer.
 */
void Display_init(void);

/**
 * @brief Prints the content of the buffer asynchronously.
 * 
 * @param buffer Pointer to the buffer containing the content to be printed.
 * @param len Length of the content in the buffer.
 */
void Display_printAsync(char *buffer, uint8_t len);

/**
 * @brief Prints the content of the buffer centered on the display asynchronously.
 * 
 * @param buffer Pointer to the buffer containing the content to be printed.
 * @param len Length of the content in the buffer.
 */
void Display_printCenteredAsync(char *buffer, uint8_t len);

/**
 * @brief Sets the cursor position on the display.
 * 
 * @param column The column position (0-based index) where the cursor should be set.
 * @param row The row position (0-based index) where the cursor should be set.
 */
void Display_setCursorAsync(uint8_t row, uint8_t column);

/**
 * @brief Clears the content displayed on the screen.
 */
void Display_clearScreenAsync(void);


#endif // APP_DISPLAY_DISPLAY_H_