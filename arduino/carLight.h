#ifndef carLight_h
#define carLight_h

#include "Arduino.h"

#include "carEngine.h"
#include "lsem.h"


#define NUM_LEDS_FRONT  10
#define NUM_LEDS_STDCAR 13
#define DATA_PIN_LS_FRONT 8
#define DATA_PIN_LS_STDCAR 9


class CarLight
{
 public:

  CarLight(LSEM *fLS,LSEM *sLS);

  void refresh(uint8_t mode);
  void setFront();
  void setBack();
  void setFlashingLeft();
  void setFlashingRight();
  void setFlashingAll();

  LSEM *f;
  LSEM *s;


 private: 

  CRGB fPatterns[2][NUM_LEDS_FRONT];
  CRGB sPatterns[2][NUM_LEDS_STDCAR];

  void _doStop();

};

#endif


