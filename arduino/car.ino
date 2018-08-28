#include <SoftwareSerial.h>

#include "engine.h"

Engine car;


void setup()
{
  Serial.begin(9600);
  Serial.println(F("Setup... 'came on, be my babe...'"));
  Serial.println(F("Foward,Backward,Stop,Left,Right. Press first letter."));
}

void loop()
{

  if (Serial.available()){
    char c=Serial.read();
    Serial.print(c);

    switch (c)
    {
      case 'F':
        car.forward();
        break;
      case 'B':
        car.backward();
        break;
      case 'R':
        car.right();
        break;
      case 'L':
        car.left();
        break;
      case 'S':
        car.stop();
        break;
      case '+':
        car.gas();
        break;
      case '-':
        Serial.println(car.brake());
        break;

    }
  }
}



