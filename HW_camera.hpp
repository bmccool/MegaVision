/******************************************************************************
* HW_camera.hpp
* This header contains prototypes and structures needed by the file
* HW_camera.cpp
*
******************************************************************************/
// Make sure this is only included once
#ifndef HW_CAMERA_HPP
#define HW_CAMERA_HPP

/////////////////////////// STANDARD INCLUDES  ////////////////////////////////
#include<iostream>
#include<opencv2/opencv.hpp>
#include <string>
#include <time.h>
#include <stdio.h>
/////////////////////////// PROJECT INCLUDES   ////////////////////////////////

// Definitions
#define ERR_NONE               0
#define ERR_CAMERA_OPEN_FAILED 1

// Exported function prototypes
int HW_camera_open(cv::VideoCapture *capture);
int HW_camera_close(cv::VideoCapture *capture);
double HW_camera_get_height(cv::VideoCapture *capture);
double HW_camera_get_width(cv::VideoCapture *capture);
#endif