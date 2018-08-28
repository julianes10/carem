#include <Arduino.h>

#include "l298nem.h"
#include "engine.h"


#define DEFAULT_SPEED 200


//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
Engine::Engine(): _frontWheels(6,11,12,5,13,4)
{
}
//------------------------------------------------
void Engine::forward ()
{
  _frontWheels.forward();
  //_backWheels.forward();
}
//------------------------------------------------
void Engine::backward ()
{
  _frontWheels.backward();
  //_backWheels.backward();
}
//------------------------------------------------
void Engine::right ()
{
  _frontWheels.right();
  //_backWheels.right();
}
//------------------------------------------------
void Engine::left ()
{
  _frontWheels.left();
  //_backWheels.left();
}
//------------------------------------------------
void Engine::stop ()
{
  _frontWheels.stop();
  //_backWheels.stop();
}
//------------------------------------------------
uint8_t Engine::speed (uint8_t s)
{
  return _frontWheels.speed(s);
  //return _backWheels.speed(s);
}
//------------------------------------------------
uint8_t Engine::brake ()
{
  return _frontWheels.brake();
  //return _backWheels.brake();
}
//------------------------------------------------
uint8_t Engine::gas ()
{
  return _frontWheels.gas();
  //return _backWheels.gas();
}
//------------------------------------------------
void Engine::turnLeft(turn_t t)
{
switch(t){
case min:;
default:;
};
}
//------------------------------------------------
void Engine::turnRight(turn_t t)
{
switch(t){
case min:;
default:;
};
}
