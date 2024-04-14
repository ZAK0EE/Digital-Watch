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
 *                             Implementation   				                *
 *******************************************************************************/
/**
 * @brief    : Set Clock ON.
 * @param[in]: Copy_Clock The clock source to be set on. It can be CLOCK_HSI, CLOCK_HSE, or CLOCK_PLL.
 * @return   : Error_enumStatus_t Error status indicating the success or failure of setting the clock on.
 * @details  : This function turns on the specified clock source.
               It enables the clock according to the provided clock source.
 */
 const TineInfo_t *Clock_MillisTicksToTime(void)
{
    uint64_t Loc_milliseconds = Sched_getTickCount();
    Loc_milliseconds *= SCHED_TICK_TIMEMS;
    static uint64_t Loc_PrevTimeStamp = 0;
    uint64_t Loc_TimeDiff = Loc_milliseconds - Loc_PrevTimeStamp;

    static TineInfo_t Time = {
        .Loc_100milliseconds = 0,
        .second = 0,
        .minute = 0,
        .hour = 0,
        .day = 1,
        .month = 1,
        .year = 2000,
    };
    /**Identify the number of milliseconds which will be added to the previous value
     s
     */
    uint64_t Loc_milliseconds_since_epoch = Time.Loc_100milliseconds + (Loc_TimeDiff) / 100;
    /**The time is given to us by milliseconds we just need to know the third digit in milliseconds
     * ex: Loc_milliseconds = 45189300 so Time.Loc_100milliseconds should equal 3
     */
    Time.Loc_100milliseconds = (Time.Loc_100milliseconds + ((Loc_TimeDiff) / 100)) % 10;

    /**Identify the number of seconds which will be added to the previous value
     * ex: Loc_milliseconds = 45189300 so Loc_seconds_since_epoch should equal 45189 seconds
     */
    uint64_t Loc_seconds_since_epoch = Time.second + (Loc_milliseconds_since_epoch / 10);
    /** Get the current seconds as if Loc_seconds_since_epoch is 45189
     *  So now is the second number should be 9  in a specific minute that i will calculate it in the next step  */
    Time.second = (Time.second + (Loc_milliseconds_since_epoch / 10)) % 60;
    /**Identify  the number of minutes which will be added to the previous value
     * ex : Loc_seconds_since_epoch = 45189 so Loc_minutes_since_epoch should equal 753 minutes
     */
    uint64_t Loc_minutes_since_epoch = Time.minute + (Loc_seconds_since_epoch / 60);
    /** Get the current minutes  as if Loc_minutes_since_epoch is 753
     *  So now is the minute number should be 33 in a specific  hour that  i will calculate it in the next step
     */
    Time.minute = (Time.minute + Loc_seconds_since_epoch / 60) % 60;
    /**Identify  the number of hours which will be added to the previous value
     * ex : Loc_minutes_since_epoch = 753 so Loc_hours_since_epoch should equal 12 minutes
     */
    uint32_t Loc_hours_since_epoch = Time.hour + (Loc_minutes_since_epoch / 60);
    /** Get the current minutes  as if Loc_hours_since_epoch is 12
     *  So now is the hours number should be 12 in a specific day that i will calculate it in the next step
     */
    Time.hour = (Time.hour + (Loc_minutes_since_epoch / 60)) % 24;
    /**Identify  the number of Days  which will be added to the previous value
     * ex : Loc_hours_since_epoch = 12 so Loc_days_since_epoch should equal 0 days
     */
    uint32_t Loc_days_since_epoch = Loc_hours_since_epoch / 24;
    Time.day += Loc_days_since_epoch;
    /*Define array to hold the number of days in each month*/
    static const uint8_t days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    while (Time.day > days_in_month[Time.month - 1])
    {
        /* Check if the current month is February */
        if (Time.month == 2 && ((Time.year % 4 == 0 && Time.year % 100 != 0) || Time.year % 400 == 0))
        {
            if (Time.day > 29)
            {
                Time.day -= 29; // Adjust for 29 days in February
                Time.month++;
            }
            else
            {
                break; // No need to adjust if it's still within February in a leap year
            }
        }
        else
        {
            Time.day -= days_in_month[Time.month - 1];
            Time.month++;
        }
        if (Time.month > 12)
        {
            Time.month = 1;
            Time.year++;
        }
    }
    Loc_PrevTimeStamp = Loc_milliseconds;
    /* Return the status of the clock operation */
    return &Time;
}
