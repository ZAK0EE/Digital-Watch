/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "DateAndTime.h"
#include "Mode.h"
#include "Frames_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

// Define the periodicity (in milliseconds)
#define PERIODICITY_MS (u32)50

#define SWITCH_INC          FRAME_SWITCH_NO1
#define SWITCH_MODE         FRAME_SWITCH_NO2
#define SWITCH_NEXT_DIGIT   FRAME_SWITCH_NO3

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

// Global variables for time and date
static Time currentTime = {.hours = 0, .minutes = 0, .seconds = 0, .milliseconds = 0};
static Date currentDate = {.day = 17, .month = 4, .year = 2024};
static DateTimeDigit currDigit = DIGIT_HOURS;

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

// Runnable to update time and date
void runnable_updateDateAndTime(void)
{
    // Increment milliseconds by the periodicity
    currentTime.milliseconds += PERIODICITY_MS;

    // Check if milliseconds overflowed
    if (currentTime.milliseconds >= 100)
    {
        currentTime.milliseconds -= 100;
        currentTime.seconds++;

        // Check if seconds overflowed
        if (currentTime.seconds >= 60)
        {
            currentTime.seconds -= 60;
            currentTime.minutes++;

            // Check if minutes overflowed
            if (currentTime.minutes >= 60)
            {
                currentTime.minutes -= 60;
                currentTime.hours++;

                // Check if hours overflowed
                if (currentTime.hours >= 24)
                {
                    currentTime.hours = 0;
                    // Increment the date at midnight
                    currentDate.day++;

                    // Check if day overflowed based on the month and year
                    switch (currentDate.month)
                    {
                    case 2: // February
                        if ((currentDate.year % 4 == 0 && currentDate.year % 100 != 0) || (currentDate.year % 400 == 0))
                        {
                            if (currentDate.day > 29)
                            {
                                currentDate.day = 1;
                                currentDate.month++;
                            }
                        }
                        else
                        {
                            if (currentDate.day > 28)
                            {
                                currentDate.day = 1;
                                currentDate.month++;
                            }
                        }
                        break;
                    case 4:
                    case 6:
                    case 9:
                    case 11: // April, June, September, November
                        if (currentDate.day > 30)
                        {
                            currentDate.day = 1;
                            currentDate.month++;
                        }
                        break;
                    default: // January, March, May, July, August, October, December
                        if (currentDate.day > 31)
                        {
                            currentDate.day = 1;
                            if (currentDate.month == 12)
                            {
                                currentDate.month = 1;
                                currentDate.year++;
                            }
                            else
                            {
                                currentDate.month++;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
}

// API function to edit date and time
void edit_DateTime(u8 frame)
{
    switch (frame)
    {
    case SWITCH_INC:
        // Increment the current digit
        switch (currDigit)
        {
        case DIGIT_HOURS:
            currentTime.hours++;
            if (currentTime.hours >= 24)
            {
                currentTime.hours = 0;
            }
            break;
        case DIGIT_MINUTES:
            currentTime.minutes++;
            if (currentTime.minutes >= 60)
            {
                currentTime.minutes = 0;
            }
            break;
        case DIGIT_SECONDS:
            currentTime.seconds++;
            if (currentTime.seconds >= 60)
            {
                currentTime.seconds = 0;
            }
            break;
        case DIGIT_DAY:
            currentDate.day++;
            // Handle day overflow based on the month
            switch (currentDate.month)
            {
            case 2: // February
                if ((currentDate.year % 4 == 0 && currentDate.year % 100 != 0) || (currentDate.year % 400 == 0))
                {
                    if (currentDate.day > 29)
                    {
                        currentDate.day = 1;
                    }
                }
                else
                {
                    if (currentDate.day > 28)
                    {
                        currentDate.day = 1;
                    }
                }
                break;
            case 4:
            case 6:
            case 9:
            case 11: // April, June, September, November
                if (currentDate.day > 30)
                {
                    currentDate.day = 1;
                }
                break;
            default: // January, March, May, July, August, October, December
                if (currentDate.day > 31)
                {
                    currentDate.day = 1;
                }
                break;
            }
            break;
        case DIGIT_MONTH:
            currentDate.month++;
            if (currentDate.month > 12)
            {
                currentDate.month = 1;
            }
            break;
        case DIGIT_YEAR_THOUSANDS:
            currentDate.year += 1000;
            break;
        case DIGIT_YEAR_HUNDREDS:
            currentDate.year += 100;
            break;
        case DIGIT_YEAR_TENS:
            currentDate.year += 10;
            break;
        case DIGIT_YEAR_UNITS:
            currentDate.year++;
            break;
        }
        break;
    case SWITCH_MODE:
        exitEditDateAndTime();
        // You can add additional logic here if needed
        break;
    case SWITCH_NEXT_DIGIT:
        // Move to the next digit
        currDigit++;
        if (currDigit > DIGIT_YEAR_UNITS)
        {
            currDigit = DIGIT_HOURS; // Wrap around to the beginning
        }
        break;
    default:
        // Handle invalid frame
        break;
    }
}

// Getter API for time
Time getTime()
{
    return currentTime;
}

// Getter API for date
Date getDate()
{
    return currentDate;
}

// Getter API for current digit being edited
DateTimeDigit getCurrentDigit()
{
    return currDigit;
}
