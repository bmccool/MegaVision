// Build with:
// g++ -O2 `pkg-config --cflags --libs opencv` imagegrabber.cpp -o imagegrabber

#include<iostream>
#include<opencv2/opencv.hpp>
#include <string>
#include <time.h>
using namespace std;
using namespace cv;

#define CAM_WIDTH 640
#define CAM_HEIGHT 480

int main()
{

    // Open the default camera
	cout << "Open the default camera" << endl;
	VideoCapture cap(0);

	// Set the width
	cout << "Set the width" << endl;
	cap.set(CV_CAP_PROP_FRAME_WIDTH, CAM_WIDTH);
	
	// Set the height
	cout << "Set the height" << endl;
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAM_HEIGHT);
	
	// Check that we succeeded
	cout << "Check that we succeeded" << endl;
	if (!cap.isOpened())
	{
		cout << "The camera is not working!" << endl;
		return -1;
	}


	// Get a new frame from the camera
	cout << "Get a new frame from the camera" << endl;
	Mat frame;
	cap >> frame;

	// Write the image
	cout << "Writing Image" << endl;
	imwrite("CameraFrame.png", frame);
	
	// Start timing the camera.  We want to see
	// how quickly we can grab images from the camera
	// in quick succession.
	const clock_t begin_time = clock();
	string OutString = "CameraFrameX.png";
	for (char c = '1'; c <= '9'; c++)
	{
	    cout << "Getting new frame..." << endl;
	    cap >> frame;
		
		OutString[11] = c;
		cout << "Writing Image:" << OutString << endl;
		imwrite(OutString, frame);
	}	
	cout << float( clock() - begin_time) / CLOCKS_PER_SEC;
	// Close the default camera
	cap.release();

	// DONE
	return 0;

}
