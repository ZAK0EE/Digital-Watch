#include "Services/Scheduler.h"

extern void initMyApp(void);

int main(int argc, char const *argv[])


{
    initMyApp();
    Sched_init();
    Sched_start();
    

    return 0;
}
