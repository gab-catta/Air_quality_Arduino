#include <Arduino.h>
#include "SoftwareSerial.h"
#include "BLINKER_PMSX003ST.h"
#include "air_quality_data.h"
#include "sensor_reader.h"

AirQualityData currentData;

// CO2 sensor command bytes
byte co2Request[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

void assessAirQuality(String status_pms, String status_co2) {
  if (status_pms == "Failed" && currentData.pm1_0 == 0){
    Serial.println("PM1.0 assessment not possible since the sensor didn't work");
  }
  else {
    // PM1.0 assessment
    Serial.print("PM2.5 Quality: ");
    if (currentData.pm1_0 <= 12) {
      Serial.println("Good");
    } else if (currentData.pm1_0 <= 35.4) {
      Serial.println("Moderate");
    } else if (currentData.pm1_0 <= 55.4) {
      Serial.println("Unhealthy for Sensitive Groups");
    } else if (currentData.pm1_0 <= 150.4) {
      Serial.println("Unhealthy");
    } else if (currentData.pm1_0 <= 250.4) {
      Serial.println("Very Unhealthy");
    } else {
      Serial.println("Hazardous");
    }
  }

  if (status_pms == "Failed" && currentData.pm2_5 == 0){
    Serial.println("PM2.5 assessment not possible since the sensor didn't work");
  }
  else {
    // PM2.5 assessment
    Serial.print("PM2.5 Quality: ");
    if (currentData.pm2_5 <= 12) {
      Serial.println("Good");
    } else if (currentData.pm2_5 <= 35.4) {
      Serial.println("Moderate");
    } else if (currentData.pm2_5 <= 55.4) {
      Serial.println("Unhealthy for Sensitive Groups");
    } else if (currentData.pm2_5 <= 150.4) {
      Serial.println("Unhealthy");
    } else if (currentData.pm2_5 <= 250.4) {
      Serial.println("Very Unhealthy");
    } else {
      Serial.println("Hazardous");
    }
  }

  if (status_pms == "Failed" && currentData.pm10_0 == 0){
    Serial.println("PM10.0 assessment not possible since the sensor didn't work");
  }
  else {
    // PM10.0 assessment
    Serial.print("PM10.0 Quality: ");
    if (currentData.pm10_0 <= 12) {
      Serial.println("Good");
    } else if (currentData.pm10_0 <= 35.4) {
      Serial.println("Moderate");
    } else if (currentData.pm10_0 <= 55.4) {
      Serial.println("Unhealthy for Sensitive Groups");
    } else if (currentData.pm10_0 <= 150.4) {
      Serial.println("Unhealthy");
    } else if (currentData.pm10_0 <= 250.4) {
      Serial.println("Very Unhealthy");
    } else {
      Serial.println("Hazardous");
    }
  }

  if (status_co2 == "Failed" && currentData.co2 == 0){
    Serial.println("CO2 assessment not possible since the sensor didn't work");
  }
  else {
    // CO2 assessment
    Serial.print("CO2 Quality: ");
    if (currentData.co2 <= 400) {
      Serial.println("Excellent");
    } else if (currentData.co2 <= 1000) {
      Serial.println("Good");
    } else if (currentData.co2 <= 2000) {
      Serial.println("Moderate - Consider ventilation");
    } else if (currentData.co2 <= 5000) {
      Serial.println("Poor - Ventilation required");
    } else {
      Serial.println("Very Poor - Immediate ventilation needed");
    }
  }

} 


void displayData(String status_pms, String status_co2) {
  Serial.println("\n============ AIR QUALITY DATA ===============");
  Serial.print("Timestamp: ");
  Serial.print(currentData.timestamp / 1000);
  Serial.println(" seconds");
  
  if (status_pms == "Failed") {
    Serial.println("PMS sensor failed to retrieve data!!!!!!!!!!!!");
  }
  else if (status_pms != "Failed") {
    Serial.println("\n--- Particulate Matter (ug/m^3) ---");
    Serial.print("PM1.0: ");
    Serial.println(currentData.pm1_0);
    Serial.print("PM2.5: ");
    Serial.println(currentData.pm2_5);
    Serial.print("PM10.0: ");
    Serial.println(currentData.pm10_0);
    
    Serial.println("\n--- Environmental Conditions ---");
    Serial.print("Temperature: ");
    Serial.print(currentData.temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(currentData.humidity);
    Serial.println(" %");
  }
  else if (status_co2 == "Failed"){
    Serial.println("CO2 sensor failed to retrieve data!!!!!!!!!!!!");
  }
  else if (status_co2 != "Failed") {
    Serial.println("\n--- CO2 Levels ---");
    Serial.print("CO2: ");
    Serial.print(currentData.co2);
    Serial.println(" ppm");
  }
  
  Serial.println("\n--- Air Quality Assessment ---");
  assessAirQuality(status_pms,status_co2);
  Serial.println("\n============================NEW READ======================================");
}

void setup() {

  Serial.begin(9600);
  // Initialize PMS7003T sensor
  pmsSerial.begin(9600);
  delay(2000);
  pms.begin(pmsSerial);
  Serial.println("PMS7003T sensor initialized");
  // Initialize MH-Z19E sensor
  co2Serial.begin(9600);
  delay(2000);
  Serial.println("MH-Z19E sensor initialized");
  
  Serial.println("\n/----------------------Air Quality Monitor Starting-----------------------/");
  Serial.println("Warming up sensors (30 seconds)...");
  delay(30000);
  
  Serial.println("/------------------------Air Quality Monitor Ready!-------------------------/");
}

// Timing variables
unsigned long lastPMSRead = 0;
unsigned long lastCO2Read = 0;
const unsigned long PMS_READ_INTERVAL = 10000; 
const unsigned long CO2_READ_INTERVAL = 10000; 
// Status sensors 
String lastStatusPMS = "Failed";
String lastStatusCO2 = "Failed";

void loop() {
  unsigned long currentTime = millis();

  // Read PMS7003T data every 10 seconds
  if (currentTime - lastPMSRead >= PMS_READ_INTERVAL) {
    lastStatusPMS = readPMSData();
    lastPMSRead = currentTime;
  }
  // Read CO2 data every 10 seconds
  if (currentTime - lastCO2Read >= CO2_READ_INTERVAL) {
    lastStatusCO2 = readCO2Data();
    lastCO2Read = currentTime;
  }
  
  // Display data every 5 seconds
  static unsigned long lastDisplay = 0;
  if (currentTime - lastDisplay >= 7000) {
    displayData(lastStatusPMS,lastStatusCO2);
    lastDisplay = currentTime;
    currentData.timestamp = lastDisplay;
  }
  
  delay(100);
}