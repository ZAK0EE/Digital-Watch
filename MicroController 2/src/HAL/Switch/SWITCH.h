#ifndef SWITCH_H_ 
#define SWITCH_H_ 


#include "../../lib/StdTypes.h"
#include "SWITCH_Confg.h" 
 


// Definitions for switch ports
#define SWITCH_PORT_A 0x00000000
#define SWITCH_PORT_B 0x00000001
#define SWITCH_PORT_C 0x00000002

// Definitions for switch pins
#define SWITCH_PIN_0  0x00000000
#define SWITCH_PIN_1  0x00000001
#define SWITCH_PIN_2  0x00000002
#define SWITCH_PIN_3  0x00000003
#define SWITCH_PIN_4  0x00000004
#define SWITCH_PIN_5  0x00000005
#define SWITCH_PIN_6  0x00000006
#define SWITCH_PIN_7  0x00000007
#define SWITCH_PIN_8  0x00000008
#define SWITCH_PIN_9  0x00000009
#define SWITCH_PIN_10 0x0000000A
#define SWITCH_PIN_11 0x0000000B
#define SWITCH_PIN_12 0x0000000C
#define SWITCH_PIN_13 0x0000000D
#define SWITCH_PIN_14 0x0000000E
#define SWITCH_PIN_15 0x0000000F

// Definitions for switch connection types
#define SWITCH_CONNECT_REVERSE 0x00000000
#define SWITCH_CONNECT_FORWARD 0x00000001

// Structure to hold switch configuration
typedef struct {
    u32 Port; // Port of the switch
    u32 Pin;  // Pin of the switch
} SWITCH_Confg_t; // Define the structure type


// Enumeration for switch states
typedef enum {
    Switch_Pressed,  // Switch pressed state
    Switch_Released, // Switch released state
} Switch_State_t; // Define the enumeration type

/**
 * @brief Initialize switches
 * 
 * @return Error_t Error status indicating success or failure of initialization
 */
void SWITCH_Init(void);

/**
 * @brief Get the state of a specific switch
 * 
 * @param SWITCH_Name Name of the switch
 * @param SWITCH_State Pointer to store the state of the switch
 * @return Error_t Error status indicating success or failure of getting the state
 */
void SWITCH_Get_State(SWITCH_Names_t SWITCH_Name, u32 *SWITCH_State);

#endif /* SWITCH_H_ */ 
