/*
 ============================================================================
 Name        : CLOCK.c
 Author      : Omar Medhat Mohamed
 Description : Source File for the CLOCK Driver
 Date        : 13/4/2024
 ============================================================================
 */
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include "APP/Clock/Clock_Date.h"
/*******************************************************************************
 *                             Definitions                                      *
 *******************************************************************************/

/*******************************************************************************
 *                        	  Types Declaration                                 *
 *******************************************************************************/


/*******************************************************************************
 *                              Variables		                                *
 *******************************************************************************/


/*******************************************************************************
 *                             Implementation   				                *
 *******************************************************************************/
/**
 * @brief    : Set Clock ON.
 * @param[in]: Copy_Clock The clock source to be set on. It can be CLOCK_HSI, CLOCK_HSE, or CLOCK_PLL.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of setting the clock on.
 * @details  : This function turns on the specified clock source.
               It enables the clock according to the provided clock source.
 */
const TineInfo_t* Clock_MillisTicksToTime(void)
{
    /* Local Variable to store error status */
    Error_enumStatus_t Loc_enumReturnStatus = Status_enumOk;
    uint64_t Loc_milliseconds = Sched_getTickCount();
    Loc_milliseconds *= SCHED_TICK_TIMEMS ; 
    static TineInfo_t Time ;
    /**The time is given to us by milliseconds we just need to know the third digit in milliseconds
     * ex: Loc_milliseconds = 45189300 so Time.Loc_100milliseconds should equal 3
    */
    Time.Loc_100milliseconds = (Loc_milliseconds&1000) /100 ;
    /**Identify  he number of seconds right now since the start of timestamp
     * ex: Loc_milliseconds = 45189300 so Loc_seconds_since_epoch should equal 45189 seconds
    */
    uint64_t Loc_seconds_since_epoch = Loc_milliseconds / 1000 ;
    /** Get the current seconds as if Loc_seconds_since_epoch is 45189 
     *  So now is the second number should be 9  in the specific a minute that i will calculate it in the next step  */
    Time.second= Loc_seconds_since_epoch % 60 ;
    /**Identify  the number of minutes right now since the start of timestamp
     * ex : Loc_seconds_since_epoch = 45189 so Loc_minutes_since_epoch should equal 753 minutes
    */
    uint64_t Loc_minutes_since_epoch = Loc_seconds_since_epoch / 60 ; 
    /** Get the current minutes  as if Loc_minutes_since_epoch is 753 
     *  So now is the minute number should be 33 in the specific a hour that  i will calculate it in the next step 
    */
   



 
    /* Return the status of the clock operation */
    return &Time;
}

//  = {
//         .millisecond = 0 ,
//         .second = 0 ,
//         .minute = 0 ,
//         .hour = 0 ,
//         .day = 1 ,
//         .month = 1 ,
//         .year = 2000 ,
//     } ;