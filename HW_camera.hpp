/******************************************************************************
* HW_camera.hpp
* This header contains prototypes and structures needed by the file
* HW_camera.cpp
*
******************************************************************************/
// Make sure this is only included once
#ifndef HW_CAMERA_HPP
#define HW_CAMERA_HPP
// Definitions
#define ERR_NONE               0
#define ERR_CAMERA_OPEN_FAILED 1

// Exported function prototypes
int HW_camera_open(VideoCapture *capture);
int HW_camera_close(VideoCapture *capture);

#endif