#include "APP/Display/Display.h"
#include "HAL/LCD/LCD.h"
#include "MCAL/RCC/RCC.h"
#include "Services/Scheduler/Scheduler.h"
void assert_failed(void)
{
    while(1);
}
#include <stdio.h>
// Periodic task call:100ms Delay:20ms
void display_test(void)
{
    char buff[30] = {'0', '0'};
    static uint64_t timeMS = 0;
    int len = sprintf(buff, "%.2d:%.2d:%.2d", (int)(timeMS / 1000 / 60) % 24, (int)(timeMS / 1000) % 60, (int)(timeMS % 1000) / 10);
    Display_printCenteredAsync(buff, len);
    timeMS += 100;
}
int test_main(void)
{
    RCC_Enable_AHB1_Peripheral(GPIOA);
    LCD_init(LCD1);
    Display_init();

    Sched_init();
    Sched_start();
    
}