#define BUTTON_TEST
#ifdef BUTTON_TEST

#include "APP/Display/Display.h"
#include "APP/Button/Button.h"

#include <stdio.h>
// Periodic task call:100ms Delay:20ms
void button_test(void)
{
    char buff[30] = {'0', '0'};
    static uint64_t timeMS = 0;
    int len = sprintf(buff, "%.2d:%.2d:%.2d", (int)(timeMS / 1000 / 60) % 24, (int)(timeMS / 1000) % 60, (int)(timeMS % 1000) / 10);
    Display_printCenteredAsync(buff, len);
    timeMS += 100;
}
int button_test_main(void)
{
    Button_init();
    
}

#endif