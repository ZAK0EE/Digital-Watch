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
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/*******************************************************************************
 *                        	  Types Declaration                                 *
 *******************************************************************************/
typedef struct 
{
	uint8_t Loc_100milliseconds ;
	uint8_t second ;
	uint8_t minute ;
	uint8_t hour ;
	uint8_t day ;
	uint8_t month ;
	uint16_t year ;
}TineInfo_t;
#define SCHED_TICK_TIMEMS 1

/*******************************************************************************
 *                  	    Functions Prototypes                               *
 *******************************************************************************/
/**
 * @brief    : Set Clock ON.
 * @param[in]: Copy_Clock The clock source to be set on. It can be CLOCK_HSI, CLOCK_HSE, or CLOCK_PLL.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of setting the clock on.
 * @details  : This function turns on the specified clock source.
			   It enables the clock according to the provided clock source.
 */
const TineInfo_t* Clock_MillisTicksToTime(void);

#endif