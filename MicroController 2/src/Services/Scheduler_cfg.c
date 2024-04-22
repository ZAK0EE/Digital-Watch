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

extern void HSwitch(void);
extern void Read_Switch(void);
extern void ModeDisplay(void);
extern void updateStopWatchTime(void);
extern void runnable_updateDateAndTime(void);
extern void LCD_Task(void);

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

const Sched_runnable_t runnables[_NUM_OF_RUNNABLES] =
{
    [Update_DateAndTime_Run] = 
    {
        .periodicityMS = 1000,
        .firstDelayMS  = 0,
        .callBack = runnable_updateDateAndTime,
    },
    [Update_SW_Run] = 
    {
        .periodicityMS = 50,
        .firstDelayMS  = 7,
        .callBack = updateStopWatchTime,
    },
    [HSwtich_Run] =
    {
        .periodicityMS  = 5,
        .firstDelayMS   = 0,
        .callBack       = HSwitch,
    },

    [Read_Switch_Run] = 
    {
        .periodicityMS = 50,
        .firstDelayMS  = 3,
        .callBack      = Read_Switch,
    },
    
    [LCD_Run] = 
    {
        .periodicityMS = 1,
        .firstDelayMS  = 0,
        .callBack = LCD_Task,
    },
    [Mode_Dislay_Run] =
    {
        .periodicityMS = 10,
        .firstDelayMS = 1000,
        .callBack = ModeDisplay,
    },
};






