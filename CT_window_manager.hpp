/******************************************************************************
* CT_window_manager.hpp
* This header contains prototypes and structures needed by the file
* CT_window_manager.cpp.  This set of functions is used by the program in a
* Ubuntu Desktop environment to find and manage windows.
*
******************************************************************************/
// Make sure this is only included once
#ifndef CT_WINDOW_MANAGER_HPP
#define CT_WINDOW_MANAGER_HPP


/////////////////////////// STANDARD INCLUDES  ////////////////////////////////
#include <string>
/////////////////////////// PROJECT INCLUDES   ////////////////////////////////
// Exported function prototypes
int send_key_down(string keycode, unsigned long int window_id);
int send_key_up(string keycode, unsigned long int window_id);
int send_key(string keycode, unsigned long int window_id);
unsigned long int get_nes_window(void);

#endif