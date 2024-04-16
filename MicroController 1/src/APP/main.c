#include "Services/Scheduler/Scheduler.h"
#include "APP/INIT/INIT.h"
void assert_failed(void)
{
     while(1);
}

extern void Button_test_main(void);
int main(void)
{
    InitSys();
    Button_test_main();
    Sched_init();
    Sched_start();
}
