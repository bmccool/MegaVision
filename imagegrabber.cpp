// Build with:
// g++ -O2 `pkg-config --cflags --libs opencv` imagegrabber.cpp -o imagegrabber

#include<iostream>
#include<opencv2/opencv.hpp>
#include <string>
#include <time.h>
using namespace std;
using namespace cv;

int main()
{

    // Open the default camera
	cout << "Open the default camera" << endl;
	VideoCapture cap(0);

	// Check the width
	cout << "Check the width... ";
	double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	cout << "Width = " << width << endl;
	
	// Check the height
	cout << "Check the height... ";
	double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Height = " << height << endl;
	
	// Check that the camera was opened
	cout << "Check that the camera was opened... ";
	if (!cap.isOpened())
	{
		cout << "The camera is not working!" << endl;
		return -1;
	}
	else
	{
	    cout << "Success!" << endl;
	}
		

	// Get a new frame from the camera
	cout << "Get a new frame from the camera" << endl;
	Mat frame;
	cap >> frame;

	// Write the image
	cout << "Writing Image" << endl;
	imwrite("CameraFrame.png", frame);
	
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
	cap.release();

	// DONE
	return 0;

}
