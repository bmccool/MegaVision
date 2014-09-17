// Build with:
// g++ -O2 `pkg-config --cflags --libs opencv` imagegrabber.cpp -o imagegrabber

#include<iostream>
#include<opencv2/opencv.hpp>
#include <string>
#include <stdio.h>

#include "HW_camera.hpp"
#include "IP_ledge_detect.hpp"
using namespace std;
using namespace cv;

#define SLIDER_MAX 100

// Local Function Prototypes
int main_polling(VideoCapture *capture);
int main_looping(VideoCapture *capture, string);

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
            main_polling(&cap);
        }
        else if (string(argv[1]) == "l")
        {
            main_looping(&cap, string(argv[2]));
        }
    }

    // Close the default camera
    HW_camera_close(&cap);

    // DONE
    return 0;

}

int main_polling(VideoCapture *capture)
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

int main_looping(VideoCapture *capture, string)
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
}
