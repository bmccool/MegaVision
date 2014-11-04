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
    waitKey(1);\
}

enum Direction{
    ShiftUp=1, ShiftRight, ShiftDown, ShiftLeft
   };

// Types
typedef std::vector<cv::Point> contour_t;
typedef std::vector<contour_t> contours_t;
typedef std::vector<cv::Point> points_t;
typedef std::vector<cv::Rect> boxes_t;

// Exported function prototypes
int detect_lines_hough_prob(cv::Mat src, int max_line_length, int max_line_gap, int threshold_var, cv::Mat& output_mat);
int detect_lines_hough(cv::Mat src, int max_line_length, int max_line_gap, int threshold_var, cv::Mat& output_mat);
contours_t find_contours(cv::Mat input_mat);
contours_t remove_duplicate_contours(contours_t contours_old, contours_t contours_new);
void draw_contours(cv::Mat input_mat, cv::Mat & output_mat, contours_t contours);
int sidescroll_right_gray(cv::Mat & old_image, cv::Mat & new_image);
cv::Mat shiftFrame(cv::Mat frame, int pixels, Direction direction);
void mark_line(cv::Mat & input, int value);
cv::Mat get_fore(cv::Mat & old_mat, cv::Mat & new_mat);
cv::Mat get_foreground(cv::Mat & old_mat, cv::Mat & new_mat, int threshold_value);
points_t get_foreground_points(cv::Mat & old_mat, cv::Mat & new_mat, int threshold_val);
void draw_box_on_foreground(cv::Mat & input_mat, points_t points);
bool point_is_close_to_box(cv::Point point_val, cv::Rect box, int close);
int distance_between(cv::Point p1, cv::Point p2);
void expand_box(cv::Point point_val, cv::Rect & box);
void draw_boxes(boxes_t boxes, cv::Mat & mat);
void creating_box_test(cv::Mat mat);
void creating_boxes_test(cv::Mat mat, int num_boxes_per_line);
cv::Mat draw_points(points_t points, cv::Mat & input_mat);


#endif
