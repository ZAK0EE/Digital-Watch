/*
 ============================================================================
 Name        : CLOCK_TesT.c
 Author      : Omar Medhat Mohamed
 Description : Source File for the CLOCK Driver
 Date        : 14/4/2024
 ============================================================================
 */
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include "APP/Clock/Clock_Date.h"

void Clock_TEST_TASK (void)
{
    volatile const TimeInfo_t* Time ;
    Time =  Clock_MillisTicksToTime();
    char buff[] =  {Time->Loc_100milliseconds , Time->second ,Time->minute , Time->hour,Time->day , Time->month , Time->year};

}