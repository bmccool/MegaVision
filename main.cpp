#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp>
#include <string>
#include <stdio.h>

#include "HW_camera.hpp"
#include "IP_ledge_detect.hpp"
#include "CS_time.hpp"
#include "CT_window_manager.hpp"

using namespace std;
using namespace cv;

#define MIN_LINE_LENGTH 10
#define MAX_LINE_GAP 5
#define THRESHOLD_VAR 35
#define SHIFT_AMOUNT 7
#define FOREGROUND_THRESHOLD 40



// Local Function Prototypes
void main_polling(VideoCapture & capture);
int main_looping(VideoCapture & capture, string filename);
void main_continuous(VideoCapture & capture);
void main_wiggle(VideoCapture & capture);
void main_wobble_old(VideoCapture & capture);
void main_wobble(VideoCapture & capture);
void main_detect(VideoCapture & capture);
void main_box_test(VideoCapture & capture);
void main_boxes_test(VideoCapture & capture);

int main(int argc, char* argv[])
{
    VideoCapture cap;  // Capture Object

    // Open the camera
    HW_camera_open(&cap);

    // Check the incoming arguments
    if (argc < 2)
    {
        // No arguments were passed
        // Grab an image and exit
        
        // Get a new frame from the camera
        cout << "Get a new frame from the camera" << endl;
        Mat frame;
        cap >> frame;

        // Write the image
        cout << "Writing Image" << endl;
        imwrite("CameraFrame.png", frame);
    }
    else
    {
        if (string(argv[1]) == "p")
        {
            main_polling(cap);
        }
        else if (string(argv[1]) == "l")
        {
            main_looping(cap, string(argv[2]));
        }
        else if (string(argv[1]) == "c")
        {
            main_continuous(cap);
        }
        else if (string(argv[1]) == "w")
        {
            main_wiggle(cap);
        }
        else if (string(argv[1]) == "ww")
        {
            main_wobble(cap);
        }
        else if (string(argv[1]) == "d")
        {
            main_detect(cap);
        }
        else if (string(argv[1]) == "boxes_test")
        {
            main_boxes_test(cap);
        }        
    }

    // Close the default camera
    HW_camera_close(&cap);

    // DONE
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
/// main_box_test will test the functions required to create and draw boxes
/// by calling the function creating_box_test().
///
/// @param[in]      capture    A VideoCapture objec that is opened and tied
///                            to the webcam.
///////////////////////////////////////////////////////////////////////////////
void main_box_test(VideoCapture & capture)
{
    Mat frame;
    
    capture >> frame;
    while (true)
    {
        creating_box_test(fore);
    }
}   

///////////////////////////////////////////////////////////////////////////////
/// main_boxes_test will test the functions required to create and draw boxes
/// by calling the function creating_box_test().
///
/// @param[in]      capture    A VideoCapture objec that is opened and tied
///                            to the webcam.
///////////////////////////////////////////////////////////////////////////////
void main_boxes_test(VideoCapture & capture)
{
    Mat frame;
    int num_boxes_per_line = 5;  // This is the number of boxes in each
                                 // vertical and horizontal line.
    capture >> frame;
    while (true)
    {
        creating_boxes_test(fore, num_boxes_per_line);
    }
}   


void main_polling(VideoCapture & capture)
{
    // Polling mode
    // This mode will poll the user for a number
    // then take that many number of pictures.
    // This mode is intended to get samples of
    // frames that we can use when designing the
    // processing component of the project.
    int num_frames = 0;
    char OutString[30];
    int i;
    Mat frame;
    do
    {
        cout << "Select number of frames: ";
        cin  >> num_frames;
                
        for (i = 0; i < num_frames; i++)
        {
            capture >> frame;
            sprintf(OutString, "FrameGrab_%d.png", i);
            imwrite(OutString, frame);
        }
    }while(num_frames > 0);
}

int main_looping(VideoCapture & capture, string filename)
{
    // Read the input image from file
    cout << "Read the input image from file" << endl;
    Mat ReadFrame = imread(filename, 0);
    // Verify imread is successful
    if (!ReadFrame.data)
    {
        cout <<  "Could not open or find the image" << endl;
        return -1;
    }
            
    // Var mat to hold the output image
    Mat output_mat;
    // Buffer for output filename
    char OutString[50];

    // Line Transform Parameters
    int max_line_length = 10;
    int max_line_gap = 5;
    int threshold_var = 5;

    for (max_line_length = 10; max_line_length < 100; max_line_length += 10)
    {
        for (max_line_gap = 5; max_line_gap < 100; max_line_gap += 10)
        {
            for (threshold_var = 5; threshold_var < 100; threshold_var += 10)
            {
                // Detect the lines in this frame
                detect_lines_hough_prob(ReadFrame, max_line_length, max_line_gap, threshold_var, output_mat);
                // Create the output filename
                sprintf(OutString, "length %d gap %d thresh %d.png", max_line_length, max_line_gap, threshold_var);
                // Write the image
                imwrite(OutString, output_mat);
            }
        }
    }
    return 0;
}

void main_continuous(VideoCapture & capture)
{
    start_timer();
    Mat frame, output_mat;
    char window_name[60];
    sprintf(window_name, "len %d, gap %d, thresh %d", MIN_LINE_LENGTH, MAX_LINE_GAP, THRESHOLD_VAR);
    namedWindow(window_name, WINDOW_AUTOSIZE);
    
    while (get_elapsed_time() < 600)
    {
        // Get a new frame
        capture >> frame;
    
        // Find ledges
        detect_lines_hough_prob(frame, MIN_LINE_LENGTH, MAX_LINE_GAP, THRESHOLD_VAR, output_mat);
    
        // Create draw the output to screen
        imshow(window_name, output_mat);
        
        // Wait for 1 ms (not waiting will cause the image to not be displayed, known imshow issue)
        waitKey(1);
    }
}

void main_wiggle(VideoCapture & capture)
{
    char Right[10] = "Right";
    char Left[10] = "Left";
    int test_length = 50;
    Mat frame, output_frame;
    Mat blank_frame = Mat::zeros(HW_camera_get_height(&capture), HW_camera_get_width(&capture), CV_8UC3 );
    contours_t contours_new, contours_old;
    string last_pressed_button;
    
    // Grab the id of the nes widow
    unsigned long int nes_window = get_nes_window();
    
    // Create a window
    namedWindow("window 1", WINDOW_AUTOSIZE);
    
    // Get a base contour to compare to
    capture >> frame;
    contours_old = find_contours(frame);
    
    // Loop
    start_timer();
    last_pressed_button = "Right";
    while (get_elapsed_time() < 999)
    {
        if (last_pressed_button == "Right")
        {
            // Press left 
            send_key(Left, KEY_PRESS, nes_window);
            sleep_for_milliseconds(test_length);
            send_key(Left, KEY_RELEASE, nes_window);
        
            // Pause
            sleep_for_milliseconds(test_length);
         
            // Show a frame
            capture >> frame;
            contours_new = find_contours(frame);
            contours_new = remove_duplicate_contours(contours_old, contours_new);
            draw_contours(frame, output_frame, contours_new);
            imshow("window 1", output_frame);
            waitKey(1);
            
            // Set last pressed button
            last_pressed_button = "Left";
        }
        else if (last_pressed_button == "Left")
        {
            // Press right for 20 mils
            send_key(Right, KEY_PRESS, nes_window);
            sleep_for_milliseconds(test_length);
            send_key(Right, KEY_RELEASE, nes_window);
        
            // Pause
            sleep_for_milliseconds(test_length);
        
            // Show a frame
            capture >> frame;
            contours_new = find_contours(frame);
            contours_new = remove_duplicate_contours(contours_old, contours_new);
            draw_contours(frame, output_frame, contours_new);
            imshow("window 1", output_frame);
            waitKey(1);
            
            // Set last pressed button
            last_pressed_button = "Right";            
        }
    }
}
    
void main_wobble_old(VideoCapture & capture)
{
    cout << "main_wobble" << endl;
    Mat frame, frame_old, back, fore;
    
    //BackgroundSubtractorMOG2 bg;
    //bg.nmixtures = 3;
    //bg.bShadowDetection = false;
    contours_t contours;

    // Create a window
    namedWindow("Foreground", WINDOW_AUTOSIZE);
    
    capture >> frame;
    int pixels;
    // Loop
    while (true)
    {

        frame.copyTo(frame_old);
        capture >> frame;
        pixels = sidescroll_right_gray(frame_old, frame);
        if (pixels > 0)
        {
            cout << "The image has advanced " << pixels << " pixels." << endl;
            frame_old = shiftFrame(frame_old, pixels, ShiftLeft);
        }
        else if (pixels == -1)
        {
            cout << "no matches found." << endl;
        }
        else if (pixels < -1)
        {
            cout << "something went wrong." << endl;
        }
        //bg.operator ()(frame_old, fore);
        //bg.operator ()(frame,fore);
        //bg.getBackgroundImage(back);
        fore = get_fore(frame_old, frame);
        erode(fore,fore,Mat());
        dilate(fore,fore,Mat());
        //findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
        //drawContours(frame,contours,-1,cv::Scalar(0,0,255),2);
        IMSHOW("Foreground",fore);
    }
}   

void main_wobble(VideoCapture & capture)
{
    cout << "main_wobble" << endl;
    Mat frame, frame_old, back, fore;
    points_t points;
    
    contours_t contours;

    // Create a window
    namedWindow("Foreground", WINDOW_AUTOSIZE);
    
    capture >> frame;
    int pixels;
    // Loop
    while (true)
    {

        frame.copyTo(frame_old);
        capture >> frame;
        pixels = sidescroll_right_gray(frame_old, frame);
        if (pixels > 0)
        {
            cout << "The image has advanced " << pixels << " pixels." << endl;
            frame_old = shiftFrame(frame_old, SHIFT_AMOUNT, ShiftLeft);
        }
        else if (pixels == -1)
        {
            cout << "no matches found." << endl;
        }
        else if (pixels < -1)
        {
            cout << "something went wrong." << endl;
        }

        //fore = get_foreground(frame_old, frame, FOREGROUND_THRESHOLD);
        points = get_foreground_points(frame_old, frame, FOREGROUND_THRESHOLD);
        //erode(fore,fore,Mat());
        //dilate(fore,fore,Mat());
        //findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
        //drawContours(frame,contours,-1,cv::Scalar(0,0,255),2);
        fore = draw_points(points, frame);
        draw_box_on_foreground(fore, points);
        IMSHOW("Foreground",fore);
        //creating_box_test(fore);
    }
}   

///////////////////////////////////////////////////////////////////////////////
/// main_detect will attempt to draw a box around a single object moving on the
/// screen.  At this point, we are only going for one item moving on the
/// screen.  WIth more than one object, behavior is not defined.
///
/// @param[in]      capture    A VideoCapture objec that is opened and tied
///                            to the webcam.
///////////////////////////////////////////////////////////////////////////////
void main_detect(VideoCapture & capture)
{
    cout << "main_detect" << endl;
    Mat frame, frame_old; // Hold two successive frames for background subtraction
    Mat fore;             // Holds the foreground (moving parts) of the image
    points_t points;      // Stores the points that make up the foreground

    // Create a window
    namedWindow("Foreground", WINDOW_AUTOSIZE);
    
    // Grab the first frame so we have history inside the loop
    capture >> frame;

    // Loop
    while (true)
    {
        // Copy the last frame to frame_old
        frame.copyTo(frame_old);
        
        // Get a new frame to compare with frame_old
        capture >> frame;
        
        // Get the points that make up the foreground
        points = get_foreground_points(frame_old, frame, FOREGROUND_THRESHOLD);
        
        // Draw the foreground points to the foreground matrix
        fore = draw_points(points, frame);
        
        // Draw a box fitting the foreground
        draw_box_on_foreground(fore, points);
        
        // Display the foreground with the box around it
        IMSHOW("Foreground",fore);
    }
}   
