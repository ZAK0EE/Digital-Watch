/**
 * @file StopWatch.h
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef APP_STOPWATCH_STOPWATCH_H_
#define APP_STOPWATCH_STOPWATCH_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include <stdint.h>

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/**
 * @brief Structure representing the time components of a stopwatch.
 */
typedef struct
{
    uint32_t hours;     /**< Hours component of the stopwatch time. */
    uint32_t minutes;   /**< Minutes component of the stopwatch time. */
    uint32_t seconds;   /**< Seconds component of the stopwatch time. */
    uint32_t secondsMS; /**< Milliseconds component of the stopwatch time. */
} StopWatch_time_t;



/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/**
 * @brief Starts the stopwatch.
 */
void StopWatch_start(void);

/**
 * @brief Pauses the stopwatch.
 */
void StopWatch_pause(void);

/**
 * @brief Resumes the stopwatch after it was paused
 */
void StopWatch_resume(void);

/**
 * @brief Retrieves the current time of the stopwatch.
 * 
 * @return StopWatch_time_t The current time of the stopwatch.
 */
const StopWatch_time_t *StopWatch_getCurrentTime(void);

/**
 * @brief Resets the stopwatch, all stopwatch components are reset to zero
 */
void StopWatch_reset(void);




























#endif // APP_STOPWATCH_STOPWATCH_H_