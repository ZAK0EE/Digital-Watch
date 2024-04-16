/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "Scheduler_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

extern void _SWITCH_getState_5MS_runnable(void);
extern void GetSwitchState_50MS_runnable(void);
extern void ToggleRedLED_250MS_runnable(void);
extern void ToggleGreenLED_500MS_runnable(void);
extern void ToggleYellowLED_1000MS_runnable(void);
extern void TrafficLights_1000MS_runnable(void);

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

const Sched_runnable_t runnables[_NUM_OF_RUNNABLES] = {
    [SWITCH_getState_5MS] = {   .periodicityMS  = 5,
                                .firstDelayMS   = 0,
                                .callBack       = _SWITCH_getState_5MS_runnable
    },
    [GetSwitchState_50MS] = {   .periodicityMS  = 50,
                                .firstDelayMS   = 0,
                                .callBack       = GetSwitchState_50MS_runnable
    },
    [TrafficLights_1000MS] = {  .periodicityMS  = 1000,
                                .firstDelayMS   = 0,
                                .callBack       = TrafficLights_1000MS_runnable
    },

    /*
    [ToggleRedLED_250MS] = {    .periodicityMS  = 250,
                                .firstDelayMS   = 0,
                                .callBack       = ToggleRedLED_250MS_runnable
    },

    [ToggleGreenLED_500MS] = {  .periodicityMS  = 500,
                                .firstDelayMS   = 0,
                                .callBack       = ToggleGreenLED_500MS_runnable
    },

    [ToggleYellowLED_1000MS] = {.periodicityMS  = 1000,
                                .firstDelayMS   = 0,
                                .callBack       = ToggleYellowLED_1000MS_runnable
    }*/
};






