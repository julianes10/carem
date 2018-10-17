#include <Arduino.h>
#include <carLight.h>

#include "carEngine.h"
#include "lsem.h"



#define POS_FLASHING_LEFT_FRONT  3 
#define POS_FLASHING_LEFT_BACK   4
#define POS_FLASHING_RIGHT_FRONT 0
#define POS_FLASHING_RIGHT_BACK  7
#define POS_FRONT_LEFT  2
#define POS_FRONT_RIGHT 1
#define POS_BACK_LEFT   5
#define POS_BACK_RIGHT  6
#define POS_BACK_CENTRAL_INIT  8
#define POS_BACK_CENTRAL_END  12





//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
CarLight::CarLight(LSEM *fLS,LSEM *sLS)
{
 
  f=fLS;
  s=sLS;

}

//------------------------------------------------
void CarLight::refresh(uint8_t mode)
{

  if (mode==E_MODE_ZERO) {
     //std light off
  }

  switch(mode) {
    case E_MODE_STOP:          _doStop();        break;
    case E_MODE_FORWARD:       setFront();       break;
    case E_MODE_BACKWARD:      setBack();       break;
    case E_MODE_LEFT:          setFlashingLeft();     break;
    case E_MODE_RIGHT:         setFlashingRight();    break;
    default: _doStop();                            break;
  }

  setFront();//TESTT PPPPPPPPPPPPPPPPPPPPPPPPPPPP TODO
  f->refresh();
  s->refresh();
  FastLED.show();
}

//------------------------------------------------
void CarLight::setFront()
{
  for (int i=0;i<NUM_LEDS_STDCAR;i++){
    sPatterns[0][i]=0xFF0000;
  }
  for (int i=0;i<NUM_LEDS_STDCAR;i++){
    sPatterns[1][i]=0x0000FF;
  }

  s->setPattern(0,&sPatterns[0][0]);
  s->setPattern(1,&sPatterns[1][0]);
}

//------------------------------------------------
void CarLight::setBack()
{
}

//------------------------------------------------
void CarLight::setFlashingLeft()
{
}

//------------------------------------------------
void CarLight::setFlashingRight()
{
}

//------------------------------------------------
void CarLight::setFlashingAll()
{
}


//------------------------------------------------
void CarLight::_doStop()
{
//Setup a timer and off
}


