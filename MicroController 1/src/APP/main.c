#include "Services/Scheduler/Scheduler.h"
#include "APP/INIT/INIT.h"
void assert_failed(void)
{
     while(1);
}

int main(void)
{
    InitSys();

    //Display_blinkChar(0, 3);
    Sched_init();
    Sched_start();
}
