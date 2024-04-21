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
Time_t currentTime = {0, 0, 0, 0, 0, 0};
Date_t currentDate = {2, 2, 0, 4, 2, 0, 2, 4};
static Digits_t currDigit = H1;

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

// Runnable to update time and date
void runnable_updateDateAndTime(void)
{
    // Increment seconds
    currentTime.S2++;
    if (currentTime.S2 >= 10)
    {
        currentTime.S2 = 0;
        currentTime.S1++;
        if (currentTime.S1 >= 6)
        {
            currentTime.S1 = 0;
            // Increment minutes
            currentTime.M2++;
            if (currentTime.M2 >= 10)
            {
                currentTime.M2 = 0;
                currentTime.M1++;
                if (currentTime.M1 >= 6)
                {
                    currentTime.M1 = 0;
                    // Increment hours
                    currentTime.H2++;
                    if (currentTime.H2 >= 10)
                    {
                        currentTime.H2 = 0;
                        currentTime.H1++;
                        if (currentTime.H1 >= 2 && currentTime.H2 >= 4)
                        {
                            currentTime.H1 = 0;
                        }
                    }
                    // Increment day
                    currentDate.D2++;
                    if (currentDate.D2 >= 10)
                    {
                        currentDate.D2 = 0;
                        currentDate.D1++;
                        // Handle day overflow
                        if (currentDate.D1 >= 4)
                        {
                            currentDate.D1 = 0;
                            currentDate.M2++;
                            if (currentDate.M2 >= 10)
                            {
                                currentDate.M2 = 0;
                                currentDate.M1++;
                                if (currentDate.M1 >= 2)
                                {
                                    currentDate.M1 = 0;
                                    // Increment month
                                    currentDate.Y4++;
                                    if (currentDate.Y4 >= 10)
                                    {
                                        currentDate.Y4 = 0;
                                        currentDate.Y3++;
                                        if (currentDate.Y3 >= 10)
                                        {
                                            currentDate.Y3 = 0;
                                            currentDate.Y2++;
                                            if (currentDate.Y2 >= 10)
                                            {
                                                currentDate.Y2 = 0;
                                                currentDate.Y1++;
                                                // Handle year overflow
                                                if (currentDate.Y1 >= 10)
                                                {
                                                    // Reset year to 0
                                                    currentDate.Y1 = 0;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


// Function to edit date and time
void edit_DateTime(u8 frame) {
    switch (frame) {
        case SWITCH_INC:
            // Increment the current digit
            switch (currDigit) {
                case H1:
                    currentTime.H1++;
                    if (currentTime.H1 >= 3) {
                        currentTime.H1 = 0;
                    }
                    break;
                case H2:
                    currentTime.H2++;
                    if (currentTime.H1 == 2 && currentTime.H2 >= 4) {
                        currentTime.H2 = 0;
                    } else if (currentTime.H2 >= 10) {
                        currentTime.H2 = 0;
                    }
                    break;
                case M1:
                    currentTime.M1++;
                    if (currentTime.M1 >= 6) {
                        currentTime.M1 = 0;
                    }
                    break;
                case M2:
                    currentTime.M2++;
                    if (currentTime.M2 >= 10) {
                        currentTime.M2 = 0;
                    }
                    break;
                case S1:
                    currentTime.S1++;
                    if (currentTime.S1 >= 6) {
                        currentTime.S1 = 0;
                    }
                    break;
                case S2:
                    currentTime.S2++;
                    if (currentTime.S2 >= 10) {
                        currentTime.S2 = 0;
                    }
                    break;
                case D1:
                    currentDate.D1++;
                    if (currentDate.D1 >= 4) {
                        currentDate.D1 = 0;
                    }
                    break;
                case D2:
                    currentDate.D2++;
                    if (currentDate.D2 >= 10) {
                        currentDate.D2 = 0;
                    }
                    break;
                case Mo1:
                    currentDate.M1++;
                    if (currentDate.M1 >= 2) {
                        currentDate.M1 = 0;
                    }
                    break;
                case Mo2:
                    currentDate.M2++;
                    if (currentDate.M2 >= 10) {
                        currentDate.M2 = 0;
                    }
                    break;
                case Y1:
                    currentDate.Y1++;
                    if (currentDate.Y1 >= 10) {
                        currentDate.Y1 = 0;
                    }
                    break;
                case Y2:
                    currentDate.Y2++;
                    if (currentDate.Y2 >= 10) {
                        currentDate.Y2 = 0;
                    }
                    break;
                case Y3:
                    currentDate.Y3++;
                    if (currentDate.Y3 >= 10) {
                        currentDate.Y3 = 0;
                    }
                    break;
                case Y4:
                    currentDate.Y4++;
                    if (currentDate.Y4 >= 10) {
                        currentDate.Y4 = 0;
                    }
                    break;
            }
            break;
        case SWITCH_MODE:
            // Exit editing mode
            // Add additional logic here if needed
            break;
        case SWITCH_NEXT_DIGIT:
            // Move to the next digit
            currDigit++;
            if (currDigit >= Y4 + 1) {
                currDigit = H1; // Wrap around to the beginning
            }
            break;
        default:
            // Handle invalid frame
            break;
    }
}


// Getter API for time
Time_t getTime()
{
    return currentTime;
}

// Getter API for date
Date_t getDate()
{
    return currentDate;
}

// Getter API for current digit being edited
Digits_t getCurrentDigit()
{
    return currDigit;
}
