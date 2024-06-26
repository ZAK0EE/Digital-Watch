#ifndef STOP_WATCH_H_
#define STOP_WATCH_H_
/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "../../lib/StdTypes.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/


// Enum for the state of the Stop Watch
typedef enum
{
    Stopped,
    Started
} StopWatchState;

// Structure definition for Stop Watch Time
typedef struct
{
    // H1H2 : M1M2 : S1S2 : Ms1Ms2
    u8 H1;
    u8 H2;
    u8 M1;
    u8 M2;
    u8 S1;
    u8 S2;
    u8 Ms1;
    u8 Ms2;
} StopWatchTime_t;


/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

void controlSW(u8 frame);

StopWatchTime_t getSW(void);

#endif