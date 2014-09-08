// Build with:
// g++ -O2 `pkg-config --cflags --libs opencv` imagegrabber.cpp -o imagegrabber

#include<iostream>
#include<opencv2/opencv.hpp>
#include <string>
#include <time.h>
using namespace std;
using namespace cv;

#define ERR_NONE				0
#define ERR_CAMERA_OPEN_FAILED 	1

int HW_camera_open(VideoCapture *capture);
int HW_camera_close(VideoCapture *capture);

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
	    if (argv[1] == "p")
		{
		    // Polling mode
			// This mode will poll the user for a number
			// then take that many number of pictures.
			// This mode is intended to get samples of
			// frames that we can use when designing the
			// processing component of the project.
			int num_frames = 0;
			string OutString = "CameraFrameX.png";
			do
			{
			    cout << "Select number of frames between 1 and 9: ";
				cin  >> num_frames;
				
	            for (int i = 1, char c = '1'; i < num_frames; i++, c++)
	            {
	                cap >> frame;
		            OutString[11] = c;
		            imwrite(OutString, frame);
				}
			}while(num_frames > 0);
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
	
#endif	// TIME CAMERA
	
	// Close the default camera
    HW_camera_close(&cap);

	// DONE
	return 0;

}
/******************************************************************************
* Function Name: HW_camera_open
* 
*
******************************************************************************/
int HW_camera_open(VideoCapture *capture)
{
    // Open the default camera
	cout << "Open the default camera" << endl;
	capture->open(0);

	// Check the width
	cout << "Check the width... ";
	double width = capture->get(CV_CAP_PROP_FRAME_WIDTH);
	cout << "Width = " << width << endl;
	
	// Check the height
	cout << "Check the height... ";
	double height = capture->get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Height = " << height << endl;
	
	// Check that the camera was opened
	cout << "Check that the camera was opened... ";
	if (!capture->isOpened())
	{
		cout << "The camera is not working!" << endl;
		return ERR_CAMERA_OPEN_FAILED;
	}
	else
	{
	    cout << "Success!" << endl;
		return ERR_NONE;
	} 
}

/******************************************************************************
* Function Name: HW_camera_close
* 
*
******************************************************************************/
int HW_camera_close(VideoCapture *capture)
{
	capture->release();
	return ERR_NONE;
}
