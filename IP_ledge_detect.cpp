///////////////////////////////////////////////////////////////////////////////
/// @file IP_ledge_detect.cpp
/// This file contains functions that deal with detecting ledges.
///
///////////////////////////////////////////////////////////////////////////////

/////////////////////////// STANDARD INCLUDES  ////////////////////////////////
#include<opencv2/opencv.hpp>
#include <iostream>

/////////////////////////// PROJECT INCLUDES   ////////////////////////////////
#include "IP_ledge_detect.hpp"
#include "CS_time.hpp"

#define EQAULISH 60
#define EQUALISH 150
#define FREEBIE_PERCENT .2

using namespace cv;
using namespace std;
/////////////////////////// LOCAL FUNCTION PROTOTYPES /////////////////////////
bool contour_match(contour_t c1, contour_t c2);
int are_columns_equalish(Mat & m1, Mat & m2, int error);
unsigned long int score_column_likeness(Mat & m1, Mat & m2, int error);
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
                // This contour is not actually new; exclude it
                j = contours_old.size() + 4;
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
    }
    
    // If we have made it this far and the points all agree, then the contours are equal.
    return true;
}

void draw_contours(Mat input_mat, Mat & output_mat, contours_t contours)
{
    // This function does NOT use hierarchy, if it is to use hierarchy, we need
    // to take a look at find contours so that we are returning hierarchy somehow.

    RNG rng(12345);
    
    // Draw contours
    output_mat = Mat::zeros(input_mat.size(), CV_8UC3 );
    for(unsigned int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
        drawContours(output_mat, contours, i, color, 2, 8, noArray(), 0, Point() );
    }
}

int sidescroll_right_gray(Mat & old_image, Mat & new_image)
{
// 1. Convert both images to grayscale
// 2. Check if the first column of the two images is the same
//    if so return 0
// 3. Check if the first column of the old image is equal to the next column of the new image
//    if so return the index of the new image column that matches the first column of the old image
//    repeat step 3 until max size has been reached.


    // Convert image to gray
    Mat old_gray;
    Mat new_gray;
    cvtColor(old_image, old_gray, CV_BGR2GRAY);
    cvtColor(new_image, new_gray, CV_BGR2GRAY);
    int min_equalish_index, debug_equalish, min_equalish = 0;
    
    for (int i = 0; i < new_gray.cols; i++)
    {
        debug_equalish = are_columns_equalish(new_gray, old_gray, EQAULISH);
        if (debug_equalish == -1)
        {
            // These columns are relatively equal
            // Return their index
            return i;
        }
        if (debug_equalish < min_equalish)
        {
            min_equalish = debug_equalish;
            min_equalish_index = i;
        }
        
        // Shift the frame one to the right so we can try again.
        old_gray =  shiftFrame(old_gray, 1, ShiftLeft);
            
    }
    // no matches were found
    printf("The best match was %d at %d  ", min_equalish, min_equalish_index);
    return -1;
}

int are_columns_equalish(Mat & m1, Mat & m2, int error)
{
    // NOTE: THIS FUNCTION ASSUMES MATS ARE OF TYPE CV_8UC1.
    // OTHER TYPES OF MATS WILL PRODUCE UNRELIABL RESULTS
    
    // Return false if the matrices are not of equal size or if error is negative.
    // Go through the first column of each mat and compare each element.
    // If two elements are not within "error" of each other, return false
    // If we get to the end and thus haven't returned false, return true.
    
    // Note: Adding in a percent "freebies" so that a certain percentage
    // of pixels are allowed to completely mismatch.
    
    int mismatch = m1.rows * FREEBIE_PERCENT;
    
    for (int i = 0; i < m1.rows; i++)
    {
        if ((abs(m1.at<uchar>(i,1) - m2.at<uchar>(i,1)) > error) ||
            (abs(m1.at<uchar>(i,1) - m2.at<uchar>(i,1)) < 0))
        {
            if (mismatch > 0)
            {
                mismatch--;
            }
            else
            {
                return i;
            }
        }
    }
    return -1;
}

unsigned long int score_column_likeness(Mat & m1, Mat & m2, int error)
{

    unsigned long int score = 0;
    
    for (int i = 0; i < m1.rows; i++)
    {
        score += (abs(m1.at<uchar>(i,1) - m2.at<uchar>(i,1)));
    }
    return score;
}

