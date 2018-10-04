#include <SoftwareSerial.h>

#include "carEngine.h"
#include "lsem.h"



//-- SOME NEEDED PROTOTYPES ---------------------------

void STATE_init(void);
void STATE_welcome(void);
void STATE_idle(void);

// State pointer function
void (*GLBptrStateFunc)();
void GLBcallbackTimeoutFLS(void);
void GLBcallbackTimeoutSLS(void);
void GLBcallbackPauseFLS(void);
void GLBcallbackPauseSLS(void);



//------------------------------------------------
//--- GLOBAL VARIABLES ---------------------------
//------------------------------------------------
char    GLBserialInputString[100]; // a string to hold incoming data
char    GLBauxString[100];
int     GLBserialIx=0;
bool    GLBserialInputStringReady = false; // whether the string is complete

const char inputBootString[] PROGMEM ={":LP0020:LT0005:LMK:LCFF,00,00:LQ:LMk:LQ:LMN"};

CarEngine car;


#define NUM_LEDS_FRONT  8
#define NUM_LEDS_STDCAR 10
#define DATA_PIN_LS_FRONT 7
#define DATA_PIN_LS_STDCAR 8
CRGB FrontLS[NUM_LEDS_FRONT];
CRGB StdCarLS[NUM_LEDS_STDCAR];


LSEM fLS(FrontLS, NUM_LEDS_FRONT, GLBcallbackPauseFLS, GLBcallbackTimeoutFLS);
LSEM sLS(StdCarLS,NUM_LEDS_STDCAR,GLBcallbackPauseSLS, GLBcallbackTimeoutSLS);


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

  FastLED.addLeds<WS2812B,DATA_PIN_LS_FRONT,GRB>(FrontLS, NUM_LEDS_FRONT);
  FastLED.addLeds<WS2812B,DATA_PIN_LS_STDCAR,GRB>(StdCarLS, NUM_LEDS_STDCAR);

  sLS.customProtocolId('M');
  
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
  fLS.processCommands(GLBauxString);
  sLS.processCommands(GLBauxString);
  car.processCommands(GLBauxString);
}

//-------------------------------------------------
void STATE_init(void)
{
  Serial.println(F("DEBUG: inputBootString..."));
  strcpy_P(GLBauxString,(char*)inputBootString);
  fLS.processCommands(GLBauxString);
  sLS.processCommands(GLBauxString);


  GLBptrStateFunc=STATE_welcome;
  Serial.println(F("STATE INIT -> WELCOME"));
}
//-------------------------------------------------
void STATE_welcome(void)
{
  if (fLS.isIdle()) {
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
  fLS.refresh();
  sLS.refresh();
  FastLED.show();

  // Write motor 
  car.refresh();



}

