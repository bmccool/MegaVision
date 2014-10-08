/******************************************************************************
* IP_ledge_detect.cpp
* This file contains functions that deal with detecting ledges.
*
******************************************************************************/

/////////////////////////// STANDARD INCLUDES  ////////////////////////////////
#include <time.h>

/////////////////////////// PROJECT INCLUDES   ////////////////////////////////
#include "CS_time.hpp"

/////////////////////////// GLOBAL VARIABLES   ////////////////////////////////
static clock_t begin_time;

/////////////////////////// EXPORTED FUNCTIONS ////////////////////////////////
void start_timer(void)
{
    begin_time = clock();
    return;
}

float get_elapsed_time(void)
{
    return (float(clock() - begin_time) / CLOCKS_PER_SEC);
}

void sleep_for_milliseconds(int sleep_milli)
{
    // We were passed the time in milliseconds
    // Convert this time to nanoseconds (*1000000)
    // Also, this needs to be packed into the timespec struct
    
    struct timespec time1, time2;  //Time2 will return the remaining time if we are interrupted.
    // For now, Time2 is unused.
    
    time1.tv_sec = 0;
    time1.tv_nsec = sleep_milli * 1000000;
    
    nanosleep(&time1, &time2);
}
