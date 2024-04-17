/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "../MCAL/SYSTICK/SYSTICK.h"
#include "Scheduler.h"
#include "Scheduler_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef struct{
    Sched_runnable_t const *runnable;
    u32 remainingTimeMS;            
}Sched_runnableInfo_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

extern const Sched_runnable_t runnables[_NUM_OF_RUNNABLES];
static Sched_runnableInfo_t rInfo[_NUM_OF_RUNNABLES];

u32 pendingTicks = 0;

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

static void TickCallBack(void);
static void Scheduler(void);

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

void TickCallBack(void)
{
    pendingTicks++;
}

void Scheduler(void)
{
    u16 iter = 0;

    for(iter = 0; iter < _NUM_OF_RUNNABLES; iter++)
    {
        if(rInfo[iter].runnable && rInfo[iter].runnable->callBack && rInfo[iter].remainingTimeMS == 0)
        {
            rInfo[iter].runnable->callBack();
            rInfo[iter].remainingTimeMS = rInfo[iter].runnable->periodicityMS;
        }
        
        rInfo[iter].remainingTimeMS -= TICK_TIME;
        
    }
}

void Sched_init(void)
{
    u16 iter = 0;
    SYSTICK_SetTime_ms(TICK_TIME);
    SYSTICK_SetCallBack(TickCallBack, 0);

    for(iter = 0; iter < _NUM_OF_RUNNABLES; iter++)
    {
        if(runnables[iter].callBack)
        {
            rInfo[iter].runnable = &runnables[iter];
            rInfo[iter].remainingTimeMS = runnables[iter].firstDelayMS;
        }
    }
}



void Sched_start(void)
{
    SYSTICK_StartCounting(SYSTICK_RUN_PERIODIC);

    while(1)
    {
        if(pendingTicks)
        {
            pendingTicks--;
            Scheduler();
        }
    }
}