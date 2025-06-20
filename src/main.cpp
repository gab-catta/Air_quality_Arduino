#include <Arduino.h>
#include "SoftwareSerial.h"
#include "BLINKER_PMSX003ST.h"


// PMS7003T sensor setup (via logic converter)
SoftwareSerial pmsSerial(2, 3); // RX, TX
BLINKER_PMSX003ST pms;
// MH-Z19E CO2 sensor setup
SoftwareSerial co2Serial(4, 5); // RX, TX

// Data structure to hold all sensor readings
struct AirQualityData {
  // PMS7003T data
  float pm1_0;
  float pm2_5;
  float pm10_0;
  float temperature;
  float humidity;
  
  // MH-Z19E data
  float co2;
  
  // Timestamp
  unsigned long timestamp;
};

AirQualityData currentData;

// Timing variables
unsigned long lastPMSRead = 0;
unsigned long lastCO2Read = 0;
const unsigned long PMS_READ_INTERVAL = 30000; // 30 seconds
const unsigned long CO2_READ_INTERVAL = 10000; // 10 seconds

// CO2 sensor command bytes
byte co2Request[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};




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
  }
return status_pms;
}

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
    return status_co2;
  }
}

void assessAirQuality(String status_pms, String status_co2) {
  if (status_pms == "Failed"){
    Serial.println("PM assessment not possible since the sensor didn't work");
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

  if (status_pms == "Failed"){
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
  else {
    Serial.println("\n--- Particulate Matter (μg/m³) ---");
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

  if (status_co2 == "Failed"){
    Serial.println("CO2 sensor failed to retrieve data!!!!!!!!!!!!");
  }
  else{
    Serial.println("\n--- CO2 Levels ---");
    Serial.print("CO2: ");
    Serial.print(currentData.co2);
    Serial.println(" ppm");
  }

  
  // Air quality assessment
  Serial.println("\n--- Air Quality Assessment ---");
  assessAirQuality(status_pms,status_co2);
  Serial.println("==================================================================");
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("/----------------------Air Quality Monitor Starting...-----------------------/");
  
  // Initialize PMS7003T sensor
  pmsSerial.begin(9600);
  pms.begin(pmsSerial);
  Serial.println("PMS7003T sensor initialized");
  
  // Initialize MH-Z19E sensor
  co2Serial.begin(9600);
  Serial.println("MH-Z19E sensor initialized");
  
  // Wait for sensors to warm up
  Serial.println("Warming up sensors (30 seconds)...");
  delay(30000);
  
  Serial.println("/------------------------Air Quality Monitor Ready!-------------------------/");
}


void loop() {
  unsigned long currentTime = millis();
  String status_pms = "Failed";
  String status_co2 = "Failed";

  // Read PMS7003T data every 30 seconds
  if (currentTime - lastPMSRead >= PMS_READ_INTERVAL) {
    status_pms = readPMSData();
    lastPMSRead = currentTime;
  }
  // Read CO2 data every 10 seconds
  if (currentTime - lastCO2Read >= CO2_READ_INTERVAL) {
    status_co2 = readCO2Data();
    lastCO2Read = currentTime;
  }
  
  // Display data every 5 seconds
  static unsigned long lastDisplay = 0;
  if (currentTime - lastDisplay >= 7000) {
    displayData(status_pms,status_co2);
    lastDisplay = currentTime;
    currentData.timestamp = lastDisplay;
  }
  
  delay(1000); // Small delay to prevent overwhelming the system
}