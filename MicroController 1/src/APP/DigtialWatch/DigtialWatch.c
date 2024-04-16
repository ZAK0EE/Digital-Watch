/*
 ============================================================================
 Name        : DigtialWatch.c
 Author      : Omar Medhat Mohamed
 Description : Source File for the DigtialWatch Driver
 Date        : 15/4/2024
 ============================================================================
 */
/*******************************************************************************
 *                                Includes	                                  *
 *******************************************************************************/
#include "HAL/HSwitch/HSwitch.h"
#include "APP/Clock/Clock_Date.h"
#include "APP/StopWatch/StopWatch.h"
#include "APP/Display/Display.h"
#include <stdio.h>
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define DW_ENTER_EDIT       0
#define DW_HR_SEC_DIG       1
#define DW_HR_FST_DIG       2
#define DW_MIN_SEC_DIG      3
#define DW_MIN_FST_DIG      4
#define DW_SEC_SEC_DIG      5
#define DW_SEC_FST_DIG      6
#define DW_YEAR_FTH_DIG     7
#define DW_YEAR_THR_DIG     8
#define DW_YEAR_SEC_DIG     9
#define DW_YEAR_FST_DIG     10
#define DW_MONTH_SEC_DIG    11
#define DW_MONTH_FST_DIG    12
#define DW_DAY_SEC_DIG      13
#define DW_DAY_FST_DIG      14


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
typedef enum
{
    DW_Mode_StopWatch_Show,
    DW_Mode_StopWatch_Operation,
    DW_Mode_StopWatch_Start,
    DW_Mode_StopWatch_Pause,
    DW_Mode_StopWatch_Resume,
    DW_Mode_StopWatch_Reset
} DW_StopWatchModes;
/*******************************************************************************
 *                        	     Variables                                      *
 *******************************************************************************/
static DW_Mode Current_Operation_Mode = DW_Mode_Clock;
static DW_ClockMode Current_Clock_Mode = DW_Mode_Clock_Show;
static DW_StopWatchModes Current_StopWatch_Mode = DW_Mode_StopWatch_Show;
uint8_t Edit_switch_status = 0;
uint8_t Inc_switch_status = 0;
uint8_t Mode_switch_status = 0;
/*******************************************************************************
 *                         Static Function Prototypes		                   *
 *******************************************************************************/
static void DW_Clock_Modes(void);
static void DW_StopWatch_Modes(void);
/*******************************************************************************
 *                             Implementation   				                *
 *******************************************************************************/
/**
 * @brief    : Updates the switch statuses.
 * @details  : This function updates the statuses of the switches used in the system,
 *             including the increment switch, mode switch, and edit switch.
 * @param    : None
 * @return   : None
 **/
void Update_Switch_Runnable(void)
{
    /* Get status of the increment switch */
    HSwitch_Get_Status(Increament, &Inc_switch_status);
    
    /* Get status of the mode switch */
    HSwitch_Get_Status(Mode, &Mode_switch_status);
    
    /* Get status of the edit switch */
    HSwitch_Get_Status(Edit, &Edit_switch_status);
}

/**
 * @brief    : Main runnable function for the Date and Time display.
 * @details  : This function manages the display modes for both the clock and stopwatch.
 *             It switches between clock and stopwatch modes based on the current operation mode.
 *             If the mode switch is pressed, it switches between clock and stopwatch modes.
 * @param    : None
 * @return   : None
 **/
void DW_Runnable(void)
{
    switch (Current_Operation_Mode)
    {
    case DW_Mode_Clock:
        /* Execute clock display modes */
        DW_Clock_Modes();
        
        /* Switch to stopwatch mode if the mode switch is pressed */
        if ((Current_Clock_Mode != DW_Mode_Clock_Edit) && (Mode_switch_status == HSwitch_PRESSED))
        {
            Current_Operation_Mode = DW_Mode_StopWatch;
            Display_clearScreenAsync();
        }
        break;
        
    case DW_Mode_StopWatch:
        /* Execute stopwatch display modes */
        DW_StopWatch_Modes();
        
        /* Switch to clock mode if the mode switch is pressed */
        if ((Mode_switch_status == HSwitch_PRESSED))
        {
            Current_Operation_Mode = DW_Mode_Clock;
            Display_clearScreenAsync();
        }
        break;
        
    default:
        break;
    }
}

/**
 * @brief    : Manages the display modes for the clock.
 * @details  : This function handles the different display modes for the clock, such as showing the current time
 *             and allowing editing of the time components.
 * @param    : None
 * @return   : None
 **/
