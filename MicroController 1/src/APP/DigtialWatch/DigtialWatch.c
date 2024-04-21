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
#include "APP/Button/Button.h"
#include <stdio.h>
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
static const uint8_t DW_days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
/*******************************************************************************
 *                        	  Types Declaration                                 *
 *******************************************************************************/
typedef enum
{
    DW_Mode_Clock,
    DW_Mode_StopWatch,
    DW_Mode_Reset
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
    DW_Mode_StopWatch_Start,
    DW_Mode_StopWatch_Pause,
    DW_Mode_StopWatch_Resume,
    DW_Mode_StopWatch_Reset
} DW_StopWatchModes;
typedef enum
{
    DW_HR_SEC_DIG,
    DW_HR_FST_DIG,
    DW_MIN_SEC_DIG,
    DW_MIN_FST_DIG,
    DW_SEC_SEC_DIG,
    DW_SEC_FST_DIG,
    DW_DAY_FST_DIG,
    DW_DAY_SEC_DIG,
    DW_MONTH_FST_DIG,
    DW_MONTH_SEC_DIG,
    DW_YEAR_FST_DIG,
    DW_YEAR_SEC_DIG,
    DW_YEAR_THR_DIG,
    DW_YEAR_FTH_DIG,
    _DW_DIGIT_NUM
} DW_EditMode;
/*******************************************************************************
 *                        	     Variables                                      *
 *******************************************************************************/
static const uint8_t DigitCol[_DW_DIGIT_NUM] = {3, 4, 6, 7, 9, 10, 3, 4, 6, 7, 9, 10, 11, 12};
static DW_Mode Current_Operation_Mode = DW_Mode_Clock;
static DW_ClockMode Current_Clock_Mode = DW_Mode_Clock_Show;
static DW_StopWatchModes Current_StopWatch_Mode = DW_Mode_StopWatch_Show;
static DW_StopWatchModes StopWatch_Start_Stop = DW_Mode_StopWatch_Show;
uint8_t Edit_switch_status = 0;
uint8_t Inc_switch_status = 0;
uint8_t Mode_switch_status = 0;
uint8_t Edit_held_status = 0;
uint8_t Mode_held_status = 0;
uint8_t Clock_Prev_State = 0;
/*******************************************************************************
 *                         Static Function Prototypes		                   *
 *******************************************************************************/
