#include <Arduino.h>
#include "SoftwareSerial.h"
#include "BLINKER_PMSX003ST.h"
#include "air_quality_data.h"

extern AirQualityData currentData;
extern byte co2Request[];

// PMS7003T sensor setup (via logic converter)
SoftwareSerial pmsSerial(2, 3); // RX, TX
BLINKER_PMSX003ST pms;
// MH-Z19E CO2 sensor setup
SoftwareSerial co2Serial(4, 5); // RX, TX



String readPMSData() {
  String status_pms = "";
  Serial.println("Reading PMS7003T data...");
  
  if (pms.read()) {
    currentData.pm1_0 = pms.getPcs(1.0);   
    currentData.pm2_5 = pms.getPcs(2.5);    
    currentData.pm10_0 = pms.getPcs(10.0);  
    currentData.temperature = pms.getTemp();
    currentData.humidity = pms.getHumi();
    currentData.timestamp = millis();
    Serial.println("PMS7003T data updated successfully");
  } 
  else {
    Serial.println("Failed to read PMS7003T data");
    status_pms = "Failed";
    currentData.pm1_0 = 0;
    currentData.pm2_5 = 0;
    currentData.pm10_0 = 0;
    currentData.temperature = -100;
    currentData.humidity = -1;
  }
  return status_pms;
}


//CHECK for a MH-Z19E library 
String readCO2Data() {
  String status_co2 = "";
  Serial.println("Reading CO2 data...");
  
  // Send request to CO2 sensor
  co2Serial.write(co2Request, 9);
  
  // Wait for response
  unsigned long startTime = millis();
  while (co2Serial.available() < 9) {
    if (millis() - startTime > 1000) {
      Serial.println("CO2 sensor timeout");
      status_co2 = "Failed";
      currentData.co2 = 0;
      return status_co2;
    }
  }
  
  // Read response
  byte response[9];
  co2Serial.readBytes(response, 9);
  
  // Validate response
  if (response[0] == 0xFF && response[1] == 0x86) {
    // Calculate CO2 concentration
    currentData.co2 = (response[2] << 8) + response[3];
    Serial.println("CO2 data updated successfully");
    return status_co2;
  } 
  else {
    Serial.println("Invalid CO2 sensor response");
    status_co2 = "Failed";
    currentData.co2 = 0;
    return status_co2;
  }
}