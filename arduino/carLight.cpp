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

const char nearf[]      PROGMEM ={":LP0080:LT0020:LMA:LC"};
const char nears[]      PROGMEM ={":MP0080:MT0020:MMA:MC"};
const char veryNearf[]  PROGMEM ={":LP0080:LT0020:LMN"};
const char veryNears[]  PROGMEM ={":MP0080:MT0020:MMN"};

//------------------------------------------------
void CarLight::refresh(uint8_t mode, int distance)
{
   char aux[20];
   char aux2[10];
/*
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
*/
  if (f->isIdle() && s->isIdle())
  {
    if (distance < 5)
    {
      strcpy_P(aux,(char*)veryNearf);
      f->processCommands(aux);
      strcpy_P(aux,(char*)veryNears);
      s->processCommands(aux);
    }
    else if (distance < 30)
    {

      if (distance < 10){
        strcpy(aux2,"00,00,FF");
      }
      else if (distance < 15){
        strcpy(aux2,"df,42,f4");
      }
      else if (distance < 20){
        strcpy(aux2,"FF,00,00");
      }
      else if (distance < 25){
        strcpy(aux2,"00,FF,00");
      }
      strcpy_P(aux,(char*)nearf);
      strcat_P(aux,aux2);
      f->processCommands(aux);
      strcpy_P(aux,(char*)nears);
      strcat_P(aux,aux2);
      s->processCommands(aux);
    }
  }
  //setFront();//TESTT PPPPPPPPPPPPPPPPPPPPPPPPPPPP TODO
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


