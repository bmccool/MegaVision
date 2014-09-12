// Build with:
// g++ -O2 `pkg-config --cflags --libs opencv` imagegrabber.cpp -o imagegrabber

#include<iostream>
#include<opencv2/opencv.hpp>
#include <string>
#include <time.h>
#include <stdio.h>

#include "HW_camera.hpp"
#include "IP_ledge_detect.hpp"
using namespace std;
using namespace cv;

#define SLIDER_MAX 100

// Global Variables
int max_line_length = 10;
int max_line_gap = 5;
int threshold = 5;
Mat ReadFrame

void trackbar_callback(void)
{
    Mat dst;
    detect_lines(ReadFrame, max_line_length, max_line_gap, threshold, dst);
    imshow("Line Transform", dst);
}


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
                    cap >> frame;
                    sprintf(OutString, "FrameGrab_%d.png", i);
                    imwrite(OutString, frame);
                }
            }while(num_frames > 0);
        }
        else if (string(argv[1]) == "l")
        {
            ReadFrame = imread(string(argv[2]), 0);


            // Create Window
            namedWindow("Line Transform", 1);
            
            // Create Trackbars
            createTrackbar("Max Line Length", "Line Transform", &max_line_length, SLIDER_MAX, trackbar_callback);
            createTrackbar("Max Line Gap", "Line Transform", &max_line_gap, SLIDER_MAX, trackbar_callback);
            createTrackbar("Threshold", "Line Transform", &threshold, SLIDER_MAX, trackbar_callback);
        }
    }





#if 0 // TIME CAMERA
    // Start timing the camera.  We want to see
    // how quickly we can grab images from the camera
    // in quick succession.  This will grab ten frames
    // from the camera and print the amount of time it
    // took.  Take the number of frames (10) divided by
    // the time to get frames per second.
    const clock_t begin_time = clock();
    string OutString = "CameraFrameX.png";
    for (char c = '1'; c <= '9'; c++)
    {
        cap >> frame;
        OutString[11] = c;
        imwrite(OutString, frame);
    }
    cout << float( clock() - begin_time) / CLOCKS_PER_SEC;
    
#endif // TIME CAMERA

    // Close the default camera
    HW_camera_close(&cap);

    // DONE
    return 0;

}

