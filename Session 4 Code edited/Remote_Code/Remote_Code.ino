/*Libraries*/
#include <SPI.h>
#include "RF24.h"
#include <LiquidCrystal.h>
#include <Encoder.h>
#include "Remote_Template.h"

/*Pin Declarations*/
const int JoyY = A3, JoyX = A0;
RF24 radio (9, 10);
const int RS = 7, Enable = 6, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal MyLCD (RS, Enable, D4, D5, D6, D7);
Encoder myEnc(18, 19);

/*Global Variables*/
int ButtonXYKpKiKdValues[6] = {0,0,0,0,0,0}, ButtonCount = 1;
long oldPosition  = -999, newPosition;

int GyroData[2];

void setup() {
  radio.begin();                              // Start the radio module
  radio.setPALevel(RF24_PA_LOW);              // Set the transmission power to low
  radio.setDataRate(RF24_250KBPS);            // Set the speed to 250kbps
  radio.setChannel(CHANNELNUMBER);            // Set the channel
  radio.openWritingPipe(rfAddresses[1]);      // Transmit to the remote address
  radio.openReadingPipe(1, rfAddresses[0]);   // Recieve sensor addressed packets
  radio.setPayloadSize(PAYLOADSIZE);          // Set the number of bytes for the data packet

  Serial.begin (9600);

  MyLCD.begin(16, 4);
}


void loop() {
  /*Analogue inputs are read in and converted to a number from -255 to 255*/
    if (analogRead(JoyX) > 505) { /*Right*/
      ButtonXYKpKiKdValues[1] = map(constrain(analogRead(JoyX), 505, 830), 505, 830, 0, 255);
    
    } else if (analogRead(JoyX) < 495) { /*Left*/
      ButtonXYKpKiKdValues[1] = 0 - map(constrain(analogRead(JoyX), 60, 505), 505, 60, 0, 255);
  
    } else { /*Straight*/
      ButtonXYKpKiKdValues[1] = 0;
    }
  
    if (analogRead(JoyY) > 508) { /*Backwards*/
      ButtonXYKpKiKdValues[2] = 0 - map(constrain(analogRead(JoyY), 508, 890), 508, 890, 0, 255);
  
    } else if (analogRead(JoyY) < 495) { /*Forwards*/
      ButtonXYKpKiKdValues[2] = map(constrain(analogRead(JoyY), 90, 495), 495, 90, 0, 255);
    
    } else { /*Stationary*/
      ButtonXYKpKiKdValues[2] = 0;
    }

     /*******Rotary Enocder Code for reading in button count*********/

    if(analogRead(A6) == 0) {
      ButtonCount++;
      delay(100);
     }

    else 
        ;
      
    if (ButtonCount > 3)
        ButtonCount = 1;
    
    switch (ButtonCount) {
       case 1: /*Kp Values*/
           newPosition = myEnc.read();
           if (newPosition != ButtonXYKpKiKdValues[3])
              ButtonXYKpKiKdValues[3] = newPosition;
           break;

       case 2: /*Ki Values*/
            newPosition = myEnc.read();
            if (newPosition != ButtonXYKpKiKdValues[4])
                ButtonXYKpKiKdValues[4] = newPosition;
            break;

       case 3: /*Kd Values*/
            newPosition = myEnc.read();
            if (newPosition != ButtonXYKpKiKdValues[5])
                ButtonXYKpKiKdValues[5] = newPosition;
            break;

       default:
          Serial.print("Button Count Error");
          break;
    }

    if ((ButtonXYKpKiKdValues[1] != 0) || (ButtonXYKpKiKdValues[2] != 0)) /*If the joysticks have been moved*/
      ButtonXYKpKiKdValues[0] = 1;                                           /*then switch to remote control*/

    if (ButtonXYKpKiKdValues[0] == 1) {
      LCDRFMode();
    }

    else {
      LCDLineFollowingMode();
    }

  PrintAllInfo();  
  radio.stopListening();
  radio.write(&ButtonXYKpKiKdValues, sizeof(ButtonXYKpKiKdValues));
  radio.startListening();
  radio.read(&GyroData, sizeof(GyroData));
  delay(1);
}





void LCDRFMode () {
  MyLCD.setCursor(0, 0);
  MyLCD.print("Remote Control M");
      
  MyLCD.setCursor(0, 1);
  MyLCD.print("XValue = ");
  if (ButtonXYKpKiKdValues[1] == 0)
    MyLCD.print("0   ");
  else
    MyLCD.print(ButtonXYKpKiKdValues[1]);
    
  MyLCD.setCursor(0, 2);
  MyLCD.print("ode YValue = ");
  if (ButtonXYKpKiKdValues[2] == 0)
    MyLCD.print("0   ");
  else
    MyLCD.print(ButtonXYKpKiKdValues[2]);
  
  MyLCD.setCursor(0, 3);
  MyLCD.print("    Acc = ");
//  if (GyroData[0] == 0)
//    MyLCD.print("0 ");
//  else
    MyLCD.print(GyroData[0]);
 
  MyLCD.print("  Angle = ");
//  if (GyroData[1] == 0)
//    MyLCD.print("0 ");
//  else
    MyLCD.print(GyroData[1]);
}

void LCDLineFollowingMode () {
  MyLCD.setCursor(0, 0);
  MyLCD.print("Line Following Mode");
    
  MyLCD.setCursor(0, 1);
  MyLCD.print("Kp = ");
  MyLCD.print(ButtonXYKpKiKdValues[3]);
    
  MyLCD.setCursor(0, 2);
  MyLCD.print("ode Ki = ");
  MyLCD.print(ButtonXYKpKiKdValues[4]);
    
  MyLCD.setCursor(0, 3);
  MyLCD.print("    Kd = ");
  MyLCD.print(ButtonXYKpKiKdValues[5]);
}

void PrintAllInfo () {
    Serial.print("Button = ");
    Serial.print(ButtonXYKpKiKdValues[0]);
    Serial.print("   X Value = ");
    Serial.print(ButtonXYKpKiKdValues[1]);
    Serial.print("   Y Value = ");
    Serial.print(ButtonXYKpKiKdValues[2]);
    Serial.print("   Kp = ");
    Serial.print(ButtonXYKpKiKdValues[3]);
    Serial.print("   Ki = ");
    Serial.print(ButtonXYKpKiKdValues[4]);
    Serial.print("   Kd = ");
    Serial.println(ButtonXYKpKiKdValues[5]);
}
