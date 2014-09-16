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

float stop_timer(void)
{
    return (float(clock() - begin_time) / CLOCKS_PER_SEC);
}

    
