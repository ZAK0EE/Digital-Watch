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
#include "Services/Scheduler/Scheduler.h"
#include "Services/Scheduler/Scheduler_cfg.h"
#include "APP/Clock/Clock_Date.h"
/*******************************************************************************
 *                        	  Types Declaration                                 *
 *******************************************************************************/
static TimeInfo_t Time = {
    .secondMS = 0,
    .second = 0,
    .minute = 0,
    .hour = 0,
    .day = 1,
    .month = 1,
    .year = 2000,
};
/*******************************************************************************
 *                        	     Variables                                      *
 *******************************************************************************/
/* Define array to hold the number of days in each month */
static const uint8_t days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
/*******************************************************************************
 *                             Implementation   				                *
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
const TimeInfo_t *Clock_CalculateCurrentTime(void)
{
    /* Retrieve the current tick count in milliseconds */
    uint64_t Loc_milliseconds = Sched_getTickCount();
    Loc_milliseconds *= SCHED_TICK_TIMEMS;
    /* Static variable to store the previous timestamp */
    static uint64_t Loc_PrevTimeStamp = 0;
    /* Calculate the time difference between the current and previous timestamps */
    uint64_t Loc_TimeDiff = Loc_milliseconds - Loc_PrevTimeStamp;

    /* Calculate the added milliseconds */
    uint64_t Loc_AddedMilliseconds = Time.secondMS + (Loc_TimeDiff) / 100;
    Time.secondMS = ( Time.secondMS + (Loc_TimeDiff) / 100) % 10;
    /* Calculate the added seconds */
    uint64_t Loc_AddedSeconds = Time.second + (Loc_AddedMilliseconds / 10);
    Time.second = (Time.second + (Loc_AddedMilliseconds / 10)) % 60;

    /* Calculate the added minutes */
    uint64_t Loc_AddedMinutes = Time.minute + (Loc_AddedSeconds / 60);
    Time.minute = (Time.minute + (Loc_AddedSeconds / 60)) % 60;

    /* Calculate the added hours */
    uint32_t Loc_AddedHours = Time.hour + (Loc_AddedMinutes / 60);
    Time.hour = (Time.hour + (Loc_AddedMinutes / 60)) % 24;

    /* Calculate the added days */
    uint32_t Loc_AddDays = Loc_AddedHours / 24;
    Time.day += Loc_AddDays;

    while (Time.day > days_in_month[Time.month - 1])
    {
        /* Check if the current month is February */
        if (Time.month == 2 && ((Time.year % 4 == 0 && Time.year % 100 != 0) || Time.year % 400 == 0))
        {
            if (Time.day > 29)
            {
                /* Adjust for 29 days in February */
                Time.day -= 29;
                Time.month++;
            }
            else
            {
                /* No need to adjust if it's still within February in a leap year */
                break;
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
    /* Update the previous timestamp */
    Loc_PrevTimeStamp = Loc_milliseconds;
    /* Return the pointer to the structure containing the current time information */
    return &Time;
}

/**
 * @brief    : Sets the seconds value in the time structure.
 * @param[in]: seconds: The value to set for seconds from 0 to 59.
 * @return   : None
 **/
void Clock_SetSeconds(uint8_t seconds)
{
    if (seconds > 59)
    {
        Time.second = 0;
    }
    else
    {
        Time.second = seconds;
    }
}

/**
 * @brief    : Sets the minutes value in the time structure.
 * @param[in]: minutes: The value to set for minutes from 0 to 59.
 * @return   : None
 **/
void Clock_SetMinutes(uint8_t minutes)
{
    if (minutes > 59)
    {
        Time.minute = 0;
    }
    else
    {
        Time.minute = minutes;
    }
}

/**
 * @brief    : Sets the hours value in the time structure.
 * @param[in]: hours: The value to set for hours from 0 to 23.
 * @return   : None
 **/
void Clock_SetHours(uint8_t hours)
{
    if (hours > 23)
    {
        Time.hour = 0;
    }
    else
    {
        Time.hour = hours;
    }
}

/**
 * @brief    : Sets the days value in the time structure.
 * @param[in]: days: The value to set for days from to 31 , 30 , 29 , 28 according to months number .
 * @return   : None
 * @details : This function checks if the days exceed the maximum days in the current month and adjusts them accordingly.
 **/
void Clock_SetDays(uint8_t days)
{
    uint8_t Current_Day = days_in_month[Time.month - 1];
    if (Time.month == 2 && ((Time.year % 4 == 0 && Time.year % 100 != 0) || Time.year % 400 == 0))
    {
        Current_Day = 29;
    }
    if ( (days > Current_Day) ||  (days == 0) )
    {
        Time.day = 1;
    }
    else
    {
        Time.day = days;
    }
}

/**
 * @brief    : Sets the months value in the time structure.
 * @param[in]: months: The value to set for months from 1 to 12.
 * @return   : None
 **/
void Clock_SetMonths(uint8_t months)
{
    if (months > 12 || (months == 0))
    {
        Time.month = 1;
    }
    else
    {
         // Check if the current day exceeds the maximum days in the new month
        uint8_t max_days_in_new_month = days_in_month[months - 1];
        if (Time.month == 2 && ((Time.year % 4 == 0 && Time.year % 100 != 0) || Time.year % 400 == 0))
        {
            max_days_in_new_month = 29 ;
        }
        if (Time.day > max_days_in_new_month)
        {
            // Reset the day to 1 if it exceeds the maximum days in the new month
            Time.day = 1;
        }
        Time.month = months;
    }
}

/**
 * @brief    : Sets the years value in the time structure.
 * @param[in]: years: The value to set for years from 2000 to 9999.
 * @return   : None
 **/
void Clock_SetYears(uint16_t years)
{
    if ((years > 9999 ) || (years < 2000 ) || (years == 0))
    {
        Time.year = 2000;
    }
    else
    {
        Time.year = years;
    }
}