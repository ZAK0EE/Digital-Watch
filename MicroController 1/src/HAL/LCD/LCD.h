#ifndef HAL_LCD_LCD_H_
#define HAL_LCD_LCD_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include <stdint.h>
#include "LCD_Cfg.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/**
 * @brief Enumeration for the data length (4-bit or 8-bit)
 */
typedef enum
{
    LCD_DL_4BIT,   /**< 4-bit data length */
    LCD_DL_8BIT    /**< 8-bit data length */
} LCD_DataLength_t;

/**
 * @brief Enumeration for cursor state (On or Off)
 */
typedef enum 
{
    LCD_CURSOR_STATE_OFF,  /**< Cursor off */
    LCD_CURSOR_STATE_ON    /**< Cursor on */
} LCD_CursorState_t;

/**
 * @brief Enumeration for cursor blinking state (Blinking On or Off)
 */
typedef enum 
{
    LCD_CURSOR_BLINKING_OFF,   /**< Cursor blinking off */
    LCD_CURSOR_BLINKING_ON     /**< Cursor blinking on */
} LCD_CursorBlinkingState_t;

/**
 * @brief Enumeration for font size (5x7 or 5x10)
 */
typedef enum
{
    LCD_FONT_5X7,    /**< 5x7 font size */
    LCD_FONT_5X10    /**< 5x10 font size */
} LCD_FontSize_t;

/**
 * @brief Enumeration for LCD states.
 */
typedef enum
{
    LCD_STATE_BUSY,   /**< LCD busy state */
    LCD_STATE_READY   /**< LCD ready state */
} LCD_State_t;


/**
 * @brief Structure to hold pin information
 */
typedef struct
{
    uint8_t PortID;    /**< Port ID */
    uint8_t PinNum;    /**< Pin number */
} LCD_Pin_t;

/**
 * @brief Structure defining LCD configuration
 */
typedef struct  
{
    LCD_DataLength_t DataLength;                    /**< Data length (4-bit or 8-bit) */
    LCD_FontSize_t Font;                            /**< Font size (5x7 or 5x10) */
    LCD_CursorState_t CursorState;                  /**< Cursor state (On or Off) */
    LCD_CursorBlinkingState_t CursorBlinkingState;  /**< Cursor blinking state (Blinking On or Off) */

    LCD_Pin_t EnablePin;					        /**< Pin configuration for Enable */
    LCD_Pin_t RSPin;						        /**< Pin configuration for RS (Register Select) */

    LCD_Pin_t Pins[8];						        /**< Pins configuration */
} LCD_Config_t;



extern LCD_Config_t LCD_Config[_NUM_OF_LCDS];
/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/**
 * @brief Performs LCD-related tasks. 
 * @note This function needs to be called periodically (Add it to the scheduler)
 */
void LCD_task(void);


/**
 * @brief Initializes the LCD identified by the given ID.
 * 
 * @param ID The ID of the LCD to initialize.
 */
void LCD_init(LCD_ID ID);

/**
 * @brief Retrieves the current state of the LCD.
 * @return LCD_State_t Current state of the LCD.
 */
LCD_State_t LCD_getState(LCD_ID ID);

/**
 * @brief Clears the screen of the specified LCD asynchronously.
 * @param ID The ID of the LCD to clear.
 */
void LCD_clearScreenAsync(LCD_ID ID);

/**
 * @brief Sets the cursor position on the specified LCD asynchronously.
 * @param ID The ID of the LCD to set the cursor position.
 */
void LCD_setCursorPositionAsync(LCD_ID ID, uint8_t row, uint8_t col);

/**
 * @brief Writes a string to the specified LCD asynchronously.
 * @param ID The ID of the LCD to write the string.
 * @param str Pointer to the string to write.
 * @param len Length of the string.
 */
void LCD_writeStringAsync(LCD_ID ID, char* str, uint32_t len);







#endif // HAL_LCD_LCD_H_