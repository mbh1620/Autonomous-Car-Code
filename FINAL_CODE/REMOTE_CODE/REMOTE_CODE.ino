///////////////////////////////////////////////////////////////
//                                                           //
// H61AEE: Dept. EEE, The University of Nottingham 2018      //
// Author: D. Fallows                                        //
//                                                           //
// REMOTE_Template.ino: HMI Template Sketch                  //
//                      Session 4 - RF Remote Control        //
//                                                           //
///////////////////////////////////////////////////////////////

// Libraries
#include <Wire.h>
#include <SPI.h>
#include <RF24.h>
#include "REMOTE_Template.h"

#define joyX A2
#define joyY A1


RF24 radio(9, 10);  // Set up the nRF24L01+ radio (CE pin, CSN pin)

void setup()
{
  
  Wire.begin(REMOTE);           // Join I2C bus as the Remote board

  Serial.begin(9600);
  
  radio.begin();                    // Start the radio module
  radio.setPALevel(RF24_PA_LOW);    // Set the transmission power to max
  radio.setDataRate(RF24_250KBPS);  // Set the speed to 250kbps
  radio.setChannel(CHANNELNUMBER);  // Set the channel

  radio.openWritingPipe(rfAddresses[1]);    // Transmit to the remote address
  radio.openReadingPipe(1, rfAddresses[0]); // Recieve sensor addressed packets
  
  radio.setPayloadSize(PAYLOADSIZE); // Set the number of bytes for the data packet

  

  /* TODO - configure your inputs and outputs here, call any setup functions required */
}

void loop()
{
  /* TODO -  add your own code to process the recieved RF data */
 

  /* TODO -  add your own code to read the joystick data and transmit */
  
  // Stop listening so we can transmi
  int xValue; 
  int yValue;
   
  xValue = analogRead(joyX);
  radio.write(&xValue,  sizeof(int));

  Serial.print("X Value ");
  Serial.println(xValue);

  delay(100);
  
  
  yValue = analogRead(joyY); 
  radio.write(&yValue, sizeof(int));
  
  Serial.print("Y value ");
  Serial.println(yValue);

  delay(100);
   // Start listening again
}

void writeToI2C(char device, char command, int parameter)
{
  Wire.beginTransmission(device); // transmit to device
  Wire.write(command);  // send the command first
  Wire.write((byte)((parameter & 0xFF00) >> 8));  // send the parameter as two bytes
  Wire.write((byte)((parameter & 0x00FF)));
  Wire.endTransmission();    // stop transmitting
}

