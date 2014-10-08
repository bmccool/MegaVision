/******************************************************************************
* CS_time.hpp
* This header contains prototypes and structures needed by the file
* CS_time.cpp
*
******************************************************************************/
// Make sure this is only included once
#ifndef CS_TIME_HPP
#define CS_TIME_HPP


/////////////////////////// STANDARD INCLUDES  ////////////////////////////////
/////////////////////////// PROJECT INCLUDES   ////////////////////////////////
// Exported function prototypes
void start_timer(void);
float get_elapsed_time(void);
void sleep_for_milliseconds(int sleep_milli);

#endif