static void DW_Clock_Modes(void);
static void DW_StopWatch_Modes(void);
/*******************************************************************************
 *                             Implementation   				                *
 *******************************************************************************/

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
    Inc_switch_status = Button_isPressed(Increment);
    Mode_switch_status = Button_isPressed(Mode);
    Edit_switch_status = Button_isPressed(Edit);
    Mode_held_status = Button_isPressedAndHeld(Mode);
    Edit_held_status = Button_isPressedAndHeld(Edit);
    switch (Current_Operation_Mode)
    {
    case DW_Mode_Clock:
        /* Execute clock display modes */
        DW_Clock_Modes();
        /* Switch to stopwatch mode if the mode switch is pressed */
        if (Mode_held_status == BUTTON_IS_HELD && Edit_held_status == BUTTON_IS_HELD)
        {
            Current_Operation_Mode = DW_Mode_Reset;
        }
        if ((Clock_Prev_State != DW_Mode_Clock_Edit) && (Mode_switch_status == BUTTON_IS_PRESSED) && Edit_switch_status == BUTTON_IS_NOT_PRESSED) 
        {

            Current_Operation_Mode = DW_Mode_StopWatch;
            Display_clearScreenAsync();
        }
        break;
    case DW_Mode_StopWatch:
        /* Execute stopwatch display modes */
        DW_StopWatch_Modes();
        /* Switch to stopwatch mode if the mode switch is pressed */
        if (Mode_held_status == BUTTON_IS_HELD && Edit_held_status == BUTTON_IS_HELD)
        {
            Current_Operation_Mode = DW_Mode_Reset;
        }
        if ((Mode_switch_status == BUTTON_IS_PRESSED) && Edit_switch_status == BUTTON_IS_NOT_PRESSED)
        {
            Current_Operation_Mode = DW_Mode_Clock;
            Display_clearScreenAsync();
        }
        break;
    case DW_Mode_Reset:
        Clock_SetHours(0);
        Clock_SetMinutes(0);
        Clock_SetSeconds(0);
        Clock_SetDays(1);
        Clock_SetMonths(1);
        Clock_SetYears(2000);
        StopWatch_reset();
        Display_clearScreenAsync();
        Current_Operation_Mode = DW_Mode_Clock;
        StopWatch_Start_Stop = DW_Mode_StopWatch_Show;

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

    char buff[30] = {'0', '0'};
    int len;
    int current_row = 0;
    uint16_t Inc_Amount = 0;
    uint16_t new_tens = 0;
    uint16_t new_ones = 0;
    /* Static variable to keep track of the current position in edit mode */
    static DW_EditMode current_posstion = DW_HR_SEC_DIG;
    Time = Clock_CalculateCurrentTime();
    /* Buffer to store formatted time strings */
    /* Set the cursor to the beginning of the display */
    Display_setCursorAsync(0, 0);
    switch (Current_Clock_Mode)
    {
    case DW_Mode_Clock_Show:
        /* Display the current time */
        len = sprintf(buff, "%.2d:%.2d:%.2d:%d", (int)Time->hour, (int)Time->minute, (int)Time->second, (int)Time->secondMS);
        Display_printCenteredAsync(buff, len);
        Display_setCursorAsync(1, 0);
        len = sprintf(buff, "%.2d/%.2d/%.4d", (int)Time->day, (int)Time->month, (int)Time->year);
        Display_printCenteredAsync(buff, len);
        Clock_Prev_State = DW_Mode_Clock_Show;
        /* Switch to edit mode if the edit switch is pressed */
        if (Edit_switch_status == BUTTON_IS_PRESSED)
        {
            Current_Clock_Mode = DW_Mode_Clock_Edit;
            current_posstion = DW_HR_SEC_DIG;
        }
        break;
    case DW_Mode_Clock_Edit:
        /* Display the current time in edit mode */
        len = sprintf(buff, "%.2d:%.2d:%.2d:%d", (int)Time->hour, (int)Time->minute, (int)Time->second, (int)Time->secondMS);
        Display_printCenteredAsync(buff, len);
        Display_setCursorAsync(1, 0);
        len = sprintf(buff, "%.2d/%.2d/%.4d", (int)Time->day, (int)Time->month, (int)Time->year);
        Display_printCenteredAsync(buff, len);
        current_row = (current_posstion / (DW_SEC_FST_DIG + 1)) < 1 ? (current_posstion / (DW_SEC_FST_DIG + 1)) : 1;
        Display_blinkChar(current_row, DigitCol[current_posstion]);
        /* Handle increment switch press to edit time components */
        if (Inc_switch_status == BUTTON_IS_PRESSED)
        {
            switch (current_posstion)
            {
            case DW_HR_SEC_DIG:
                Inc_Amount = 10;
                Clock_SetHours(Time->hour + (uint8_t)Inc_Amount);
                break;
            case DW_HR_FST_DIG:
                Inc_Amount = 1;
                new_tens = (Time->hour / 10) * 10;
                if (Time->hour >= 23)
                {
                    new_ones = 0;
                    new_tens = 20;
                }
                else
                {
                    new_ones = (Time->hour + Inc_Amount) % 10;
                }
                Clock_SetHours(new_tens + new_ones);
                break;
            case DW_MIN_SEC_DIG:
                Inc_Amount = 10;
                Clock_SetMinutes(Time->minute + Inc_Amount);
                break;
            case DW_MIN_FST_DIG:
                Inc_Amount = 1;
                Clock_SetMinutes(((Time->minute / 10) * 10) + ((Time->minute % 10 + Inc_Amount) % 10));
                break;
            case DW_SEC_SEC_DIG:
                Inc_Amount = 10;
                Clock_SetSeconds(Time->second + Inc_Amount);
                break;
            case DW_SEC_FST_DIG:
                Inc_Amount = 1;
                Clock_SetSeconds(((Time->second / 10) * 10) + ((Time->second % 10 + Inc_Amount) % 10));
                break;
            case DW_YEAR_FST_DIG:
                Inc_Amount = 1000;
                new_tens = (Time->year / 1000) * 1000 + Inc_Amount;
                new_ones = (Time->year % 1000);
                Clock_SetYears(new_tens + new_ones);
                break;
            case DW_YEAR_SEC_DIG:
                Inc_Amount = 100;
                Clock_SetYears(((Time->year / 1000) * 1000) + ((Time->year + Inc_Amount) % 1000));
                break;
            case DW_YEAR_THR_DIG:
                Inc_Amount = 10;
                Clock_SetYears(((Time->year / 100) * 100) + ((Time->year + Inc_Amount) % 100));
                break;
            case DW_YEAR_FTH_DIG:
                Inc_Amount = 1;
                Clock_SetYears(((Time->year / 10) * 10) + ((Time->year + Inc_Amount) % 10));
                break;
            case DW_MONTH_FST_DIG:
                Inc_Amount = 10;
                Clock_SetMonths(Time->month + Inc_Amount);
                break;
            case DW_MONTH_SEC_DIG:
                Inc_Amount = 1;
                new_tens = (Time->month / 10) * 10;
                if (Time->month >= 12)
                {
                    new_ones = 0;
                    new_tens = 10;
                }
                else
                {
                    new_ones = (Time->month + Inc_Amount) % 10;
                }
                Clock_SetMonths(new_tens + new_ones);
                break;
            case DW_DAY_FST_DIG:
                Inc_Amount = 10;

                // Calculate the new tens digit of the day
                int new_tens = (Time->day / 10 + 1) * 10; // Increment tens digit

                // If the new tens digit exceeds the maximum days in the month, reset it to 0
                if (new_tens >= DW_days_in_month[(Time->month) - 1])
                {
                    if (new_tens < 40)
                    {
                        new_tens = 30;
                        new_ones = 0;
                    }
                    else if (new_tens >= 40 || ((Time->month == 2) && (new_tens >= 30)))
                    {
                        new_tens = 0;
                        new_ones = 1;
                    }
                }

                // Update the display with the new day value
                Clock_SetDays(new_tens + new_ones);
                break;
            case DW_DAY_SEC_DIG:
                Inc_Amount = 1;
                if (Time->day >= DW_days_in_month[(Time->month) - 1])
                {
                    new_ones = 0;
                    if (Time->month == 2)
                    {
                        new_tens = 20;
                    }
                    else
                    {
                        new_tens = 30;
                    }
                }
                else
                {
                    new_ones = (Time->day + Inc_Amount) % 10;
                    new_tens = (Time->day / 10) * 10;
                }
                Clock_SetDays(new_tens + new_ones);
                break;
            default:
                current_posstion = DW_HR_SEC_DIG;
                break;
            }
        }

        /* Switch back to show mode if mode switch is pressed */
        if (Mode_switch_status == BUTTON_IS_PRESSED)
        {
            Current_Clock_Mode = DW_Mode_Clock_Show;
            Clock_Prev_State = DW_Mode_Clock_Edit;
            current_posstion = DW_HR_SEC_DIG;
            Display_stopBlinkChar();
        }

        /* Move to the next digit in edit mode if edit switch is pressed */
        if (Edit_switch_status == BUTTON_IS_PRESSED)
        {
            Current_Clock_Mode = DW_Mode_Clock_Edit_Next_Dig;
        }
        break;

    case DW_Mode_Clock_Edit_Next_Dig:
        /* Move to the next digit in edit mode */
        current_posstion++;
        if (current_posstion >= _DW_DIGIT_NUM)
        {
            current_posstion = DW_HR_SEC_DIG;
        }
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

    /* Buffer to store formatted stopwatch time string */
    char buff[30] = {0};

    /* Pointer to hold the current stopwatch time information */
    const StopWatch_time_t *stopWatchTime = StopWatch_getCurrentTime();

    /* Format the stopwatch time string */
    int len = sprintf(buff, "%.2d:%.2d:%.2d:%.2d", (int)(stopWatchTime->hours), (int)(stopWatchTime->minutes), (int)(stopWatchTime->seconds), (int)(stopWatchTime->secondsMS / 10));
    buff[len] = 0;
    Display_setCursorAsync(0, 0);
    switch (Current_StopWatch_Mode)
    {
    case DW_Mode_StopWatch_Show:
        /* Display the stopwatch time */
        Display_printCenteredAsync(buff, len);

        /* Switch to operation mode if edit switch is pressed */
        if (Edit_switch_status == BUTTON_IS_PRESSED && Mode_switch_status == BUTTON_IS_NOT_PRESSED)
        {
            // Current_StopWatch_Mode = DW_Mode_StopWatch_Operation;
            StopWatch_Start_Stop++;
            Current_StopWatch_Mode = StopWatch_Start_Stop;
            if (Current_StopWatch_Mode == DW_Mode_StopWatch_Pause )
            {
                StopWatch_pause();
                Current_StopWatch_Mode = DW_Mode_StopWatch_Show;
            }
        }
        /* Reset stopwatch if increment switch is pressed */
        if (Inc_switch_status == BUTTON_IS_PRESSED)
        {
            Current_StopWatch_Mode = DW_Mode_StopWatch_Reset;
            StopWatch_Start_Stop = DW_Mode_StopWatch_Show;
        }
        break;
    case DW_Mode_StopWatch_Start:
        StopWatch_start();
        Current_StopWatch_Mode = DW_Mode_StopWatch_Show;
        break;
    case DW_Mode_StopWatch_Resume:
        StopWatch_resume();
        Current_StopWatch_Mode = DW_Mode_StopWatch_Show;
        StopWatch_Start_Stop = DW_Mode_StopWatch_Start;
        break;
    case DW_Mode_StopWatch_Reset:
        /* Reset the stopwatch and switch back to show mode */
        StopWatch_reset();
        StopWatch_Start_Stop = DW_Mode_StopWatch_Show;
        Current_StopWatch_Mode = DW_Mode_StopWatch_Show;
        break;
    default:
        break;
    }
}