Mat shiftFrame(Mat frame, int pixels, Direction direction)
{
    //create a same sized temporary Mat with all the pixels flagged as invalid (-1)
    Mat temp = Mat::zeros(frame.size(), frame.type());

    switch (direction)
    {
    case(ShiftUp) :
        frame(Rect(0, pixels, frame.cols, frame.rows - pixels)).copyTo(temp(cv::Rect(0, 0, temp.cols, temp.rows - pixels)));
        break;
    case(ShiftRight) :
        frame(Rect(0, 0, frame.cols - pixels, frame.rows)).copyTo(temp(cv::Rect(pixels, 0, frame.cols - pixels, frame.rows)));
        break;
    case(ShiftDown) :
        frame(Rect(0, 0, frame.cols, frame.rows - pixels)).copyTo(temp(cv::Rect(0, pixels, frame.cols, frame.rows - pixels)));
        break;
    case(ShiftLeft) :
        frame(Rect(pixels, 0, frame.cols - pixels, frame.rows)).copyTo(temp(cv::Rect(0, 0, frame.cols - pixels, frame.rows)));
        break;
    default:
        cout << "Shift direction is not set properly" << endl;
    }

    return temp;
}

void mark_line(Mat & input, int value)
{

    for (int i = 0; i < input.rows; i++)
    {
        input.at<uchar>(i,1) = value;
    }
}

Mat get_fore(Mat & old_mat, Mat & new_mat)
{
    Mat fore; // Foreground Mat
    BackgroundSubtractorMOG2 bg; // BG subtractor
    
    bg.operator ()(old_mat, fore); // Feed the first frame
    bg.operator ()(new_mat, fore); // Feed the second frame
    
    return fore; // Return the foreground calculated.
}

Mat get_foreground(Mat & old_mat, Mat & new_mat, int threshold_val)
{
    // NOTE: ROW:Y::COL:X
    // Convert images to gray
    Mat old_gray;
    Mat new_gray;
    cvtColor(old_mat, old_gray, CV_BGR2GRAY);
    cvtColor(new_mat, new_gray, CV_BGR2GRAY);
    Mat fore = Mat::zeros(new_mat.size(), new_gray.type()); // Foreground Mat

    for (int row_index = 0; row_index < new_gray.rows; row_index++)
    {
        // For each row in the new matrix
        for (int col_index = 0; col_index < new_mat.cols; col_index++)
        {
            // For each column in the new matrix
            if (threshold_val != -1)
            {
                // We are using a threshold.  If the difference of the pixels is less than the threshold value, color it white
                if ((abs(old_gray.at<uchar>(col_index, row_index) - new_gray.at<uchar>(col_index, row_index)) > threshold_val) ||
                    (abs(old_gray.at<uchar>(col_index, row_index) - new_gray.at<uchar>(col_index, row_index)) < 0)) // Needed to be inclusive and protect from wraparound, should never get here due to abs()
                {
                    fore.at<uchar>(col_index, row_index) = 255;
                }
                else
                {
                    fore.at<uchar>(col_index, row_index) = 0;
                }
            }
            else
            {
                // we are not using threshold.  Just make each output pixel the difference of the two input pixels
                fore.at<uchar>(col_index, row_index) = abs(old_gray.at<uchar>(col_index, row_index) - new_gray.at<uchar>(col_index, row_index));
            }
        }
    }    
    
    return fore; // Return the foreground calculated.
}

points_t get_foreground_points(Mat & old_mat, Mat & new_mat, int threshold_val)
{
    // NOTE: ROW:Y::COL:X
    // Convert images to gray
    Mat old_gray;
    Mat new_gray;
    cvtColor(old_mat, old_gray, CV_BGR2GRAY);
    cvtColor(new_mat, new_gray, CV_BGR2GRAY);
    points_t output; 
    int output_index = 0;
    
    for (int row_index = 0; row_index < new_gray.rows; row_index++)
    {
        // For each row in the new matrix
        for (int col_index = 0; col_index < new_mat.cols; col_index++)
        {
            // For each column in the new matrix
            // We are using a threshold.  If the difference of the pixels is less than the threshold value, color it white
            if (abs(old_gray.at<uchar>(col_index, row_index) - new_gray.at<uchar>(col_index, row_index)) > threshold_val)
            {
                // We have found a point in the foreground, put it in the output
                if (output_index == 65535)
                {
                    // We don't have any more space in the vector; exit early
                    /// @todo return some sort of error
                    return output;
                }
                output.push_back(Point(col_index, row_index));
                output_index++;
            }
        }
    }    

    return output; // Return the foreground calculated.
}


