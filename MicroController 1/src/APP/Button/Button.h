/**
 * @file Button.h
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef APP_BUTTON_BUTTON_H_
#define APP_BUTTON_BUTTON_H_

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

/**
 * @brief Enumeration representing the state of a button (pressed or not pressed).
 */
typedef enum
{
    BUTTON_IS_NOT_PRESSED, /**< Button is not pressed. */
    BUTTON_IS_PRESSED      /**< Button is pressed. */
} Button_state_t;

/**
 * @brief Enumeration representing the hold state of a button (held or not held).
 */
typedef enum
{
    BUTTON_IS_NOT_HELD, /**< Button is not held. */
    BUTTON_IS_HELD      /**< Button is held. */
} Button_holdState_t;

/**
 * @brief Enumeration representing the source of a button (GPIO or UART).
 */
typedef enum
{
    BUTTON_SOURCE_GPIO, /**< Button source is GPIO. */
    BUTTON_SOURCE_UART  /**< Button source is UART. */
} Button_source_t;

/**
 * @brief Configuration structure for a button.
 */
typedef struct
{
    uint8_t SwitchID;               /**< ID of the switch/button. */
    Button_source_t ButtonSource;   /**< Source of the button (GPIO or UART). */
} Button_config_t;


/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/**
 * @brief Initializes the buttons.
 */
void Button_init();

/**
 * @brief Checks if a button is currently pressed.
 * 
 * @param ButtonID The ID of the button to check.
 * @return The state of the button (pressed or not).
 */
Button_state_t Button_isPressed(Button_ID_t ButtonID);

/**
 * @brief Checks if a button is pressed and held.
 * 
 * @param ButtonID The ID of the button to check.
 * @return The hold state of the button (pressed and held or not).
 */
Button_holdState_t Button_isPressedAndHeld(Button_ID_t ButtonID);




























#endif // APP_BUTTON_BUTTON_H_