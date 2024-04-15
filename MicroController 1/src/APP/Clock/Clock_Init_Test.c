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
#define Clock_DISPLAY_TEST
#ifdef Clock_TEST
#include "APP/Clock/Clock_Date.h"

void Clock_TEST_TASK (void)
{
    volatile const TimeInfo_t* Time ;
    Time =  Clock_MillisTicksToTime();
    char buff[] =  {Time->Loc_100milliseconds , Time->second ,Time->minute , Time->hour,Time->day , Time->month , Time->year};

}
#endif
#ifdef Clock_DISPLAY_TEST
#include "APP/Clock/Clock_Date.h"
#include "APP/Display/Display.h"
#include "HAL/LCD/LCD.h"
#include "APP/INIT/INIT.h"
#include "Services/Scheduler/Scheduler.h"
void assert_failed(void)
{
    while(1);
}
#include <stdio.h>
// Periodic task call:100ms Delay:20ms
void display_testa(void)
{
    Display_setCursorAsync(0 ,0);
     volatile const TimeInfo_t* Time ;
     Time =  Clock_CalculateCurrentTime();
    char buff[30]={'0', '0'}; 
    int len = sprintf(buff, "%.2d:%.2d:%.2d:%d",(int)Time->hour , (int)Time->minute, (int)Time->second,(int)Time->secondMS);
    Display_printCenteredAsync(buff, len);
     Display_setCursorAsync(1 ,0);
    len = sprintf(buff, "%.4d:%.2d:%.2d",(int)Time->year , (int)Time->month, (int)Time->day);
    Display_printCenteredAsync(buff, len);

}
int main(void)
{
    InitSys();
    Clock_SetYears(2024);
    Clock_SetMonths(4);
    Clock_SetDays(15);
    Clock_SetHours(5);
    Clock_SetMinutes(39);
    Sched_init();
    Sched_start();
    
}

#endif