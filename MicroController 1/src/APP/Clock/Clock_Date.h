/*
 ============================================================================
 Name        : CLOCK.h
 Author      : Omar Medhat Mohamed
 Description : Header File for the CLOCK Driver
 Date        : 13/4/2024
 ============================================================================
 */
#ifndef CLOCK_H_
#define CLOCK_H_
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include "LIB/std_types.h"
#include "LIB/Mask32.h"
#include "LIB/Error.h"
#include "Services/Scheduler/Scheduler.h"
#include "Services/Scheduler/Scheduler_cfg.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/*******************************************************************************
 *                        	  Types Declaration                                 *
 *******************************************************************************/
typedef struct 
{
	uint8_t MillisecondsIn100 ;
	uint8_t second ;
	uint8_t minute ;
	uint8_t hour ;
	uint8_t day ;
	uint8_t month ;
	uint16_t year ;
}TimeInfo_t;

/*******************************************************************************
 *                  	    Functions Prototypes                               *
 *******************************************************************************/
/**
 * @brief    : Calculates the current time based on the system tick count.
 * @details  : This function calculates the current time using the system tick count.
 *             - Retrieves the current tick count in milliseconds.
 *             - Calculates the time difference between the current and previous timestamps.
 *             - Updates the time structure with the added milliseconds, seconds, minutes, hours, and days.
 *             - Adjusts the days if they exceed the number of days in the current month, considering leap years.
 * @param[in]: None
 * @return   : const TimeInfo_t* Pointer to the structure containing the current time information.
 **/
const TimeInfo_t *Clock_CalculateCurrentTime(void) ;

/**
 * @brief    : Sets the seconds value in the time structure.
 * @param[in]: seconds: The value to set for seconds.
 * @return   : None
 **/
void Clock_SetSeconds(uint8_t seconds);
/**
 * @brief    : Sets the minutes value in the time structure.
 * @param[in]: minutes: The value to set for minutes.
 * @return   : None
 **/
void Clock_SetMinutes(uint8_t minutes);
/**
 * @brief    : Sets the hours value in the time structure.
 * @param[in]: hours: The value to set for hours.
 * @return   : None
 **/
void Clock_SetHours(uint8_t hours);

/**
 * @brief    : Sets the days value in the time structure.
 * @param[in]: days: The value to set for days.
 * @return   : None
 **/
void Clock_SetDays(uint8_t days);

/**
 * @brief    : Sets the months value in the time structure.
 * @param[in]: months: The value to set for months.
 * @return   : None
 **/
void Clock_SetMonths(uint8_t months);

/**
 * @brief    : Sets the years value in the time structure.
 * @param[in]: years: The value to set for years.
 * @return   : None
 **/
void Clock_SetYears(uint16_t years);

#endif