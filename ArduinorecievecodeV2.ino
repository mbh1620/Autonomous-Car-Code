#include <Wire.h>
#include <H61AEE_S01.h>


//Global Variables
Vehicle car;

//Functions for moving car

void fd(int distance) //function for moving car fd 
{
  car.setDirection(LEFT, forwards);
  car.setDirection(RIGHT, forwards);
  car.setSpeed(LEFT, 50);
  car.setSpeed(RIGHT, 50);
  delay(distance); //This constant is yet to be worked out
}

void bk(int distance)// function for moving car bk
{
  car.setDirection(LEFT, backwards);
  car.setDirection(RIGHT, backwards);
  car.setSpeed(LEFT, 50);
  car.setSpeed(RIGHT, 50);
  delay(distance); //This constant is yet to be worked out 
}

void rt(int angle)// function for turning rt
{
  car.setDirection(LEFT, forwards);
  car.setDirection(RIGHT, backwards);
  car.setSpeed(LEFT, 50);
  car.setSpeed(RIGHT, 50);
  delay(angle); //These constants are yet to be worked out
  
}

void lt(int angle)
{
  car.setDirection(LEFT, backwards);
  car.setDirection(RIGHT, forwards);
  car.setSpeed(LEFT, 50);
  car.setSpeed(RIGHT, 50);
  delay(angle); //These constants are yet to be worked out
}





void setup() {

  car.setupVehicle(); // configure car library
  car.enableMotors(true); // Disable motors so they dont move

  Wire.begin(7); //Join I2c bus at adress 7
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);

  fd(1000);
  
}

void loop() {
  delay(100);
}

//Function for when data is recieved from Raspberry Pi

void receiveEvent(int howMany)
{
  char datarr[200];
  for (int b = 0; b < 200; b++)
  {
    datarr[b] = 0; 
  }
  char c = 'a';
  int i=0;
  int param=0;
  while ( (c = Wire.read()) != '.')
  {
    if ( ( c > 32 ) && ( c < 123 ))
    {
      datarr[i++]= c;
      Serial.print(c);
    }
  }
  Serial.print(" All read");
  

  // "F:100."
  sscanf(datarr,"%c:%d", &c, &param);

  switch (c)
  {
    case 'F' :  Serial.print("Forward");
                Serial.print(param);
                fd(param);
                break;

    case 'B' :  Serial.print("Backwards");
                Serial.print(param);
                bk(param);
                break;
    
    case 'R' :  Serial.print("Right Turn");
                Serial.print(param);   
                rt(param);
                break;

    case 'L' :  Serial.print("Left Turn");
                Serial.print(param);
                lt(param);
                
                break;
  }
}

