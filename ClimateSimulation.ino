
#include "Constants.h"
#include <Time.h>


//Settings.lcdBrightness = 120;
//Settings.lcdTimeout = 10;
//Settings.maxTargetTemp = 2260; //22.6*C
//Settings.minTargetTemp = 1530; //15.3*C
//Settings.maxTargetTimeHours = 13; //encoding 13:40
//Settings.maxTargetTimeMinutes = 40;
//Settings.minTargetTimeHours = 4; //encoding 4:25
//Settings.minTargetTimeMinutes = 25;

time_t lastMinTempTime;

float getSimulateClimateTemperature() {
  float maxC = Settings.getMaxTargetTempFloat();
  float minC = Settings.getMinTargetTempFloat();
  
  float targetTemp = getSimulateClimationCosine(minC, maxC);
  
  Serial.print(", targetTemp = ");Serial.print(targetTemp);
  
  return targetTemp;
}



float getSimulateClimationCosine(float minC, float maxC) {
   
  time_t xTime = now() - Settings.getMinTargetTimeHour() * SECS_PER_HOUR;
 
  return (maxC - minC)/2 * (1 - cos(2 * PI * xTime / SECS_PER_DAY)) + minC;
}
