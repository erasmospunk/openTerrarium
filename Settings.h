#ifndef SETTINGS_h
#define SETTINGS_h

#include <Arduino.h>
#include <EEPROM.h>
#include <Time.h>

#include "Constants.h"


#define MIN_MID_TEMP_HOURS_DURATION 4
#define MAX_MID_TEMP_HOURS_DURATION 16


// ID of the settings block
#define CONFIG_VERSION "Rt2"

// Tell it where to store your config data in EEPROM
#define CONFIG_START 32



struct SettingsStoreStruct {
  char version[4];
  // The settings variables
  byte lcdBrightness; 
  byte lcdTimeout;

  int maxTargetTemp;
  int minTargetTemp;
  byte midTempRatio;
  byte midTempHoursDuration;
  byte maxTargetTimeHours;
  
  byte relayOnDayPercent;
//  byte maxTargetTimeMinutes;
//  byte minTargetTimeHours;
//  byte minTargetTimeMinutes;
};

class SettingsClass {
public:
  void loadConfig();
  void saveConfig();
  void debugConfig(); 
  void loadDefault();

  //Getters and setters
  byte getLcdBrightness() { return _vars.lcdBrightness; }
  void setLcdBrightness(byte lcdBrightness) { _vars.lcdBrightness = lcdBrightness;}
  
  byte getLcdTimeout() { return _vars.lcdTimeout; }
  void setLcdTimeout(byte lcdTimeout) { _vars.lcdTimeout = lcdTimeout;}
  
  float getRelayOnDayPercent() { return (float)(_vars.relayOnDayPercent) / 100; }
  void setRelayOnDayPercent(float relayOnDayPercent) { _vars.relayOnDayPercent = relayOnDayPercent * 100; }
  
  float getMidTempRatioFloat() { return (float)(_vars.midTempRatio) / 100; }
  byte getMidTempRatio() { return _vars.midTempRatio; }
  void setMidTempRatio(byte midTempRatio) { _vars.midTempRatio = midTempRatio; }
  
  //depricated
  float getMidLowTargetTemp() {
    return (float)(getMaxTargetTempFloat() - getMinTargetTempFloat()) * getMidTempRatioFloat();
  }
  
  float getMidHiTargetTemp() {
    return getMaxTargetTempFloat() - getMidTargetTemp();
  }
  
  float getMidTargetTemp() {
    return getMinTargetTempFloat() + getMidLowTargetTemp();
  }
  
  float getMaxTargetTempFloat() { return (float)(_vars.maxTargetTemp) / 100; }
  int getMaxTargetTemp() { return _vars.maxTargetTemp; }
  void setMaxTargetTemp(int maxTargetTemp) { _vars.maxTargetTemp = maxTargetTemp;}
  
  float getMinTargetTempFloat() { return (float)(_vars.minTargetTemp) / 100; }
  int getMinTargetTemp() { return _vars.minTargetTemp; }
  void setMinTargetTemp(int minTargetTemp) { _vars.minTargetTemp = minTargetTemp;}

  time_t getMaxTargetTempSeconds() { return _vars.maxTargetTimeHours * SECS_PER_HOUR; }
  time_t getMinTargetTempSeconds() { (getMaxTargetTempSeconds() + SECS_PER_HALF_DAY) % SECS_PER_DAY; }  
                           
  time_t getMidTempSecondsDuration() { return _vars.midTempHoursDuration * SECS_PER_HOUR; }                   
  char getMidTempHoursDuration() { return _vars.midTempHoursDuration; }
  void setMidTempHoursDuration(char midTempHoursDuration) {
    if (midTempHoursDuration < MIN_MID_TEMP_HOURS_DURATION) {
      _vars.midTempHoursDuration = MAX_MID_TEMP_HOURS_DURATION;
    }
    else if (midTempHoursDuration > MAX_MID_TEMP_HOURS_DURATION) {
      _vars.midTempHoursDuration = MIN_MID_TEMP_HOURS_DURATION;
    }
    else {
      _vars.midTempHoursDuration = midTempHoursDuration;
    }
  }
  
  char getMinTargetTimeHour() { return (_vars.maxTargetTimeHours + HOURS_PER_HALF_DAY) % HOURS_PER_DAY; }
  char getMaxTargetTimeHour() { return _vars.maxTargetTimeHours; }
  void setMaxTargetTimeHour(char maxTargetTimeHours) {
    if (maxTargetTimeHours < 0)
      _vars.maxTargetTimeHours = HOURS_PER_DAY - 1;
    else if (maxTargetTimeHours > HOURS_PER_DAY - 1)
      _vars.maxTargetTimeHours = 0;
    else
      _vars.maxTargetTimeHours = maxTargetTimeHours;
  }
  
//  char getMinTargetTimeHours() { return _vars.minTargetTimeHours; }
//  void setMinTargetTimeHours(char minTargetTimeHours) {
//    if (minTargetTimeHours < 0)
//      _vars.minTargetTimeHours = 23;
//    else
//      _vars.minTargetTimeHours = minTargetTimeHours % 24;
//  }
//  
//  char getMaxTargetTimeMinutes() { return _vars.maxTargetTimeMinutes; }
//  void setMaxTargetTimeMinutes(char maxTargetTimeMinutes) {
//    if (maxTargetTimeMinutes < 0)
//      _vars.maxTargetTimeMinutes = 59;
//    else
//      _vars.maxTargetTimeMinutes = maxTargetTimeMinutes % 60;
//  }
//  
//  char getMinTargetTimeMinutes() { return _vars.minTargetTimeMinutes; }
//  void setMinTargetTimeMinutes(char minTargetTimeMinutes) {
//    if (minTargetTimeMinutes < 0)
//      _vars.minTargetTimeMinutes = 59;
//    else
//      _vars.minTargetTimeMinutes = minTargetTimeMinutes % 60;
//  }
  
private:
  SettingsStoreStruct _vars;
};

extern SettingsClass Settings;
 
#endif
