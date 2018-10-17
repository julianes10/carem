#include <SoftwareSerial.h>

#include "carEngine.h"
#include "carLight.h"
#include "lsem.h"



//-- SOME NEEDED PROTOTYPES ---------------------------

void STATE_init(void);
void STATE_welcome(void);
void STATE_idle(void);

// State pointer function
void (*GLBptrStateFunc)();


//------------------------------------------------
//--- GLOBAL VARIABLES ---------------------------
//------------------------------------------------
char    GLBserialInputString[100]; // a string to hold incoming data
char    GLBauxString[100];
int     GLBserialIx=0;
bool    GLBserialInputStringReady = false; // whether the string is complete

const char inputBootStringFLS[] PROGMEM ={":LP0080:LT0030:LMK:LCFF,00,00"};

const char inputBootStringSLS[] PROGMEM ={":MP0080:MT0030:MMK:MC00,00,FF"};

CarEngine car;

#define NUM_LEDS_FRONT  10
#define NUM_LEDS_STDCAR 13
#define DATA_PIN_LS_FRONT 8
#define DATA_PIN_LS_STDCAR 9

//------------------------------------------------
//-------    TIMER CALLBACKS PROTOTYPES  ---------
//------------------------------------------------
void GLBcallbackTimeoutFLS(void);
void GLBcallbackTimeoutSLS(void);
void GLBcallbackPauseFLS(void);
void GLBcallbackPauseSLS(void);

//------------------------------------------------
//-------    GLOBAL VARS TO THIS MODULE  ---------
//------------------------------------------------
CRGB FrontLS[NUM_LEDS_FRONT];
CRGB StdCarLS[NUM_LEDS_STDCAR];

LSEM fLS(FrontLS, NUM_LEDS_FRONT, GLBcallbackPauseFLS, GLBcallbackTimeoutFLS);
LSEM sLS(StdCarLS,NUM_LEDS_STDCAR,GLBcallbackPauseSLS, GLBcallbackTimeoutSLS);




CarLight lights(&fLS,&sLS);


//------------------------------------------------
//--- GLOBAL FUNCTIONS ---------------------------
//------------------------------------------------



//------------------------------------------------
//-------    TIMER CALLBACKS     -----------------
//------------------------------------------------
void GLBcallbackTimeoutFLS(void)
{
  //if (LSEM.getDebug()) {Serial.println(F("DEBUG: callbackTimeout"));}
  fLS.callbackTimeout();
}
void GLBcallbackTimeoutSLS(void)
{
  //if (LSEM.getDebug()) {Serial.println(F("DEBUG: callbackTimeout"));}
  sLS.callbackTimeout();
}
//------------------------------------------------
void GLBcallbackPauseFLS(void)
{
  //Serial.println(F("DEBUG: callbackPause");
  fLS.callbackPause();
}

//------------------------------------------------
void GLBcallbackPauseSLS(void)
{
  //Serial.println(F("DEBUG: callbackPause");
  sLS.callbackPause();
}





//------------------------------------------------

void setup() { 

  // Serial to debug AND comunication protocolo with PI              
  Serial.begin(9600);
  Serial.println(F("Setup... 'came on, be my baby, came on'"));
  GLBserialInputString[0]=0;

  GLBptrStateFunc = STATE_init;
  Serial.println(F("STATE INIT"));


  sLS.customProtocolId('M');
  FastLED.addLeds<WS2812B,DATA_PIN_LS_FRONT,GRB>(FrontLS, NUM_LEDS_FRONT);
  FastLED.addLeds<WS2812B,DATA_PIN_LS_STDCAR,GRB>(StdCarLS, NUM_LEDS_STDCAR);


  
}

//------------------------------------------------
void serialEvent() {
  while (Serial.available()) {
     char inChar = (char)Serial.read();
     if (inChar < 0x20) {
       GLBserialInputStringReady = true;
       GLBserialInputString[GLBserialIx]=0;
       GLBserialIx=0;
       return;
     }
     GLBserialInputString[GLBserialIx++]=inChar;
  }
}

//-------------------------------------------------
void processSerialInputString()
{
  strcpy(GLBauxString,GLBserialInputString);
  GLBserialInputString[0]=0;
  GLBserialInputStringReady = false;
  lights.f->processCommands(GLBauxString);
  lights.s->processCommands(GLBauxString);
  car.processCommands(GLBauxString);
}

//-------------------------------------------------
void STATE_init(void)
{

  Serial.println(F("DEBUG: inputBootStringFLS..."));
  strcpy_P(GLBauxString,(char*)inputBootStringFLS);
  lights.f->processCommands(GLBauxString);
  strcpy_P(GLBauxString,(char*)inputBootStringSLS);
  lights.s->processCommands(GLBauxString);


  GLBptrStateFunc=STATE_welcome;
  Serial.println(F("STATE INIT -> WELCOME"));
}
//-------------------------------------------------
void STATE_welcome(void)
{
  if (lights.f->isIdle()) {
    GLBptrStateFunc=STATE_idle;
    Serial.println(F("STATE WELCOME -> IDLE"));
  }
}
//-------------------------------------------------
void STATE_idle(void)
{
  if (GLBserialInputStringReady){
    processSerialInputString();
  }
}

//-------------------------------------------------
//-------------------------------------------------
//-------------------------------------------------
void loop() { 

  //------------- INPUT REFRESHING ----------------

  //--------- TIME TO THINK MY FRIEND -------------
  // State machine as main controller execution
  GLBptrStateFunc();

  // ------------- OUTPUT REFRESHING ---------------
  // Write led strip
  lights.refresh(car.getMode());

  // Write motor 
  car.refresh();



}

