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
int HW_camera_open(cv::VideoCapture *capture)
{
    // Open the default camera
    std::cout << "Open the default camera" << std::endl;
    capture->open(0);

    // Check the width
    std::cout << "Check the width... ";
    double width = capture->get(CV_CAP_PROP_FRAME_WIDTH);
    std::cout << "Width = " << width << std::endl;
    
    // Check the height
    std::cout << "Check the height... ";
    double height = capture->get(CV_CAP_PROP_FRAME_HEIGHT);
    std::cout << "Height = " << height << std::endl;
    
    // Check that the camera was opened
    std::cout << "Check that the camera was opened... ";
    if (!capture->isOpened())
    {
        std::cout << "The camera is not working!" << std::endl;
        return ERR_CAMERA_OPEN_FAILED;
    }
    else
    {
        std::cout << "Success!" << std::endl;
        return ERR_NONE;
    }
}

/******************************************************************************
* Function Name: HW_camera_close
* 
*
******************************************************************************/
int HW_camera_close(cv::VideoCapture *capture)
{
    capture->release();
    return ERR_NONE;
}

/******************************************************************************
* Function Name: HW_camera_get_height
* 
*
******************************************************************************/
double HW_camera_get_height(cv::VideoCapture *capture)
{
    return capture->get(CV_CAP_PROP_FRAME_HEIGHT);
}

/******************************************************************************
* Function Name: HW_camera_get_height
* 
*
******************************************************************************/
double HW_camera_get_width(cv::VideoCapture *capture)
{
    return capture->get(CV_CAP_PROP_FRAME_WIDTH);
}