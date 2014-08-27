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


	// Store Image to matrix
	cout << "Store Image to matrix" << endl;
	Mat imgInput;
	capture >> imgInput;

	// Write the image
	cout << "Writing Image" << endl;
	imwrite("imgInput.png", imgInput);

	// DONE
	return 0;

}

