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
/////////////////////////// LOCAL FUNCTION PROTOTYPES /////////////////////////
bool contour_match(contour_t c1, contour_t c2);
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

contours_t find_contours(Mat input_mat)
{
    Mat canny_output, src_gray;
    contours_t contours;
    vector<Vec4i> hierarchy;
    
    // Convert image to gray and blur it
    cvtColor(input_mat, src_gray, CV_BGR2GRAY);
    blur(src_gray, src_gray, Size(3,3) );

    // Detect edges using canny
    Canny(src_gray, canny_output, 50, 200, 3 );
    
    // Find contours
    findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    return contours;
}

contours_t remove_duplicate_contours(contours_t contours_old, contours_t contours_new)
{
    contours_t output; // The output vector can at most be size of the new contours
    output.resize(contours_new.size());
    
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int k = 0;
    for (i = 0; i < contours_new.size(); i++)
    {
        // For each new contour, check if it matches any old contours.
        // If there is a match, set the index to bound + 5
        for (j = 0; j < contours_old.size(); j++)
        {
            if (contour_match(contours_new[i], contours_old[j]))
            {
                cout << "This contour is the same!" << endl;
                // This contour is not actually new; exclude it
                j = contours_old.size() + 5;
            }    
        }
        // Add this contour to the output array if it is truly new (j !+ 5)
        if (j == contours_old.size() + 5)
        {
            output[k++] = contours_new[i];
        }
        
        
    }

    // Resize the output vector since hopefully the "new" contours are smaller in
    // number than all contours.
    output.resize(k+1);
    return output;
}

bool contour_match(contour_t c1, contour_t c2)
{
    // contours are vectors containing points, so to check that they are the same,
    // we first need to check if the size of the two contours are the same.
    if (c1.size() != c2.size())
    {
        // The sizes are not the same, return false
        return false;
    }
    // If the sizes are the same, then we need to walk down the contours and compare
    // each point.  If at any time, the points are not the same, then the contour is
    // not the same, and we can exit early.
    for (unsigned int i = 0; i < c1.size(); i++)
    {
        // check if the points are equal to each other
        if (c1[i] != c2[i])
        {
            // The points are not equal
            return false;
        }
        cout << "(" << c1[i].x << ", " << c1[i].y << ") == (" << c2[i].x << ", " << c2[i].y << ")" << endl;
    }
    
    // If we have made it this far and the points all agree, then the contours are equal.
    return true;
}

// This function does NOT use hierarchy, if it is to use hierarchy, we need
// to take a look at find contours so that we are returning hierarchy somehow.
void draw_contours(Mat input_mat, Mat & output_mat, contours_t contours)
{
    RNG rng(12345);
    
    // Draw contours
    output_mat = Mat::zeros(input_mat.size(), CV_8UC3 );
    for(unsigned int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
        drawContours(output_mat, contours, i, color, 2, 8, noArray(), 0, Point() );
    }
}