/**
 * @file Scheduler.h
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef SERVICES_SCHEDULER_SCHEDULER_H_
#define SERVICES_SCHEDULER_SCHEDULER_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include <stdint.h>

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
/**
 * @brief Callback function type for scheduler runnables.
 * 
 * This typedef defines the type for a callback function that can be registered
 * as a task with the scheduler.
 */
typedef void (*Sched_Runnable_Callback_t)(void);


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/**
 * @brief Configuration structure for scheduler runnables.
 * 
 * This structure defines the configuration for a task that can be scheduled
 * by the scheduler.
 */
typedef struct	
{
	uint32_t DelayMS;                       /**< Time until the task is first executed. */
	uint32_t PeriodicityMS;                 /**< Periodicity of the task in milliseconds. */
	Sched_Runnable_Callback_t CallBack;     /**< Callback function to be executed as the task. */
} Sched_Runnable_Config_t;


/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/


/**
 * @brief Initialize the scheduler.
 * 
 * This function initializes the scheduler by reading tasks from Scheduler_cfg.c.
 * It must be called before starting the scheduler using Sched_start().
 */
void Sched_init(void);

/**
 * @brief Start the scheduler.
 * 
 * This function starts the scheduler, allowing scheduled tasks to be executed.
 * Before calling this function, ensure that the scheduler has been initialized
 * using Sched_init().
 */
void Sched_start(void);



























#endif // SERVICES_SCHEDULER_SCHEDULER_H_