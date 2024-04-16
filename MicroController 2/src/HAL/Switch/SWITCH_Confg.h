#ifndef SWITCH_CONFG_H_ // Header guard to prevent multiple inclusion of the header file
#define SWITCH_CONFG_H_ // Define the header guard macro

#include "../../lib/StdTypes.h" // Include standard types header file
#include "../../lib/Error/Error.h" // Include error handling header file

// Enumeration for switch names
typedef enum {
    INC_Switch,
    Mode_Switch,
    Edit_Switch,
    SWITCH_Number,  // Define the total number of switches
} SWITCH_Names_t;   // Define the enumeration type for switch names

#endif // End of header guard and file
