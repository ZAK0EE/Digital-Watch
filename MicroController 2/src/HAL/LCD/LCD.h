#ifndef LCD_H_    // Header guard: prevents multiple inclusion of the same header file
#define LCD_H_

#include "LCD_Confg.h"    // Include necessary configuration header file

// Function prototypes

/**
 * @brief Clears the display of the specified LCD module.
 * 
 * @param LCD_Name: The name or identifier of the LCD module.
 */
void LCD_Clear_Async(LCD_Modules_t LCD_Name);

/**
 * @brief Sets the cursor position on the specified LCD module.
 * 
 * @param LCD_Name: The name or identifier of the LCD module.
 * @param Row: The row number where the cursor will be placed.
 * @param Col: The column number where the cursor will be placed.
 */
void LCD_Set_Cursor_Async(LCD_Modules_t LCD_Name, u8 Row, u8 Col);

/**
 * @brief Writes a string to the specified LCD module.
 * 
 * @param LCD_Name: The name or identifier of the LCD module.
 * @param string: Pointer to the string to be written.
 */
void LCD_Write_String_Async(LCD_Modules_t LCD_Name, const char *string);

#endif /*LCD_H_*/    // End of header guard
