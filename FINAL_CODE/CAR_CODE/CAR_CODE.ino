///////////////////////////////////////////////////////////////
//                                                           // 
// H61AEE: Dept. EEE, The University of Nottingham 2018      //
// Author: D. Fallows                                        //
//                                                           //
// CAR_Template.ino: Vehicle Template Sketch                 //
//                   Session 4 - RF Remote Control           //
//                                                           // 
///////////////////////////////////////////////////////////////

// Libraries
#include <Wire.h>
#include <H61AEE_S02.h>
#include "CAR_Template.h"



int xValue =0 , yValue = 0;
void setup()
{
  car.setupVehicle(); // configure car library
  car.enableMotors(true); // Disable motors so they dont move
  
  Wire.begin(CAR);              // join i2c bus as the car
  Wire.onReceive(receiveEvent); // register function to be called whenever data is received
  //register function to be called when data needs to be transmitted
  
  Serial.begin(9600);           // start serial for output

  pinMode(YELLOW_LED, OUTPUT);   // Configure LED outputs
  pinMode(RED_LED, OUTPUT);


  /* TODO - Configure IO and libraries as required */
}

void loop()
{
//  int yValue = 700;
//  int xValue = 510;
//  
  int yValueMax = 887;
  int yValueRest = 498; //Give a +/- 4 on this
  int yValueMin = 138;
  
  int xValueMax = 875;
  int xValueRest = 510; //Give a +/- 4 on this
  int xValueMin = 88;
  
  float speedofcar;
  float directionofcar;

  
  
  
  Serial.print(speedofcar);
  
  if (yValue > (yValueRest+4))
  {
    float a = yValueMax-yValueRest-4;
    float b = yValue-yValueRest-4;
    speedofcar = (255/(a))*(b);
     
    car.setDirection(LEFT, forwards);
    car.setDirection(RIGHT, forwards);
    car.setSpeed(LEFT, speedofcar);
    car.setSpeed(RIGHT, speedofcar);
  }

   else if (yValue < (yValueRest-4))
  {
    float a = yValueRest+4-yValueMin;
    float b = yValueRest+4-yValue;
    speedofcar = (255/(a))*(b);
    car.setDirection(LEFT, backwards);
    car.setDirection(RIGHT, backwards);
    car.setSpeed(LEFT, speedofcar);
    car.setSpeed(RIGHT, speedofcar);
  }


//if functions for right joystick that moves tha car left or right

   if (xValue > (xValueRest+4))
  {
    
    float a = xValueMax-xValueRest-4;
    float b = xValue-xValueRest-4;
    directionofcar = (255/(a))*(b);
    car.setDirection(LEFT, backwards);
    car.setDirection(RIGHT, forwards);
    car.setSpeed(LEFT, (speedofcar-directionofcar));
    car.setSpeed(RIGHT, speedofcar);
  }

  else if (xValue < (xValueRest-4))
  {
    float a = xValueRest+4-xValueMin;
    float b = xValueRest+4-xValue;
    directionofcar = (255/(a))*(b);
    car.setDirection(LEFT, forwards);
    car.setDirection(RIGHT, backwards);
    car.setSpeed(LEFT, speedofcar);
    car.setSpeed(RIGHT, (speedofcar-directionofcar));
  }
  /* TODO - Option 1: Integrate code from library examples to read the MPU6050 and implement a PID routine */
}

// This function executes whenever I2C data is received
void receiveEvent(int howMany)
{
  char command = 0;
  int parameter = 0;

  if(howMany != 3)  // Commands should be 3 bytes long. Anything else is an error
  {
    emptyBuffer();
    return;
  }
  
  command = Wire.read(); // Read the first byte (the command character)

  byte MSB = Wire.read();   // receive MSB of parameter
  byte LSB = Wire.read();   // receive LSB of parameter

  parameter = (MSB << 8) | LSB; // combine the two bytes into an integer

  switch ( command )
  {
    case 'X' : xValue = parameter; break;
    case 'Y' : yValue = parameter; break;
  }
  
  Serial.print(" X: ");
  Serial.print(xValue);
  Serial.print(" Y: ");
  Serial.print(yValue);
  Serial.println("");

  
  

}
  
  
  // Process recieved command


// Function to clear the I2C buffer
void emptyBuffer(void)
{
  while(Wire.available())
  {
    Wire.read();
  }
}

