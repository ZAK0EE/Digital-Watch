/**
 * @file StopWatch.c
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */
/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "StopWatch.h"
#include "Services/Scheduler/Scheduler.h"
#include "Services/Scheduler/Scheduler_cfg.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
uint64_t startTickCount = 0;
uint64_t pauseTickCount = 0;
uint64_t delayTickCount = 0;

StopWatch_time_t currentTime = {0};
uint8_t isStopWatchRunning = 0;
/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/


void StopWatch_start(void)
{
    StopWatch_reset();
    isStopWatchRunning = 1;
    startTickCount = Sched_getTickCount();
}


void StopWatch_pause(void)
{
    pauseTickCount = Sched_getTickCount();
    isStopWatchRunning = 0;
}

void StopWatch_resume(void)
{
    if(isStopWatchRunning == 0)
    {
        delayTickCount += Sched_getTickCount() - pauseTickCount;
        isStopWatchRunning = 1;
    }
}

const StopWatch_time_t *StopWatch_getCurrentTime(void)
{
    if(isStopWatchRunning)
    {
        uint64_t currentTickCount = Sched_getTickCount();
        uint64_t timeDifferenceMS = (currentTickCount - startTickCount - delayTickCount) * SCHED_TICK_TIMEMS;

        /* Updating current time */
        currentTime.secondsMS   = (timeDifferenceMS) % 1000;
        currentTime.seconds     = (timeDifferenceMS / 1000) % 60;
        currentTime.minutes     = (timeDifferenceMS / 1000 / 60) % 60;
        currentTime.hours       = (timeDifferenceMS / 1000 / 60 / 60) % 24;

        //startTickCount = currentTickCount;
    }

    return &currentTime;

}


void StopWatch_reset(void)
{
    currentTime.secondsMS   = 0;
    currentTime.seconds     = 0;
    currentTime.minutes     = 0;
    currentTime.hours       = 0; 

    startTickCount = 0;
    pauseTickCount = 0;
    delayTickCount = 0;
    isStopWatchRunning = 0;
}