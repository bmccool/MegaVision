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

enum key_action_t
{
    KEY_PRESS,
    KEY_RELEASE,
    KEY_PRESS_RELEASE
};

#define CT_ERROR 2
/////////////////////////// STANDARD INCLUDES  ////////////////////////////////
/////////////////////////// PROJECT INCLUDES   ////////////////////////////////
// Exported function prototypes
int send_key(char* keycode, key_action_t action, unsigned long int window_id);
unsigned long int get_nes_window(void);

#endif
