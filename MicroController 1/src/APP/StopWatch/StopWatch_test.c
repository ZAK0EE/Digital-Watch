
#include "APP/Display/Display.h"
#include "HAL/LCD/LCD.h"
#include "MCAL/RCC/RCC.h"
#include "Services/Scheduler/Scheduler.h"
#include "APP/StopWatch/StopWatch.h"
void assert_failed(void)
{
    while(1);
}
#include <stdio.h>

// task periodicity 50ms, delay 20ms
void Stopwatch_test(void)
{
    static uint64_t timeMS = 0;
    if(timeMS == 0)
    {
        StopWatch_start();
    }

    char buff[30]= {0};
    const StopWatch_time_t *stopWatchTime = StopWatch_getCurrentTime();
    int len = sprintf(buff, "%.2d:%.2d:%.2d:%.2d", (int)(stopWatchTime->hours), (int)(stopWatchTime->minutes), (int)(stopWatchTime->seconds), (int)(stopWatchTime->secondsMS / 10));
    buff[len] = 0;
    Display_printCenteredAsync(buff, len);

    if((timeMS)/1000.0 == 5.0)
    {
        StopWatch_pause();
    }
    else if((timeMS)/1000.0 == 10.0)
    {
        StopWatch_resume();
    } 
    else if((timeMS)/1000.0 == 15.0)
    {
        StopWatch_pause();
    } 
    else if((timeMS)/1000.0 == 20.0)
    {
        StopWatch_resume();
    }
    else if((timeMS)/1000.0 == 25.0)
    {
        StopWatch_reset();
        StopWatch_start();
    }                 
    timeMS += 50;
}

void stopwatch_test_main(void)
{
    RCC_Enable_AHB1_Peripheral(GPIOA);
    LCD_init(LCD1);
    Display_init();

    Sched_init();
    Sched_start();
    
}
