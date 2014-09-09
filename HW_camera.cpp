/******************************************************************************
* HW_camera.cpp
* This file contains functions that deal with operating the physical camera.
*
******************************************************************************/

/////////////////////////// STANDARD INCLUDES  ////////////////////////////////
/////////////////////////// PROJECT INCLUDES   ////////////////////////////////
#include "HW_camera.hpp"

/////////////////////////// EXPORTED FUNCTIONS ////////////////////////////////
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