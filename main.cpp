// Include files for required libraries
#include <stdio.h>

#include "opencv_aee.hpp"
#include "main.hpp"     // You can use this file for declaring defined values and functions
#include "pi2c.h"
#include <stdio.h>
#include <stdlib.h>

Pi2c car(0x22); // Configure the I2C interface to the Car as a global variable

void setup(void)
{
    setupCamera(320, 240);  // Enable the camera for OpenCV
}

int main( int argc, char** argv )
{
    setup();    // Call a setup function to prepare IO and devices

    cv::namedWindow("Photo"); // Create a GUI window called photo

    int lowH = 0, highH = 255, lowS = 0, highS =255, lowV =0, highV = 40; //initialise the HSV limits
    int counts = 0;
    while(1)    // Main loop to perform image processing
    {
        Mat frame;
        Mat frameHSV;
        //Mat croppedIMG;
        counts++;


        while(frame.empty())
            frame = captureFrame(); // Capture a frame from the camera and store in a new matrix variable

        //croppedIMG = frameHSV(Rect(1,1,100,100));

        lowH = 0, highH = 255, lowS = 0, highS =255, lowV =0, highV = 40;

        cvtColor(frame, frameHSV, COLOR_BGR2HSV);
        inRange(frameHSV, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), frameHSV);

        //cv::putText(frame, "colour", Point(50,100), FONT_HERSHEY_COMPLEX, 1, Scalar(255,255,255), 1);
        //Pixels
        //LEFT pixels
        float Pvalue = 5;
        float Ivalue = 0;
        float Dvalue = 0;
        char Direction[1];
        int speed;

        int L100 = 100, L110 = 110, L120=120, L130 = 130, L140 = 140, L150 = 150;
        bool L100on = false, L110on = false, L120on = false, L130on = false, L140on = false, L150on = false;
        //Center Pixels
        int C160 = 160;
        bool C160on = false;
        //RIGHT pixels
        int R170 = 170, R180 = 180, R190 = 190, R200 = 200, R210 = 210, R220 = 220;
        bool R170on = false, R180on = false, R190on = false,R200on = false, R210on = false, R220on = false;

        float WeightedTotal = 0, SensorTotal = 0, PreviousError = 0, CurrentError;
        float Proportional, Integral = 0, Differential, PIDoutput;


        Mat comparison;

        uchar* p = frameHSV.ptr<uchar>(150);
        for(int x = 0; x < frameHSV.cols; x++)
        {

            PreviousError = CurrentError;
            WeightedTotal = 0;
            SensorTotal = 0;

            p[x];

            uchar pixel = p[x];

            if(p[L100] != 0)
            {
                L100on = true;
            }

            if(p[L110] != 0)
            {
                L110on = true;
            }

            if(p[L120] != 0)
            {
                L120on = true;
            }

            if(p[L130] != 0)
            {
                L130on = true;
            }

            if(p[L140] != 0)
            {
                L140on = true;
}
            if(p[L150] != 0)
            {
                L150on = true;
            }

            if(p[C160] != 0)
            {
                C160on = true;
            }

            if(p[R170] != 0)
            {
                R170on = true;
            }

            if(p[R180] != 0)
            {
                R180on = true;
            }

            if(p[R190] != 0)
            {
                R190on = true;
            }

            if(p[R200] != 0)
            {
                R200on = true;
            }

            if(p[R210] != 0)
            {
                R210on = true;
            }

            if(p[R220] != 0)
            {
                R220on = true;
            }

            int Preading[13] = {L100on, L120on, L130on, L140on,L150on,C160on, R170on, R180on, R190on, R200on, R210on, R220on};

            int Pweight[13] = {-60, -50, -40, -30, -20, -10, 0, 10, 20, 30, 40, 50, 60};

            for(int i = 0; i<13; i++)
            {
                WeightedTotal += Pweight[i] * Preading[i];
                SensorTotal += Preading[i];
            }

            CurrentError = WeightedTotal/SensorTotal;

            Proportional = CurrentError;
            Integral += CurrentError;
            Differential = CurrentError - PreviousError;

            PIDoutput = Proportional*Pvalue + Integral*Ivalue + Differential*Dvalue;

//            if(SensorTotal == 0)
//            {
//                Direction [0] = 'S';
//                speed = 0;
//            }

            if (PIDoutput > 15)
            {
                Direction[0] = 'L';
                speed = PIDoutput;
            }

            else if(PIDoutput < -15)
            {
                Direction[0] = 'R';
                speed = -PIDoutput;
            }

            else
            {
                Direction[0] = 'N';
                speed = 0;
            }


        //printf("%d -- %d -- %d -- %d -- %d -- %d -- %d  Direction %c  speed %f\n", L130on, L140on ,L150on ,C160on, R170on, R180on, R190on, Direction[0], PIDoutput);
        //printf("\033[H\033[J");

        }


        int output = PIDoutput;

        int i;


//        car.i2cWriteArduinoInt(100);
//        car.i2cWriteArduinoInt(200);

        lowH = 150, highH = 180, lowS = 50, highS =255, lowV =50, highV = 255;

        cvtColor(frame, frameHSV, COLOR_BGR2HSV);
        inRange(frameHSV, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), frameHSV);


        printf("%d \n",output);
        car.i2cWriteArduinoInt(output);
        printf("Sent");


        //printf("%d \n", counts);


        cvtColor(frameHSV, frameHSV, COLOR_GRAY2BGR);
        hconcat(frame, frameHSV, comparison);

        cv::imshow("Photo", comparison); //Display the image in the window=

        int key = cv::waitKey(1);   // Wait 1ms for a keypress (required to update windows)

        key = (key==255) ? -1 : key;    // Check if the ESC key has been pressed
        if (key == 27)
            break;
	}

	closeCV();  // Disable the camera and close any windows

	return 0;
}
