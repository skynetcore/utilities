#include <stdlib.h>

#define PROC_USE_SEMAPHORE
#define PROC_USE_LOGGER printf

#include "../include/proc_lock.h"


int main()
{
    //lock
    PROC_LOCK
    printf("running speedtest unit test 1.0 \n");
    sleep(10);
    printf("sleep is over for speedtest unit test 1.0 \n");

}

