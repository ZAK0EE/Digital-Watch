/**
 * @file Scheduler_cfg.h
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef SERVICES_SCHEDULER_SCHEDULER_CFG_H_
#define SERVICES_SCHEDULER_SCHEDULER_CFG_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "Scheduler.h"

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
 * @brief Enumeration of runnable task names.
 * 
 * This enumeration lists the names of all the runnable tasks that can be scheduled
 * by the scheduler. 
 * @note _NUM_OF_RUNNABLES is used for internal calculations and should not be modified,
 */
typedef enum
{
    SCHED_DISPLAY_TASK,
    SCHED_LCD_TASK,
    SCHED_SWITCH_TASK,
    SCHED_UPDATE_SWICH,
    SCHED_DigitalWatch_Task,

    _NUM_OF_RUNNABLES,    /**< Total number of runnables. Do not modify. */
} Sched_Runnable_Name_t;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/



#endif // SERVICES_SCHEDULER_SCHEDULER_CFG_H_