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
#include "HAL/HSwitch/HSwitch.h"
#include "APP/Clock/Clock_Date.h"
#include "APP/Display/Display.h"
#include <stdio.h>
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define DW_HR_SEC_DIG 0
#define DW_HR_FST_DIG 1
#define DW_MIN_SEC_DIG 2
#define DW_MIN_FST_DIG 3
#define DW_SEC_FST_DIG 4
#define DW_SEC_SEC_DIG 5
#define DW_YEAR_FTH_DIG 6
#define DW_YEAR_THR_DIG 7
#define DW_YEAR_SEC_DIG 8
#define DW_YEAR_FST_DIG 9
#define DW_MONTH_SEC_DIG 10
#define DW_MONTH_FST_DIG 11
#define DW_DAY_SEC_DIG 12
#define DW_DAY_FST_DIG 13

/*******************************************************************************
 *                        	  Types Declaration                                 *
 *******************************************************************************/
typedef enum
{
    DW_Mode_Clock,
    DW_Mode_StopWatch
} DW_Mode;
typedef enum
{
    DW_Mode_Clock_Show,
    DW_Mode_Clock_Edit,
    DW_Mode_Clock_Edit_Next_Dig,
} DW_ClockMode;
/*******************************************************************************
 *                        	     Variables                                      *
 *******************************************************************************/
static DW_Mode Current_Operation_Mode = DW_Mode_Clock;
static DW_ClockMode Current_Clock_Mode = DW_Mode_Clock_Show;
uint8_t Edit_switch_statsus = 0;
uint8_t Inc_switch_statsus = 0;
uint8_t Mode_switch_statsus = 0;
/*******************************************************************************
 *                         Static Function Prototypes		                   *
 *******************************************************************************/
static void DW_Clock_Modes(void);
/*******************************************************************************
 *                             Implementation   				                *
 *******************************************************************************/
void Update_Switch_Runnable(void)
{
    HSwitch_Get_Status(Increament, &Inc_switch_statsus);
    HSwitch_Get_Status(Mode, &Mode_switch_statsus);
    HSwitch_Get_Status(Edit, &Edit_switch_statsus);
}
void DW_Runnable(void)
{

    switch (Current_Operation_Mode)
    {
    case DW_Mode_Clock:
        DW_Clock_Modes();
        if (Mode_switch_statsus == HSwitch_PRESSED)
        {
            Current_Operation_Mode = DW_Mode_StopWatch;
        }
        break;
    case DW_Mode_StopWatch:
        break;
    default:
        break;
    }
}
static void DW_Clock_Modes(void)
{
    Display_setCursorAsync(0, 0);
    const TimeInfo_t *Time;
    Time = Clock_CalculateCurrentTime();
    char buff[30] = {'0', '0'};
    int len;
    uint16_t Inc_Amount = 0;
    static uint8_t current_posstion = DW_HR_SEC_DIG;
    switch (Current_Clock_Mode)
    {
    case DW_Mode_Clock_Show:
        len = sprintf(buff, "%.2d:%.2d:%.2d:%d", (int)Time->hour, (int)Time->minute, (int)Time->second, (int)Time->secondMS);
        Display_printCenteredAsync(buff, len);
        Display_setCursorAsync(1, 0);
        len = sprintf(buff, "%.4d/%.2d/%.2d", (int)Time->year, (int)Time->month, (int)Time->day);
        Display_printCenteredAsync(buff, len);
        if (Edit_switch_statsus == HSwitch_PRESSED)
        {
            Current_Clock_Mode = DW_Mode_Clock_Edit;
        }
        break;
    case DW_Mode_Clock_Edit:
        len = sprintf(buff, "%.2d:%.2d:%.2d:%d", (int)Time->hour, (int)Time->minute, (int)Time->second, (int)Time->secondMS);
        Display_printCenteredAsync(buff, len);
        Display_setCursorAsync(1, 0);
        len = sprintf(buff, "%.4d/%.2d/%.2d", (int)Time->year, (int)Time->month, (int)Time->day);
        Display_printCenteredAsync(buff, len);
        Display_setCursorAsync(0, current_posstion);
        if (Inc_switch_statsus == HSwitch_PRESSED)
        {
            switch (current_posstion)
            {
            case DW_HR_SEC_DIG:
                Inc_Amount = 10;
                Clock_SetHours(Time->hour + (uint8_t)Inc_Amount);
                break;
            case DW_HR_FST_DIG:
                Inc_Amount = 1;
                Clock_SetHours(Time->hour + (uint8_t)Inc_Amount);
                break;
            case DW_MIN_SEC_DIG:
                Inc_Amount = 10;
                Clock_SetMinutes(Time->minute + (uint8_t)Inc_Amount);
                break;
            case DW_MIN_FST_DIG:
                Inc_Amount = 1;
                Clock_SetMinutes(Time->minute + (uint8_t)Inc_Amount);
                break;
            case DW_SEC_SEC_DIG:
                Inc_Amount = 10;
                Clock_SetSeconds(Time->second + (uint8_t)Inc_Amount);
                break;
            case DW_SEC_FST_DIG:
                Inc_Amount = 1;
                Clock_SetSeconds(Time->second + (uint8_t)Inc_Amount);
                break;
            case DW_YEAR_FTH_DIG:
                Inc_Amount = 1000;
                Clock_SetYears(Time->year + Inc_Amount);
                break;
            case DW_YEAR_THR_DIG:
                Inc_Amount = 100;
                Clock_SetYears(Time->year + (uint8_t)Inc_Amount);
                break;
            case DW_YEAR_SEC_DIG:
                Inc_Amount = 10;
                Clock_SetYears(Time->year + (uint8_t)Inc_Amount);
                break;
            case DW_YEAR_FST_DIG:
                Inc_Amount = 1;
                Clock_SetYears(Time->year + (uint8_t)Inc_Amount);
                break;
            case DW_MONTH_SEC_DIG:
                Inc_Amount = 10;
                Clock_SetMonths(Time->month + (uint8_t)Inc_Amount);
                break;
            case DW_MONTH_FST_DIG:
                Inc_Amount = 1;
                Clock_SetMonths(Time->month + (uint8_t)Inc_Amount);
                break;
            case DW_DAY_SEC_DIG:
                Inc_Amount = 10;
                Clock_SetDays(Time->day + (uint8_t)Inc_Amount);
                break;
            case DW_DAY_FST_DIG:
                Inc_Amount = 1;
                Clock_SetDays(Time->day + (uint8_t)Inc_Amount);
            default:
                break;
            }
        }
        if (Mode_switch_statsus == HSwitch_PRESSED)
        {
            Current_Clock_Mode = DW_Mode_Clock_Show;
        }
        if (Edit_switch_statsus == HSwitch_PRESSED)
        {
            Current_Clock_Mode = DW_Mode_Clock_Edit_Next_Dig;
        }
        break;
    case DW_Mode_Clock_Edit_Next_Dig:
        current_posstion++;
        Current_Clock_Mode = DW_Mode_Clock_Edit;
        break;
    default:
        break;
    }
}