static void DW_Clock_Modes(void)
{
    /* Pointer to hold the current time information */
    const TimeInfo_t *Time;
    Time = Clock_CalculateCurrentTime();
    
    /* Buffer to store formatted time strings */
    char buff[30] = {'0', '0'};
    int len;
    uint16_t Inc_Amount = 0;
    
    /* Static variable to keep track of the current position in edit mode */
    static uint8_t current_posstion = DW_ENTER_EDIT;
    
    /* Set the cursor to the beginning of the display */
    Display_setCursorAsync(0, 0);
    
    switch (Current_Clock_Mode)
    {
    case DW_Mode_Clock_Show:
        /* Display the current time */
        len = sprintf(buff, "%.2d:%.2d:%.2d:%d", (int)Time->hour, (int)Time->minute, (int)Time->second, (int)Time->secondMS);
        Display_printCenteredAsync(buff, len);
        Display_setCursorAsync(1, 0);
        len = sprintf(buff, "%.4d/%.2d/%.2d", (int)Time->year, (int)Time->month, (int)Time->day);
        Display_printCenteredAsync(buff, len);
        
        /* Switch to edit mode if the edit switch is pressed */
        if (Edit_switch_status == HSwitch_PRESSED)
        {
            Current_Clock_Mode = DW_Mode_Clock_Edit;
        }
        break;
        
    case DW_Mode_Clock_Edit:
        /* Display the current time in edit mode */
        len = sprintf(buff, "%.2d:%.2d:%.2d:%d", (int)Time->hour, (int)Time->minute, (int)Time->second, (int)Time->secondMS);
        Display_printCenteredAsync(buff, len);
        Display_setCursorAsync(1, 0);
        len = sprintf(buff, "%.4d/%.2d/%.2d", (int)Time->year, (int)Time->month, (int)Time->day);
        Display_printCenteredAsync(buff, len);
        Display_setCursorAsync(0, current_posstion);
        
        /* Handle increment switch press to edit time components */
        if (Inc_switch_status == HSwitch_PRESSED)
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
                break;
            default:
                current_posstion = DW_HR_SEC_DIG;
                break;
            }
        }
        
        /* Switch back to show mode if mode switch is pressed */
        if (Mode_switch_status == HSwitch_PRESSED)
        {
            Current_Clock_Mode = DW_Mode_Clock_Show;
        }
        
        /* Move to the next digit in edit mode if edit switch is pressed */
        if (Edit_switch_status == HSwitch_PRESSED)
        {
            Current_Clock_Mode = DW_Mode_Clock_Edit_Next_Dig;
        }
        break;
        
    case DW_Mode_Clock_Edit_Next_Dig:
        /* Move to the next digit in edit mode */
        current_posstion++;
        Current_Clock_Mode = DW_Mode_Clock_Edit;
        break;
        
    default:
        break;
    }
}

/**
 * @brief    : Manages the display modes for the stopwatch.
 * @details  : This function handles the different display modes for the stopwatch, such as showing the current time
 *             and allowing control over stopwatch operations (start, pause, resume, reset).
 * @param    : None
 * @return   : None
 **/
static void DW_StopWatch_Modes(void)
{
    /* Static variable to keep track of the current stopwatch mode */
    static DW_StopWatchModes StopWatch_Start_Stop = DW_Mode_StopWatch_Operation;
    
    /* Buffer to store formatted stopwatch time string */
    char buff[30] = {0};
    
    /* Pointer to hold the current stopwatch time information */
    const StopWatch_time_t *stopWatchTime = StopWatch_getCurrentTime();
    
    /* Format the stopwatch time string */
    int len = sprintf(buff, "%.2d:%.2d:%.2d:%.2d", (int)(stopWatchTime->hours), (int)(stopWatchTime->minutes), (int)(stopWatchTime->seconds), (int)(stopWatchTime->secondsMS / 10));
    buff[len] = 0;

    switch (Current_StopWatch_Mode)
    {
    case DW_Mode_StopWatch_Show:
        /* Display the stopwatch time */
        Display_printCenteredAsync(buff, len);
        
        /* Switch to operation mode if edit switch is pressed */
        if (Edit_switch_status == HSwitch_PRESSED)
        {
            Current_StopWatch_Mode = DW_Mode_StopWatch_Operation;
            StopWatch_Start_Stop++;
        }
        
        /* Reset stopwatch if increment switch is pressed */
        if (Inc_switch_status == HSwitch_PRESSED)
        {
            Current_StopWatch_Mode = DW_Mode_StopWatch_Reset;
            StopWatch_Start_Stop = DW_Mode_StopWatch_Operation;
        }
        break;
        
    case DW_Mode_StopWatch_Operation:
        /* Handle stopwatch operation based on current state */
        switch (StopWatch_Start_Stop)
        {
        case DW_Mode_StopWatch_Start:
            StopWatch_start();
            Current_StopWatch_Mode = DW_Mode_StopWatch_Show;
            break;
        case DW_Mode_StopWatch_Pause:
            StopWatch_pause();
            Current_StopWatch_Mode = DW_Mode_StopWatch_Show;
            break;
        case DW_Mode_StopWatch_Resume:
            StopWatch_resume();
            Current_StopWatch_Mode = DW_Mode_StopWatch_Show;
            StopWatch_Start_Stop = DW_Mode_StopWatch_Start;
            break;
        default:
            break;
        }
        break;
        
    case DW_Mode_StopWatch_Reset:
        /* Reset the stopwatch and switch back to show mode */
        StopWatch_reset();
        Current_StopWatch_Mode = DW_Mode_StopWatch_Show;
        break;
        
    default:
        break;
    }
}
 