///////////////////////////////////////////////////////////////////////////////
/// draw_points will take in a points_t and source mat, then draw each of the
/// points in a new mat, of the same type and size as the source mat.  The new
/// mat is returned.
///
/// @param[in]      points      A points_t containing each of the points we 
///                             want to draw
/// @param[in]      input_matrix    A Mat that will be used to get the type and
///                             size of the output.
/// @return             A Mat type is returned that is the same type and size
///                     as the input_mat with each of the points colored at 255
///                     and everything else left black at 0.
///
///////////////////////////////////////////////////////////////////////////////
Mat draw_points(points_t points, Mat & input_matrix)
{
    Mat input_gray;
    cvtColor(input_matrix, input_gray, CV_BGR2GRAY);
    Point point_val;
    Mat output = Mat::zeros(input_gray.size(), input_gray.type()); // Create Output Mat
    for (unsigned int i = 0; i < points.size(); i++)
    {
        // For each point in the input points_t...
        // Draw the point in the output
        point_val = points.back();
        output.at<uchar>(point_val.x, point_val.y) = 255;
        points.pop_back();
    }
    
    return output;
    
}
void draw_box_on_foreground(Mat & input_mat, points_t points)
{
    // The plan for this function is to cycle through the points once.  We
    // will maintain a list of rectangles and single points.  For each point,
    // we pick one of the options below, starting with the first, and advancing
    // down the line if that step is not appropriate.
    // 1. If a point is close enough to another rectangle, we check to see if
    //    the point is inside the rectangle.  If it is inside the rectangle,
    //    we are done with this point.  If it is not inside the rectangle, we
    //    expand the rectangle to include it in both the x and y directions if
    //    necessary. 
    // 2. If a point is close enough to another point, we 
    //    a. draw a rectangle using the two points as opposite corners
    //    b. remove the first point from the "Single Points" list.
    //    c. add the rectangle to the "Rectangles" list.    
    // 3. If a point is not close enough to another point or a rectangle,
    //    we put it in the "Single Points" list.
    
    // What is the maximum distance a pixel can be from another pixel
    // before it is considered a different object?
    #define CLOSE_ENOUGH 20 // I'm calling it max_distance
    
    boxes_t boxes;
    int boxes_index = 0;
    points_t single_points;
    int single_points_index = 0;
    
    bool point_done = false;
    for (unsigned int i = 0; i < points.size(); i++)
    {
        // For each point in the input points_t...
        // Check to see if this point is close enough to another rectangle
        for (int j = 0; (j < boxes_index) && (!point_done); j++)
        {
            // Check to see if the point is CLOSE_ENOUGH to the box
            if (point_is_close_to_box(points[i], boxes[boxes_index], CLOSE_ENOUGH))
            {
                // If it is, 
                // expand the box to include the point,
                expand_box(points[i], boxes[boxes_index]);

                // set point_done to true to indicate we are done with this point
                point_done = true;
            }
        }
        // Check to see if this point is close enough to another point
        for (int j = 0; (j < single_points_index) && (!point_done); j++)
        {
            // Check to see if this point is CLOSE_ENOUGH to our point
            if (distance_between(points[i], single_points[single_points_index]) < CLOSE_ENOUGH)
            {
                // If it is,
                // create a box with these two points as the opposite corners
                // Push that box to the boxes vector
                boxes.push_back(Rect(points[i], single_points[single_points_index]));
                
                // erase this point from the points vector
                single_points.erase(single_points.begin() + single_points_index);
                
                // Set point_done to true to indicate we are done with this point
                point_done = true;
            }
        }
        // This point is not close enough to anything else
        if (!point_done)
        {
            // If we have not found a box to put our point in, or another single
            // point with which to make a box, this point is a single point. 
            // Put it in the single points vector
            
            // Push this point in the single points vector
            single_points.push_back(points[i]);
            single_points_index++;
        }
    }
    
    // Now that we have found the boxes, draw the boxes on the input image
    draw_boxes(boxes, input_mat);
}

