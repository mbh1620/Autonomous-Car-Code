///////////////////////////////////////////////////////////////
//                                                           //
// H61AEE: Dept. EEE, The University of Nottingham 2018      //
// Author: D. Fallows                                        //
//                                                           //
// SENSOR_Template.ino: HMI Template Sketch                  //
//                      Session 4 - RF Remote Control        //
//                                                           //
///////////////////////////////////////////////////////////////

// Libraries
#include <Wire.h>
#include <SPI.h>
#include <RF24.h>
#include "SENSOR_Template.h"

RF24 radio(9, 10);  // Set up the nRF24L01+ radio (CE pin, CSN pin)

void setup()
{
  Wire.begin(SENSOR);           // join i2c bus as the Sensor board

  Serial.begin(9600);

  radio.begin();                    // Start the radio module
  radio.setPALevel(RF24_PA_LOW);    // Set the transmission power to max
  radio.setDataRate(RF24_250KBPS);  // Set the speed to 250kbps
  radio.setChannel(CHANNELNUMBER);  // Set the channel

  radio.openWritingPipe(rfAddresses[0]);    // Transmit to the remote address
  radio.openReadingPipe(1, rfAddresses[1]); // Recieve sensor addressed packets

  radio.setPayloadSize(PAYLOADSIZE); // Set the number of bytes for the data packet

  radio.startListening();

  /* TODO - configure any IO and libraries here */
}

void loop()
{
 
  if (radio.available())
  {
    while (radio.available())   //Read the full data packet
    {
      radio.read(&rfPacket, PAYLOADSIZE);  // rfPacket contains the data recieved from the remote as bytes
    }

    int test = rfPacket[0];   // Do something with the data... e.g. reconstuct an integer and display on the serial monitor
    test |= rfPacket[1] << 8;
    Serial.print("Y value: "); 
    Serial.print(test);
   
    writeToI2C(CAR, 'Y', test);
   }
   
  delay(100);
  
  if (radio.available())
  {
    while (radio.available())   //Read the full data packet
    {
      radio.read(&rfPacket, PAYLOADSIZE);  // rfPacket contains the data recieved from the remote as bytes
    }
  
    int test2 = rfPacket[0];   // Do something with the data... e.g. reconstuct an integer and display on the serial monitor
    test2 |= rfPacket[1] << 8;
    Serial.print("X value: "); 
    Serial.println(test2);
    
    writeToI2C(CAR, 'X', test2);
    delay(100);
  } 
}

void writeToI2C(char device, char command, int parameter)
{
  Wire.beginTransmission(device); // transmit to car
  Wire.write(command);  // send the command first
  Wire.write((byte)((parameter & 0xFF00) >> 8));  // send the parameter as two bytes
  Wire.write((byte)((parameter & 0x00FF)));
  Wire.endTransmission();    // stop transmitting
}

