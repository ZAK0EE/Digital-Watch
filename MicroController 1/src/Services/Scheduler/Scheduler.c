/**
 * @file Scheduler.c
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "Scheduler.h"
#include "Scheduler_cfg.h"
#include "MCAL/SysTick/SysTick.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

/**
 * @brief Time interval for the scheduler tick in milliseconds.
 */
#define SCHED_TICK_TIMEMS 1



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/


/**
 * @brief Structure to hold information about a scheduled runnable task.
 */
typedef struct
{
    Sched_Runnable_Config_t *Runnable;  /**< Pointer to the configuration of the scheduled task */
    int64_t RemainingTimeMS;           /**< Remaining time until the task's next execution */
} RunnableInfo_t;





/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

/**
 * @brief Array containing the configurations of all registered runnables.
 * 
 * This array holds the configurations of all the runnables registered with the scheduler.
 */
extern Sched_Runnable_Config_t Sched_Runnables[_NUM_OF_RUNNABLES];

/**
 * @brief Variable to track the number of pending ticks.
 * 
 */
static uint32_t PendingTicks = 1;

/**
 * @brief Array to store runtime information for each runnable task.
 * 
 */
static RunnableInfo_t rinfo[_NUM_OF_RUNNABLES];



/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

/**
 * @brief Callback function invoked by the SysTick timer interrupt.
 * 
 * This function is registered as the callback function for the SysTick timer interrupt.
 * It increments the `PendingTicks` variable, indicating that a tick has occurred.
 */
static void TickCallBack(void)
{
	PendingTicks++;
}

/**
 * @brief Scheduler function responsible for task execution.
 * 
 * This function iterates through all the registered runnables and executes
 * the callback function associated with each runnable if the remaining time
 * for the task has elapsed.
 */
static void Scheduler(void)
{
	uint32_t idx;
	for(idx = 0; idx < _NUM_OF_RUNNABLES; idx++)
	{

		if(rinfo[idx].Runnable && rinfo[idx].Runnable->CallBack && rinfo[idx].RemainingTimeMS <= 0)
		{

			rinfo[idx].Runnable->CallBack();

			rinfo[idx].RemainingTimeMS = rinfo[idx].Runnable->PeriodicityMS;
		}

		rinfo[idx].RemainingTimeMS -= SCHED_TICK_TIMEMS;

	}    
}
/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/


void Sched_init(void)
{
    PendingTicks = 0;

    /* Initializing Systick */
    SysTick_Config_t config = 
    {
        .CallbackFunction = TickCallBack,
        .ClockSource = SYSTICK_CLK_AHB,
        .ExceptionState = SYSTICK_EXCEPTION_ENABLED,
    };
    SysTick_init(&config);

    /* Initializing runnables states */
    uint32_t idx;
	for(idx = 0; idx < _NUM_OF_RUNNABLES; idx++)
    {
        rinfo[idx].Runnable = &Sched_Runnables[idx];
		rinfo[idx].RemainingTimeMS = rinfo[idx].Runnable->DelayMS;
		      
    }
}


void Sched_start(void)
{
    SysTick_startTimerMS(SCHED_TICK_TIMEMS);
	while(1)
	{
		if(PendingTicks)
		{
			PendingTicks--;
			Scheduler();
		}
	}
}