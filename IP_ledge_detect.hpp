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
#include <cmath>
/////////////////////////// PROJECT INCLUDES   ////////////////////////////////

/////////////////////////// DEFINITIONS        ////////////////////////////////
#define IMSHOW(WindowName, MatName) \
{\
    imshow((WindowName),(MatName));\
    waitKey(1)

// Types
typedef std::vector<cv::Point> contour_t;
typedef std::vector<contour_t> contours_t;


// Exported function prototypes
int detect_lines_hough_prob(cv::Mat src, int max_line_length, int max_line_gap, int threshold_var, cv::Mat& output_mat);
int detect_lines_hough(cv::Mat src, int max_line_length, int max_line_gap, int threshold_var, cv::Mat& output_mat);
contours_t find_contours(cv::Mat input_mat);
contours_t remove_duplicate_contours(contours_t contours_old, contours_t contours_new);
void draw_contours(cv::Mat input_mat, cv::Mat & output_mat, contours_t contours);
int sidescroll_right_gray(cv::Mat & old_image, cv::Mat & new_image);

#endif