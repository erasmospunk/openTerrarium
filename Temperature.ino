/**************************************************
 * class: Temperature
 * constructor: initTempSensor()
 *
 * methods:
 **************************************************/



// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature tempSensor(&oneWire);

// arrays to hold device address
DeviceAddress tAddr;

void initTempSensor() {
  tempSensor.begin();
  //tempSensor.setWaitForConversion(false);
  
  if (!tempSensor.getAddress(tAddr, 0)) {
    Serial.println("Unable to find address for Device 0"); 
  }
  
  printAddress(tAddr);
  
  requestTemperatures();
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  Serial.print("Found device with id: ");
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
  Serial.println();
}


float getTemperature() {
  float lastTempSensor = getTempC();
  
  requestTemperatures();
  
  return lastTempSensor;
}

void requestTemperatures() {
//  Serial.print("requesting temp... ");  
  tempSensor.requestTemperatures();
}

float getTempC() {
  float lastTempSensor = tempSensor.getTempC(tAddr);

  Serial.print(", temp = ");  
  Serial.print(lastTempSensor);
//  Serial.println("* C");
  
  return lastTempSensor;
}
