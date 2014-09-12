/******************************************************************************
* IP_ledge_detect.hpp
* This header contains prototypes and structures needed by the file
* IP_ledge_detect.cpp
*
******************************************************************************/
// Make sure this is only included once
#ifndef IP_LEDGE_DETECT_HPP
#define IP_LEDGE_DETECT_HPP

/////////////////////////// STANDARD INCLUDES  ////////////////////////////////
#include<iostream>
#include<opencv2/opencv.hpp>
#include <string>
#include <time.h>
#include <stdio.h>
/////////////////////////// PROJECT INCLUDES   ////////////////////////////////

// Definitions


// Exported function prototypes
int detect_lines(cv::Mat src, int max_line_length, int max_line_gap, int threshold, Mat dst);

#endif