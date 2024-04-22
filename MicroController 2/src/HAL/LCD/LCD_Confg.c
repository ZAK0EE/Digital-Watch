#include "LCD_Confg.h"    // Include LCD configuration header file

// Define LCD pin connections for each LCD module
const LCD_PortPin_t LCD_Pins_Arr[LCD_Num] = 
{
    [LCD1] =    // Configuration for LCD1
    {
        .LCD_arr[LCD_RS].Port  = LCD_PORT_A,    // Port B for Register Selection pin
        .LCD_arr[LCD_RS].Pin   = LCD_PIN_0,     // Pin 0 for Register Selection pin

        .LCD_arr[LCD_RW].Port  = LCD_PORT_A,    // Port B for Read/Write Selection pin
        .LCD_arr[LCD_RW].Pin   = LCD_PIN_1,     // Pin 1 for Read/Write Selection pin

        .LCD_arr[LCD_E].Port   = LCD_PORT_A,    // Port B for Enable Signal pin
        .LCD_arr[LCD_E].Pin    = LCD_PIN_2,     // Pin 2 for Enable Signal pin

        .LCD_arr[LCD_DB0].Port = LCD_PORT_A,    // Port B for Data pin 0
        .LCD_arr[LCD_DB0].Pin  = LCD_PIN_3,     // Pin 3 for Data pin 0

        .LCD_arr[LCD_DB1].Port = LCD_PORT_A,    // Port B for Data pin 1
        .LCD_arr[LCD_DB1].Pin  = LCD_PIN_4,     // Pin 4 for Data pin 1

        .LCD_arr[LCD_DB2].Port = LCD_PORT_A,    // Port B for Data pin 2
        .LCD_arr[LCD_DB2].Pin  = LCD_PIN_5,     // Pin 5 for Data pin 2

        .LCD_arr[LCD_DB3].Port = LCD_PORT_A,    // Port B for Data pin 3
        .LCD_arr[LCD_DB3].Pin  = LCD_PIN_6,     // Pin 6 for Data pin 3

        .LCD_arr[LCD_DB4].Port = LCD_PORT_A,    // Port B for Data pin 4
        .LCD_arr[LCD_DB4].Pin  = LCD_PIN_7,     // Pin 7 for Data pin 4

        .LCD_arr[LCD_DB5].Port = LCD_PORT_A,    // Port B for Data pin 5
        .LCD_arr[LCD_DB5].Pin  = LCD_PIN_8,     // Pin 8 for Data pin 5

        .LCD_arr[LCD_DB6].Port = LCD_PORT_A,    // Port B for Data pin 6
        .LCD_arr[LCD_DB6].Pin  = LCD_PIN_11,     // Pin 9 for Data pin 6

        .LCD_arr[LCD_DB7].Port = LCD_PORT_A,    // Port B for Data pin 7
        .LCD_arr[LCD_DB7].Pin  = LCD_PIN_12,    // Pin 10 for Data pin 7
    },
};
