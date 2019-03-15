// Include files for required libraries
#include <iostream>
#include "opencv_aee.hpp"
#include "main.hpp"     // You can use this file for declaring defined values and functions
#include "pi2c.h"

Pi2c car(0x22); // Configure the I2C interface to the Car as a global variable
Pi2c sensor(0x23);

using namespace std;

void setup(void)
{
    setupCamera(320, 240);  // Enable the camera for OpenCV
}

Mat GetFrameHSVForBlack (const Mat frame)
{
    Mat frameHSV;
    int lowH = 0, highH = 255, lowS = 0, highS = 255, lowV = 0, highV = 40; //initialise the HSV limits

    cvtColor(frame, frameHSV, COLOR_BGR2HSV);
    inRange(frameHSV, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), frameHSV);

    return frameHSV;
}

Mat GetFrameHSVForPinkSquare (const Mat frame)
{
    Mat frameHSV;
    int lowH = 150, highH = 160, lowS = 50, highS = 255, lowV = 50, highV = 255;

    cvtColor(frame, frameHSV, COLOR_BGR2HSV);
    inRange(frameHSV, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), frameHSV);

    return frameHSV;
}

void CalculateSpeedAndDirectionOfLine (Mat frameHSV, int& Speed, unsigned char & Direction)
{
    int x;

    uchar* p = frameHSV.ptr<uchar>(120);
    int DistanceFromLeft;
    for(x = 0, DistanceFromLeft = 0; x < frameHSV.cols; x++)
    {
        DistanceFromLeft++;

        p[x];

        uchar pixel = p[x];

        if(p[x] == 255)
            break;
    }

    uchar* q = frameHSV.ptr<uchar>(120);
    int DistanceFromRight;
    for(x = 319, DistanceFromRight = 0; x >= 0; x--)
    {
        DistanceFromRight++;

        q[x];

        uchar pixel = q[x];

        if(q[x] == 255)
            break;
    }

//    cout<<DistanceFromLeft<<"\t"<<DistanceFromRight<<"\n";

    if((DistanceFromLeft == 320 && DistanceFromRight == 320) || (DistanceFromLeft == 1 && DistanceFromRight == 1))
    {
        Direction = 'N';
        Speed = 0;
    }

    else if (DistanceFromLeft == 1)
    {
        Direction = 'L';
        Speed = 255 + 256;
    }

    else if (DistanceFromRight == 1)
    {
        Direction = 'R';
        Speed = 255;
    }

    else if(DistanceFromLeft > DistanceFromRight)
    {
        Direction = 'R';
        Speed = DistanceFromLeft - DistanceFromRight;

        Speed = (255.0/320.0)*Speed;
    }

    else if (DistanceFromRight > DistanceFromLeft)
    {
        Direction = 'L';
        Speed = DistanceFromRight - DistanceFromLeft;
        Speed = (255.0/320.0)*Speed + 256;
    }

//    cout<<"\n"<<Direction<<"   "<<(int)Speed;
}

void TiltCameraUp()
{
    if (sensor.i2cWriteArduinoInt(90) < 0)
        cout<<"Error in sending speed\n";
}

void TiltCameraDown()
{
    if (sensor.i2cWriteArduinoInt(20) < 0)
        cout<<"Error in sending speed\n";
}

void DetectSymbol()
{

}

int main( int argc, char** argv )
{
    setup();    // Call a setup function to prepare IO and devices

    cv::namedWindow("Photo");   // Create a GUI window called photo

    while(1)    // Main loop to perform image processing
    {
        Mat frame;
        while(frame.empty())
            frame = captureFrame(); // Capture a frame from the camera and store in a new matrix variable

        Mat frameBlackHSV = GetFrameHSVForBlack(frame);

        int Speed;
        unsigned char Direction;
        CalculateSpeedAndDirectionOfLine(frameBlackHSV, Speed, Direction);

        if (car.i2cWriteArduinoInt(Speed) < 0)
            cout<<"Error in sending speed\n";

        Mat framePinkSquareHSV = GetFrameHSVForPinkSquare(frame);
        int NonZeroPinkSquare = countNonZero(framePinkSquareHSV);

//        cout<<NonZeroPinkSquare<<"\n";

        if (NonZeroPinkSquare >= 10000)
        {
            Speed = 0;
            if (car.i2cWriteArduinoInt(Speed) < 0)
                cout<<"Error in sending speed\n";
            while(1);
        }


        cv::imshow("Photo", frame); //Display the image in the window




        int key = cv::waitKey(1);   // Wait 1ms for a keypress (required to update windows)
        key = (key==255) ? -1 : key;    // Check if the ESC key has been pressed
        if (key == 27)
            break;
    }

    closeCV();  // Disable the camera and close any windows

    return 0;
}



