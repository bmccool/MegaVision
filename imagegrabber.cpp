// Build with:
// g++ -O2 `pkg-config --cflags --libs opencv` imagegrabber.cpp -o imagegrabber

#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;


// These were the threshold values that were
// used for image thresholding.  They are 
// thresholds for blue.
int low [3] =  {0, 21, 66};
int high[3] = {61, 50, 95};

// TODO: Make sure these are the correct resolution for our camera
double width = 176;
double height = 144;

int main()
{

	cout << "VideoCapture capture(0);" << endl;
	VideoCapture capture(0);
	cout << "capture.set(CV_CAP_PROP_FRAME_WIDTH, width);" << endl;
	capture.set(CV_CAP_PROP_FRAME_WIDTH, width);
	cout << "capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);" << endl;
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);

	if (!capture.isOpened())
	{
		cout << "The camera is not working!" << endl;
	}


	// Grab the Image
	cout << "Grab the Image" << endl;
	Mat imgInput, imgHSV;
	capture >>imgInput;

	// Convert from BGR to HSV ColorSpace
	cout << "Convert from BGR to HSV ColorSpace" << endl;
	cvtColor(imgInput, imgHSV, CV_BGR2HSV);

	// Split into color arrays
	cout << "Split into color arrays" <<endl;
	Mat planes[3];
	split(imgHSV, planes);

	// Threshold the image
	cout << "Threshold the image" << endl;
	for (int i = 0; i < 3; i++)
	{
		Mat imgThreshLow, imgThreshHigh;
		threshold(planes[i], imgThreshLow, low[i], 255, THRESH_BINARY);
		threshold(planes[i], imgThreshHigh, high[i], 255, THRESH_BINARY);
		bitwise_and(imgThreshLow, imgThreshHigh, planes[i]);
	}

	// Create the final result
	cout << "Create the final result" << endl;
	Mat imgResult(Size(width, height), CV_8UC1, Scalar(255));
	for (int i = 0; i < 3; i++)
	{
		cout << "Looping through image creation..." << endl;
		bitwise_and(imgResult, planes[i], imgResult);
	}

	// Write the images
	cout << "Writing Images" << endl;
	imwrite("imgInput.png", imgInput);
	imwrite("imgHSV.png", imgHSV);
	imwrite("imgResult.png", imgResult);

	// DONE
	return 0;

}

