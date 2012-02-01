/********************************************************
bugs:
  temp >40 or <0. Problem in tempGraph
  edit temp Settings affects relay control
  get temp like this: getTemo && reqTemp ==> faster updates
*/

#define VERSION "0.1"

#include <OneWire.h>
#include <DallasTemperature.h>

#include <Arduino.h>
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t

#include <MsTimer2.h>

#include "pitches.h"

#include <EEPROM.h>
#include "Settings.h"
#include "Constants.h"





static double tempC;

struct GlobalStruct {
 unsigned long lastUserInteraction;
 unsigned long lastBgTask;
} Global;


//up .O...
void (*upPressed)(boolean);
//down O....
void (*downPressed)(boolean);
//left ....O
void (*leftPressed)(boolean);
//right ...O.
void (*rightPressed)(boolean);
//enter ..O..
void (*enterPressed)(boolean);


void setup() {
//  Serial.begin(9600);  
  Serial.begin(115200);
  
  Serial.print("Wellcome to HotReptile! ");
  Serial.println(VERSION);
  Serial.println();
  
  
  //load settings
  Settings.loadConfig();
  Serial.println();
  
  //setup time
  setSyncProvider(RTC.get);
    
  initTempSensor();
  //initRelay();
  initDimmer();
  initLcd();
  initButtons();
  
  loadMainScreen();  
  Serial.println();
  
  backgroundTasks();
  
   
//  MsTimer2::set(TEMP_UPDATE_PERIOD, backgroundTasks); // temperature update period
//  MsTimer2::start();
  
  Global.lastUserInteraction = now();
  
}

void loop() {

  uiUpdate();  

  fastBackgroundTasks();
  
  backgroundTasks();
  
  //dimmerControl(3000);
  //delay(PROGRAM_SPEED);
}

//can use LCD here
void fastBackgroundTasks() {

}

//don't use LCD here!!
void backgroundTasks() {
  //ensure that we update TEMP_UPDATE_PERIOD
  if ((long)(millis() - Global.lastBgTask) < TEMP_UPDATE_PERIOD) {
    return;
  }
  Global.lastBgTask = millis();
  
  Serial.print(year());Serial.print("/");
  serialPrintDigits(month());Serial.print("/");
  serialPrintDigits(day());Serial.print(" ");
  serialPrintDigits(hour());Serial.print(":");
  serialPrintDigits(minute());Serial.print(":");
  serialPrintDigits(second());
  
    
  
  
  tempC = getTemperature();
  
  //targetTemp for realay control. TODO handle better
//  setTargetTemp(getSimulateClimateTemperature());
  
  //controlRelay(tempC);

  //controlTimedRelay(Settings.getRelayOnDayPercent());
  
  Serial.println();
}


//void (*screenHandler)();
void (*logic)();
void uiUpdate() {
  
  logic();
  buttons();
  autocontrolLcdBrightness();
  autocontrolScreenReset();
}




void serialPrintDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
















// notes in the melody:
int melody[] = { NOTE_G3 };

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 4 };

void medody() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 1; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
  }
}
