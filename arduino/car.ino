#include <SoftwareSerial.h>

#include "carEngine.h"
#include "lsem.h"


CarEngine car;

//------------------------------------------------
//--- GLOBAL VARIABLES ---------------------------
//------------------------------------------------
char    GLBserialInputString[100]; // a string to hold incoming data
char    GLBauxString[100];
int     GLBserialIx=0;
bool    GLBserialInputStringReady = false; // whether the string is complete

const char inputBootString[] PROGMEM ={":LP0020:LT0005:LMK:LCFF,00,00:LQ:LMk:LQ:LMN"};


//------------------------------------------------
//--- GLOBAL FUNCTIONS ---------------------------
//------------------------------------------------

//-- SOME NEEDED PROTOTYPES ---------------------------

void STATE_init(void);
void STATE_welcome(void);
void STATE_idle(void);

// State pointer function
void (*GLBptrStateFunc)();


//------------------------------------------------

void setup() { 

  // Serial to debug AND comunication protocolo with PI              
  Serial.begin(9600);
  Serial.println(F("Setup... 'came on, be my baby, came on'"));
  GLBserialInputString[0]=0;

  GLBptrStateFunc = STATE_init;
  Serial.println(F("STATE INIT"));
  
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
  LSEM.processCommands(GLBauxString);
  car.processCommands(GLBauxString);
}

//-------------------------------------------------
void STATE_init(void)
{
  Serial.println(F("DEBUG: inputBootString..."));
  strcpy_P(GLBauxString,(char*)inputBootString);
  LSEM.processCommands(GLBauxString);

  GLBptrStateFunc=STATE_welcome;
  Serial.println(F("STATE INIT -> WELCOME"));
}
//-------------------------------------------------
void STATE_welcome(void)
{
  if (LSEM.isIdle()) {
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
  LSEM.refresh();

  // Write motor 
  car.refresh();

}

