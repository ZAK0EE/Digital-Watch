/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "StopWatch.h"
#include "Frames_cfg.h"
#include "Mode.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define STOPWATCH_RUNNABLE_PREIODICTY (u8)50

#define STOPWATCH_RESET         FRAME_SWITCH_NO1
#define STOPWATCH_MODE          FRAME_SWITCH_NO2
#define STOPWATCH_START_STOP    FRAME_SWITCH_NO3

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

// Global variables
StopWatchState stopWatchState = Stopped;
StopWatchTime_t currentStopWatchTime = {0, 0, 0, 0, 0, 0, 0, 0};

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

// Runnable to update the current Stop Watch Time
void updateStopWatchTime()
{
    static u8 msCounter = 0; // Counter to track milliseconds

    // Check if the Stop Watch is started
    if (stopWatchState == Started)
    {
        // Increment the millisecond counter
        msCounter += STOPWATCH_RUNNABLE_PREIODICTY;

        // Check if 100ms has passed
        if (msCounter >= 100)
        {
            // Increment milliseconds
            currentStopWatchTime.Ms1++;
            msCounter = 0;

            // Check if Ms1 reached 10 (equivalent to 100ms)
            if (currentStopWatchTime.Ms1 >= 10)
            {
                currentStopWatchTime.Ms1 = 0;
                // Increment seconds
                currentStopWatchTime.S2++;
                if (currentStopWatchTime.S2 >= 10)
                {
                    currentStopWatchTime.S2 = 0;
                    currentStopWatchTime.S1++;
                    if (currentStopWatchTime.S1 >= 6)
                    {
                        currentStopWatchTime.S1 = 0;
                        // Increment minutes
                        currentStopWatchTime.M2++;
                        if (currentStopWatchTime.M2 >= 10)
                        {
                            currentStopWatchTime.M2 = 0;
                            currentStopWatchTime.M1++;
                            if (currentStopWatchTime.M1 >= 6)
                            {
                                currentStopWatchTime.M1 = 0;
                                // Increment hours
                                currentStopWatchTime.H2++;
                                if (currentStopWatchTime.H2 >= 10)
                                {
                                    currentStopWatchTime.H2 = 0;
                                    currentStopWatchTime.H1++;
                                    if (currentStopWatchTime.H1 >= 10)
                                    {
                                        currentStopWatchTime.H1 = 0; // Reset hours if it exceeds 9
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

void controlSW(u8 frame)
{
    switch (frame)
    {
    case STOPWATCH_RESET:
        // Reset current Stop Watch time and state
        currentStopWatchTime.H1 = 0;
        currentStopWatchTime.H2 = 0;
        currentStopWatchTime.M1 = 0;
        currentStopWatchTime.M2 = 0;
        currentStopWatchTime.S1 = 0;
        currentStopWatchTime.S2 = 0;
        currentStopWatchTime.Ms1 = 0;
        currentStopWatchTime.Ms2 = 0;
        stopWatchState = Stopped;
        break;
    case STOPWATCH_MODE:
        changeMode(frame);
        break;
    case STOPWATCH_START_STOP:
        // Toggle current Stop Watch state
        if (stopWatchState == Stopped)
        {
            stopWatchState = Started;
        }
        else
        {
            stopWatchState = Stopped;
        }
        break;

    default:
        break;
    }
}

StopWatchTime_t getSW(void)
{
    return currentStopWatchTime;
}
