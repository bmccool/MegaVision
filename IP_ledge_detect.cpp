/******************************************************************************
* IP_ledge_detect.cpp
* This file contains functions that deal with detecting ledges.
*
******************************************************************************/

/////////////////////////// STANDARD INCLUDES  ////////////////////////////////
#include<opencv2/opencv.hpp>
#include <iostream>

/////////////////////////// PROJECT INCLUDES   ////////////////////////////////
#include "IP_ledge_detect.hpp"

/////////////////////////// EXPORTED FUNCTIONS ////////////////////////////////
/******************************************************************************
* Function Name: detect_lines
* 
*
******************************************************************************/
//#define PHOUGH_MIN_LENGTH 10
//#define PHOUGH_MAX_LINE_GAP 5
//#define PHOUGH_THRESHOLD (PHOUGH_MIN_LENGTH - PHOUGH_MAX_LINE_GAP)
#define ABS_MAX_LEDGE_SLOPE 0.1
#define DEBUG 0

using namespace cv;
using namespace std;

int detect_lines_hough_prob(Mat input_mat, int max_line_length, int max_line_gap, int threshold_var, Mat& output_mat)
{
    if(input_mat.empty())
    {
        cout << "Matrix is empty!" << endl;
        return -1;
    }

    Mat intermediate_mat;
    Canny(input_mat, intermediate_mat, 50, 200, 3);
    cvtColor(intermediate_mat, output_mat, CV_GRAY2BGR);

    // Probabilistic Hough Line Transform
    // First, apply the transform
    vector<Vec4i> lines;
    HoughLinesP(intermediate_mat, lines, 1, CV_PI/180, threshold_var, max_line_length, max_line_gap );
	
	// Display the result by drawing the lines
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        if (!(l[0] - l[2] == 0) && // If the line is NOT purely vertical (div by zero protection) and...
           (((float)(l[1] - l[3]) / (float)(l[0] - l[2])) < ABS_MAX_LEDGE_SLOPE) && // If the slope of the line is less than 1/2 and...
           (((float)(l[1] - l[3]) / (float)(l[0] - l[2])) > -ABS_MAX_LEDGE_SLOPE)) // If the slope of the line is greater than 1/2.
        {
            #if (DEBUG)
            cout << "(" << l[0] << ", " << l[1] << "), (" << l[2] << ", " << l[3] << ") Slope = " << ((float)(l[1] - l[3]) / (float)(l[0] - l[2])) << endl;
            #endif
            // only draw lines with slope less than 1/2
            line( output_mat, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,255,0), 1, CV_AA);
        }
    }
    return 0;
}

int detect_lines_hough(Mat input_mat, int max_line_length, int max_line_gap, int threshold_var, Mat& output_mat)
{
    if(input_mat.empty())
    {
        cout << "Matrix is empty!" << endl;
        return -1;
    }

    Mat intermediate_mat;
    Canny(input_mat, intermediate_mat, 50, 200, 3);
    cvtColor(intermediate_mat, output_mat, CV_GRAY2BGR);

    // Standard Hough Line Transform
    vector<Vec2f> lines;
    HoughLines(intermediate_mat, lines, 1, CV_PI/180, 100, 0, 0 );

    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( output_mat, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
    }

    return 0;
}

int find_countours(Mat input_mat, Mat & output_mat)
{
    Mat canny_output, src_gray;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    int thresh = 100;
    RNG rng(12345);
    
    /// Convert image to gray and blur it
    cvtColor(input_mat, src_gray, CV_BGR2GRAY);
    blur(src_gray, src_gray, Size(3,3) );

    /// Detect edges using canny
    Canny(src_gray, canny_output, thresh, thresh*2, 3 );
    
    /// Find contours
    findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Draw contours
    output_mat = Mat::zeros(canny_output.size(), CV_8UC3 );
    for(unsigned int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours(output_mat, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }
    return 0;
}