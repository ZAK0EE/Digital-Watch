#include "APP/Display/Display.h"
#include "HAL/LCD/LCD.h"
#include "MCAL/RCC/RCC.h"
#include "Services/Scheduler/Scheduler.h"
void assert_failed(void)
{
    while(1);
}
int main(void)
{
    RCC_Enable_AHB1_Peripheral(GPIOA);
    LCD_init(LCD1);
   // LCD_writeStringAsync(LCD1, "ABC", 3);
    Display_init();
    char msg[] = "Hello, world";
    Display_printAsync(msg, 12);
    Display_setCursorAsync(1, 0);
    Display_printAsync("ABC", 3);
    Sched_init();
    Sched_start();
    
}