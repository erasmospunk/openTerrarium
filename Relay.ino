/**************************************************
 * class: Relay
 * constructor: initRelay()
 *
 * methods:
 *   setTargetTemp(float temp)
 *   controlRelay(float currentTemp)
 **************************************************/
#include <PID_v1.h>

#define RELAY_ON HIGH
#define RELAY_OFF LOW

double Input, Output;
double targetTemp = 14;
double adjustmentTemp = - 0.8;

PID myPID(&Input, &Output, &targetTemp, 2, 0.25, 1, DIRECT);

unsigned long timeRelayChanged = 0;

float tempDeviation = 0.5;
static byte relayStatus;

void initRelay() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  relayStatus = RELAY_OFF;
  
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0,1);
}

void controlTimedRelay(float thresholdOn) {
  
  float currentPercent = getSimulateClimationCosine(0, 1);
  
  
  Serial.print(", currentPercent = ");
  Serial.print(currentPercent);
  
  if (currentPercent >= thresholdOn) {
    relay(RELAY_ON);
  }
  else {
    relay(RELAY_OFF);
  } 
}

void increaseTargetTemp(float byFactor) {
  targetTemp += byFactor;
  if (targetTemp > 30) {
    targetTemp = 30;
  }
  else if (targetTemp < 10) {
    targetTemp = 10;
  }
  
  setTargetTemp(targetTemp);
}

float getTargetTemp() {
  return targetTemp;
}

void setTargetTemp(float temp) {
  targetTemp = temp;
}

void controlRelay(float currentTemp) {
  _controlRelay(currentTemp);
  Serial.print(", relayStatus = ");
  Serial.print(relayStatus, DEC);
  
  return;
  
  Input = currentTemp;
  myPID.Compute();
  
  Serial.print(", pid = ");
  Serial.print(Output*100);

  if (Output > 0.5) {
    relay(RELAY_ON);
  }
  else {
    relay(RELAY_OFF);
  }
}

void _controlRelay(float currentTemp) {
  float differenceTemp = currentTemp - targetTemp - adjustmentTemp;

  unsigned long time = millis();
  //avoid time overflow issues
  if (time < timeRelayChanged) {
    timeRelayChanged = 0;
  }
  
  //avoid fast relay on/off. delay states switches for 2s
  if (time - timeRelayChanged < 2000) {
    return;
  }
  
  

  if (differenceTemp > tempDeviation && relayStatus == RELAY_ON) { //temp too high, turn off heat
    relay(RELAY_OFF);
    
    melody[0] = NOTE_GS1;
    medody();
  }
  else if (differenceTemp < (-1)*tempDeviation && relayStatus == RELAY_OFF) { //temp too low, turn on heat
    relay(RELAY_ON);
    
    melody[0] = NOTE_GS3;
    medody();
  }
}


void relay(byte on) {
  if (on != RELAY_ON && on != RELAY_OFF) { //wrong value
    return;
  }
    
  relayStatus = on;
  digitalWrite(RELAY_PIN, relayStatus);
  timeRelayChanged = millis();
}
