#include "HAL/Switch/SWITCH.h" // Include the switch module header file

// Define the configuration array for switches
const SWITCH_Confg_t SWITCH_Confg[SWITCH_Number] = 
{
    [INC_Switch] = // Configuration for SWITCH_1
    {
        .Port = SWITCH_PORT_A, 
        .Pin  = SWITCH_PIN_0,  
    },

    [Mode_Switch] = 
    {
        .Port = SWITCH_PORT_A,
        .Pin  = SWITCH_PIN_0,
    },

    [Edit_Switch] = 
    {
        .Port = SWITCH_PORT_A,
        .Pin  = SWITCH_PIN_0, 
    }
}; 
