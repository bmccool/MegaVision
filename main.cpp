// Build with:
// g++ -O2 `pkg-config --cflags --libs opencv` imagegrabber.cpp -o imagegrabber

#include<iostream>
#include<opencv2/opencv.hpp>
#include <string>
#include <stdio.h>
#include <chrono>
#include <thread>

#include "HW_camera.hpp"
#include "IP_ledge_detect.hpp"
#include "CS_time.hpp"
using namespace std;
using namespace cv;

#define MIN_LINE_LENGTH 10
#define MAX_LINE_GAP 5
#define THRESHOLD_VAR 35



// Local Function Prototypes
void main_polling(VideoCapture & capture);
int main_looping(VideoCapture & capture, string filename);
void main_continuous(VideoCapture & capture);
void main_wiggle(void);


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
        else if (string(argv[1] == "w")
        {
            main_wiggle();
        }
    }

    // Close the default camera
    HW_camera_close(&cap);

    // DONE
    return 0;

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

void main_wiggle(void)
{
    unsigned long int nes_window = get_nes_window();
    while (true)
    {
        
        send_key_down("Left", nes_window);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        send_key_up("Left", nes_window);
        send_key_down("Right", nes_window);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        send_key_up("Right", nes_window);
    }
}
    