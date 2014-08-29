// Build with:
// g++ -O2 `pkg-config --cflags --libs opencv` imagegrabber.cpp -o imagegrabber

#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main()
{

    // Open the default camera
	cout << "Open the default camera" << endl;
	VideoCapture cap(0);

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
	
	// Close the default camera
	cap.release();

	// DONE
	return 0;

}
