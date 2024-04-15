#include "Services/Scheduler/Scheduler.h"

void assert_failed(void)
{
     while(1);
}
int main(void)
{

     Sched_init();
     Sched_start();
}