bool point_is_close_to_box(Point point_val, Rect box, int close)
{
    // There are two possible cases we can encounter here.
    // The point can be inside (or on) the box, or the point can
    // be in any of the eight directions away from the box (think #).
    
    // First we should check to see if the point is inside the box.
    // We can do this by getting the four bounds of the box and checking
    // if the point is less than the max bound and greater than the min
    // bound.
    
    // Rect holds x,y referring to the top left corner, and height
    // and width fields.  With the above in mind, these are the bounds.
    // NOTE: Origin is in the top left, x increases to the right, y increases
    // to the bottom.
    int left_bound   = box.x;
    int right_bound  = box.x + box.width;
    int top_bound    = box.y;
    int bottom_bound = box.y + box.height;
    
    // Next we need to check if we are close to the box.
    // We need to find where the point is in relationship to the box and its
    // corners before we can figure this out.
    
    if (point_val.x < left_bound)
    {
        // We are on the left side of the box
        if (point_val.y > bottom_bound)
        {
            // We are in the bottom left corner.
            if (distance_between(point_val, Point(left_bound, bottom_bound)) < close) return true;
        }
        else if (point_val.y >= top_bound)
        {
            // We are on the left side.
            if (distance_between(point_val, Point(left_bound, point_val.y)) < close) return true;
        }
        else
        {
            // We are in the top left corner.
            if (distance_between(point_val, Point(left_bound, top_bound)) < close) return true;
        }
    }
    else if (point_val.x <= right_bound)
    {
        // We are in the box, on top of the box, or below the box.
        // Find out if we are in the box, or to the left or right
        if (point_val.y > bottom_bound)
        {
            // We are below the box.
            if (distance_between(point_val, Point(point_val.x, bottom_bound)) < close) return true;
        }
        else if (point_val.y >= top_bound)
        {
            // We are in the box.
            return true;
        }
        else
        {
            // We are above the box.
            if (distance_between(point_val, Point(point_val.x, top_bound)) < close) return true;
        }
    }
    else
    {
        // We are on the right side of the box.
        if (point_val.y > bottom_bound)
        {
            // We are in the borrom right corner.
            if (distance_between(point_val, Point(right_bound, bottom_bound)) < close) return true;
        }
        else if (point_val.y >= top_bound)
        {
             // We are on the right side.
             if (distance_between(point_val, Point(right_bound, point_val.y)) < close) return true;
        }
        else
        {
            // We are in the top right corner.
            if (distance_between(point_val, Point(right_bound, top_bound)) < close) return true;
        }
    }
    return false;
}

int distance_between(Point p1, Point p2)
{
    return sqrt((p2.x - p1.x) * (p2.x - p1.x)
              + (p2.y - p1.y) * (p2.y - p1.y));
}

void expand_box(Point point_val, Rect & box)
{
    // Rect.x, Rect.y - Top left corner of box
    // Rect.height - Height of box
    // Rect.width - Width of box
    // NOTE: Origing is top left, x increases to the right, y increases to the
    // bottom.
    
    if (point_val.x < box.x)
    {
        // Point is to the left of the current box
        box.width += (box.x - point_val.x);
        box.x = point_val.x;
    }
    else if (point_val.x > (box.x + box.width))
    {
        // Point is to the right of the current box
        box.width += (point_val.x - (box.x + box.width));
    }
    
    if (point_val.y < box.y)
    {
        // Point is above the current box
        box.height += (box.y - point_val.y);
        box.y = point_val.y;
    }
    else if (point_val.y > (box.y + box.height))
    {
        // Point is below the current box
        box.height += (point_val.y - (box.y + box.height));
    }
}

void draw_boxes(boxes_t boxes, Mat & mat)
{
    for (unsigned int i = 0; i < boxes.size(); i++)
    {
        rectangle(mat, 
        Point(boxes[i].x,boxes[i].y), 
        Point(boxes[i].x + boxes[i].width, boxes[i].y + boxes[i].height), 
        Scalar(255,255,255),
        3);
    }
}

