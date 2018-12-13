#include <Wire.h>
#include <H61AEE_S02.h>

//Global Variables
Vehicle car;

void setup() {
  car.setupVehicle(); // configure car library
  car.enableMotors(true); // Disable motors so they dont move
  Serial.begin(9600);
}

void loop() {
  int yValue;
  int yValueMax = 909;
  int yValueRest = 498; //Give a +/- 4 on this
  int yValueMin = 80;
  
  int xValue;
  int xValueMax = 880;
  int xValueRest = 510; //Give a +/- 4 on this
  int xValueMin = 53;
  
  int speedofcar;
  int directionofcar;
  
//if functions for left joystick that moves tha car fowards or backwards//

  if (yValue > (yValueRest+4))
  {
    speedofcar = (255/(yValueMax-yValueRest-4))*(yValue-yValueRest-4);
    car.setDirection(LEFT, forwards);
    car.setDirection(RIGHT, forwards);
    car.setSpeed(LEFT, speedofcar);
    car.setSpeed(RIGHT, speedofcar);
  }

   else if (yValue < (yValueRest-4))
  {
    speedofcar = (255/(yValueRest+4-yValueMin))*(yValueRest+4-yValue);
    car.setDirection(LEFT, backwards);
    car.setDirection(RIGHT, backwards);
    car.setSpeed(LEFT, speedofcar);
    car.setSpeed(RIGHT, speedofcar);
  }


//if functions for right joystick that moves tha car left or right

   if (xValue > (xValueRest+4))
  {
    directionofcar = (255/(xValueMax-xValueRest-4))*(xValue-xValueRest-4);
    car.setDirection(LEFT, backwards);
    car.setDirection(RIGHT, forwards);
    car.setSpeed(LEFT, (speedofcar-directionofcar));
    car.setSpeed(RIGHT, speedofcar);
  }

  else if (xValue < (xValueRest-4))
  {
    directionofcar = (255/(xValueRest+4-xValueMin))*(xValueRest+4-xValue);
    car.setDirection(LEFT, forwards);
    car.setDirection(RIGHT, backwards);
    car.setSpeed(LEFT, speedofcar);
    car.setSpeed(RIGHT, (speedofcar-directionofcar));
  }
}
