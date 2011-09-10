#include "Settings.h"

void SettingsClass::loadConfig() {
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2]) {
    Serial.println("Loading settings...");
    for (unsigned int t=0; t<sizeof(_vars); t++) {
      *((byte*)&_vars + t) = EEPROM.read(CONFIG_START + t);
    }
    
    debugConfig();
  }
  else {
    Serial.println("ERROR GETING SETTINGS FROM EEPROM.\nLoading defaults.");
    loadDefault();
    saveConfig();
  }
}

void SettingsClass::debugConfig() {
  Serial.print("\tversion = ");Serial.println(_vars.version);
  Serial.print("\tlcdBrightness = ");Serial.println(_vars.lcdBrightness, DEC);
  Serial.print("\tlcdTimeout = ");Serial.println(_vars.lcdTimeout, DEC);
  Serial.print("\tmidTempRatio = ");Serial.println(_vars.midTempRatio, DEC);
  Serial.print("\tmidTempHoursDuration = ");Serial.println(_vars.midTempHoursDuration, DEC);
  Serial.print("\tmaxTargetTemp = ");Serial.println(_vars.maxTargetTemp, DEC);
  Serial.print("\tminTargetTemp = ");Serial.println(_vars.minTargetTemp, DEC);
  Serial.print("\tmaxTargetTimeHours = ");Serial.println(_vars.maxTargetTimeHours, DEC);
  Serial.print("\trelayOnDayPercent = ");Serial.println(_vars.relayOnDayPercent, DEC);
  
}


void SettingsClass::saveConfig() {
  Serial.println("Saving settings...");
  for (unsigned int t=0; t<sizeof(_vars); t++) {
    EEPROM.write(CONFIG_START + t, *((byte*)&_vars + t));
  }
}


void SettingsClass::loadDefault() {
  _vars.version[0] = CONFIG_VERSION[0];
  _vars.version[1] = CONFIG_VERSION[1];
  _vars.version[2] = CONFIG_VERSION[2];
  _vars.version[3] = CONFIG_VERSION[3];
  // The default values
  _vars.lcdBrightness = 120;
  _vars.lcdTimeout = 30;
  _vars.midTempRatio = 35; //35%
  _vars.midTempHoursDuration = 10; //hours
  _vars.maxTargetTemp = 2260; //22.6*C
  _vars.minTargetTemp = 1530; //15.3*C
  _vars.maxTargetTimeHours = 13; //encoding 13:00
  
  _vars.relayOnDayPercent = 50;
//  _vars.maxTargetTimeMinutes = 40;
//  _vars.minTargetTimeHours = 4; //encoding 4:25
//  _vars.minTargetTimeMinutes = 25;
}

SettingsClass Settings;
