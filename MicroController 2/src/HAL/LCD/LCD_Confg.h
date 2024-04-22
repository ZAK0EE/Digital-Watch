#ifndef LCD_CONFG_H_    // Header guard: prevents multiple inclusion of the same header file
#define LCD_CONFG_H_

#include "../../lib/StdTypes.h"    // Include standard types header file

/* PORTS */

#define LCD_PORT_A 0x00000000    // Define port A for LCD
#define LCD_PORT_B 0x00000001    // Define port B for LCD
#define LCD_PORT_C 0x00000002    // Define port C for LCD

/* PINS */

#define LCD_PIN_0  0x00000000    // Define pin 0 for LCD
#define LCD_PIN_1  0x00000001    // Define pin 1 for LCD
#define LCD_PIN_2  0x00000002    // Define pin 2 for LCD
#define LCD_PIN_3  0x00000003    // Define pin 3 for LCD
#define LCD_PIN_4  0x00000004    // Define pin 4 for LCD
#define LCD_PIN_5  0x00000005    // Define pin 5 for LCD
#define LCD_PIN_6  0x00000006    // Define pin 6 for LCD
#define LCD_PIN_7  0x00000007    // Define pin 7 for LCD
#define LCD_PIN_8  0x00000008    // Define pin 8 for LCD
#define LCD_PIN_9  0x00000009    // Define pin 9 for LCD
#define LCD_PIN_10 0x0000000A    // Define pin 10 for LCD
#define LCD_PIN_11 0x0000000B    // Define pin 11 for LCD
#define LCD_PIN_12 0x0000000C    // Define pin 12 for LCD
#define LCD_PIN_13 0x0000000D    // Define pin 13 for LCD
#define LCD_PIN_14 0x0000000E    // Define pin 14 for LCD
#define LCD_PIN_15 0x0000000F    // Define pin 15 for LCD

// Enumeration for different LCD modules
typedef enum
{
    LCD1,

    LCD_Num,
} LCD_Modules_t;

// Structure for LCD pin connections
typedef struct
{
    u8 Port;    // Port number
    u8 Pin;     // Pin number
} LCD_Pins_Connection_t;



/* LCD Module pins */
typedef enum
{
    /* Register Selection */
    LCD_RS,
    
    /* Read/Write Selection */
    LCD_RW,
    
    /* Enable Signal */
    LCD_E,
    
    /* Low-Order Bi-Directional data bus */
    LCD_DB0,
    LCD_DB1,
    LCD_DB2,
    LCD_DB3,
    
    /* High-Order Bi-Directional data bus */
    LCD_DB4,
    LCD_DB5,
    LCD_DB6,
    LCD_DB7,
    
    LCD_Pins_Num,
} LCD_Pins_t;

// Structure for LCD port and pin connections
typedef struct
{
    LCD_Pins_Connection_t LCD_arr[LCD_Pins_Num];    // Array of LCD pin connections
} LCD_PortPin_t;

#endif /* LCD_CONFG_H_ */    // End of header guard