void creating_box_test(Mat mat)
{
    // This is a test function used to test the creation and drawing
    // of boxes on a Mat image.
    
    // The main form of this test is to start with the top left point
    // of the input matrix, then sweep the box width out until it hits
    // the right edge of the Mat.  Then we will increase the box height
    // by one, and repeat until we hit the bottom right corner of the
    // Mat.
    
    // If we insert a delay into this process after drawing the box,
    // we should be able to see the sweeping progresion of the box
    // as it is drawn left to right, top to bottom.
    
    // Create a window
    namedWindow("Boxes Test", WINDOW_AUTOSIZE);

    // Boxes vector    
    boxes_t boxes;
    
    // Temporary mat for displaying boxes
    Mat temp;
    
    // Outer loop goes from top to bottom
    // Inner loop goes from left to right
    // We are assuming Point(0, 0) is the top left corner
    for (int j = 0; j < mat.rows; j++)
    {
        for (int i = 0; i < mat.cols; i++)
        {
            // Add the box to the vector
            // This is what we are trying to do
            //      boxes.push_back(Rect(Point(i,j), Point(0,0)));
            boxes.push_back(Rect(Point(0,0), Point(1,1)));
            expand_box(Point(i,j), boxes[0]);
            cout << "(" << boxes[0].x << ", " << boxes[0].y << ")  ";
            cout << "(" << boxes[0].x + boxes[0].width << ", " << boxes[0].y + boxes[0].height << ")" << endl;
            
            // Copy Mat
            mat.copyTo(temp);
            
            // Draw the box
            draw_boxes(boxes, temp);
            
            // Show the drawn box
            IMSHOW("Boxes Test", temp);
            
            // Remove the box from the vector
            boxes.pop_back();
            
            // Wait for 1ms
            //sleep_for_milliseconds(1);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// This is a test function used to test the creation and drawing
/// of boxes on a Mat.
/// 
/// The main form of this test is to start with the top left point
/// of the input matrix, and draw as many boxes as we can going 
/// down and right.  After each draw, we will increase the size
/// of the boxes and draw again until one box hits the bottom or
/// right side of the Mat.  Once this happens, we will begin decreasing
/// the size of the box in the direction that was hit.  We will continue
/// until the other direction hits the edge, then continue until the box
/// is too small to continue.
///
/// @param[in]  mat     A Mat object that is the same type and size of our
///                     output.
/// @param[in]  num_boxes_per_line  An int that stores the number of boxes
///                                 in each vert and horiz line.
///
///////////////////////////////////////////////////////////////////////////////
void creating_boxes_test(Mat mat, int num_boxes_per_line)
{

    
    // Create a window
    namedWindow("Boxes Test", WINDOW_AUTOSIZE);

    // Boxes vector    
    boxes_t boxes;
    
    // Box size
    int box_x = 1;
    int box_y = 1;
    
    // Box size direction
    bool box_x_increasing true;
    bool box_y_increasing true;
    
    // Temporary mat for displaying boxes
    Mat temp;
    
    // Outer loop goes from top to bottom
    // Inner loop goes from left to right
    // We are assuming Point(0, 0) is the top left corner
    while (true)
    {
        // check if the boxes will fit
        // Check x direction
        while ((box_x * num_boxes_per_line) > mat.cols)
        {
            // The box is too big in the x direction
            // Check if we are already as small as can be
            if (box_x < 2)
            {
                // The box is already as small as can be
                cout << "too many boxes in x direction!" << endl;
                break;
            }
            else
            {
                // The box can be smaller.
                // Change the x direction to false.
                box_x_increasing = false;
                // decrease the size by and check again
                box_x--;
            }
        }
        // Check y direction
        while ((box_y * num_boxes_per_line) > mat.rows)
        {
            // The box is too big in the y direction
            // Check if we are already as small as can be
            if (box_y < 2)
            {
                // The box is already as small as can be
                cout << "too many boxes in y direction!" << endl;
                break;
            }
            else
            {
                // The box can be smaller.
                // Change the y direction to false.
                box_y_increasing = false;
                // decrease the size by and check again
                box_y--;
            }
        }
        
        // Check if the boxes are too small
        // check x direction
        if (box_x == 0)
        {
            // Box is too small in x direction
            // Change box direction
            box_x_increasing = true;
            // increase box size
            box_x++;
        }
        // check y direction
        if (box_y == 0)
        {
            // Box is too small in y direction
            // Change box direction
            box_y_increasing = true;
            // increase box size
            box_y++;
        }
            
        // Draw boxes
        for (int i = 0; i < num_boxes_per_line; i++)
        {
            for (int j = 0; j < num_boxes_per_line; j++)
            {
                // i is the x direction of boxes
                // j is the y direction of boxes
                boxes.push_back(Rect(Point(i * box_x, j * box_y), Point((i + 1) * box_x, (i+1) * box_y)));
            }
        }
        
        // Copy Mat
        mat.copyTo(temp);
            
        // Draw the box
        draw_boxes(boxes, temp);
            
        // Show the drawn box
        IMSHOW("Boxes Test", temp);
            
        // Remove the box from the vector
        for (int i = num_boxes_per_line * num_boxes_per_line; i > 0; i--)
        {
            boxes.pop_back();
        }
        
        // Modify boxes        
        if (box_x_increasing)
        {
            box_x++;
        }
        else
        {
            box_x--;
        }
        
        if (box_y_increasing)
        {
            box_y++;
        }
        else
        {
            box_y--;
        }
    }
}
