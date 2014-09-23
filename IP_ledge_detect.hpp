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
// Types
typedef std::vector<std::vector<std::Point>> contours_t;

// Exported function prototypes
int detect_lines_hough_prob(cv::Mat src, int max_line_length, int max_line_gap, int threshold_var, cv::Mat& output_mat);
int detect_lines_hough(cv::Mat src, int max_line_length, int max_line_gap, int threshold_var, cv::Mat& output_mat);
int find_countours(cv::Mat input_mat, cv::Mat & output_mat);

#endif