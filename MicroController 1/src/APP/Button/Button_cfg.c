/**
 * @file Button_cfg.c
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "Button.h"
#include "Button_cfg.h"



/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/


/**
 * @brief Array holding configurations for each button.
 */
Button_config_t Button_config[_NUM_OF_BUTTONS]
{
    [BUTTON_INCREMENT]=
    {
        .SwitchID = 0,
        .ButtonSource = BUTTON_SOURCE_UART,
    },
    [BUTTON_MODE]=
    {
        .SwitchID = 0,
        .ButtonSource = BUTTON_SOURCE_UART,
    },
    [BUTTON_EDIT]=
    {
        .SwitchID = 0,
        .ButtonSource = BUTTON_SOURCE_UART,
    },

    [BUTTON_TOSEND_INCREMENT]=
    {
        .SwitchID = 0,
        .ButtonSource = BUTTON_SOURCE_GPIO,
    },  
    [BUTTON_TOSEND_MODE]=
    {
        .SwitchID = 1,
        .ButtonSource = BUTTON_SOURCE_GPIO,
    },  
    [BUTTON_TOSEND_EDIT]=
    {
        .SwitchID = 2,
        .ButtonSource = BUTTON_SOURCE_GPIO,
    },                     
}