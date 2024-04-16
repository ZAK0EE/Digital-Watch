#define BUTTON_TEST
#ifdef BUTTON_TEST

#include "APP/Display/Display.h"
#include "APP/Button/Button.h"

#include <stdio.h>
// Periodic task call:100ms Delay:100ms
void Button_test(void)
{
    char buff[30] = {'0', '0'};

    Display_setCursorAsync(0, 0);
    int len = sprintf(buff, "IN:%d MD:%d ED:%d", Button_isPressed(BUTTON_INCREMENT), Button_isPressed(BUTTON_MODE), Button_isPressed(BUTTON_EDIT));
    Display_printAsync(buff, len);

    Display_setCursorAsync(1, 0);
    len = sprintf(buff, " H:%d  H:%d  H:%d", Button_isPressedAndHeld(BUTTON_INCREMENT), Button_isPressedAndHeld(BUTTON_MODE), Button_isPressedAndHeld(BUTTON_EDIT));
    Display_printAsync(buff, len);    
}
void Button_test_main(void)
{
    Button_init();
    
}

#endif