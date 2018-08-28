#ifndef Engine_h
#define Engine_h
#include "l298nem.h"
#include "engine.h"
#include "Arduino.h"

enum turn_t { min, turn45, turn90, uturn };

class Engine
{
 public:

  Engine();
  
  void forward();
  void backward();
  void stop();
  void left();
  void right();
  uint8_t speed(uint8_t s);
  uint8_t brake();
  uint8_t gas();
  void turnLeft(turn_t t);
  void turnRight(turn_t t);

 private:

  L298NEM _frontWheels;
  //L298NEM _backWheels;

};

#endif


