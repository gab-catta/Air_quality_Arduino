# Air Quality Monitor with Arduino UNO (WORK IN PROGRESS, blocked till MH-Z19E sensor doesn't arrive)

A real-time air quality monitoring system using Arduino UNO, PMS7003T sensor (particulates, humidity, temperature), and MH-Z19E CO2 sensor.

## Components Required

- **Arduino UNO R3**
- **PMS7003T** - Air quality sensor (PM1.0, PM2.5, PM10.0, Temperature, Humidity)
- **MH-Z19E** - CO2 sensor
- **Logic Level Converter** - For PMS7003T (3.3V to 5V conversion)
- **Breadboard**
- **Jumper wires**
- **USB cable** for Arduino

## Wiring Diagram

### Detailed ASCII Wiring Diagram
```
                    ARDUINO UNO
                ┌─────────────────┐
                │                 │
                │  ┌─────────────┐ │
                │  │   DIGITAL   │ │
                │  │             │ │
                │  │ 2 ──────────┼─┼─── PMS7003T TX (via Logic Converter)
                │  │ 3 ──────────┼─┼─── PMS7003T RX (via Logic Converter)
                │  │ 4 ──────────┼─┼─── MH-Z19E TX
                │  │ 5 ──────────┼─┼─── MH-Z19E RX
                │  │             │ │
                │  └─────────────┘ │
                │                 │
                │  ┌─────────────┐ │
                │  │   POWER     │ │
                │  │             │ │
                │  │ 5V ─────────┼─┼─── Logic Converter LV
                │  │ 3.3V ───────┼─┼─── Logic Converter HV
                │  │ GND ────────┼─┼─── Logic Converter GND
                │  │             │ │
                │  └─────────────┘ │
                └─────────────────┘
                         │
                         │
                    ┌────┴────┐
                    │         │
                    │ 5V ─────┼─── MH-Z19E VCC
                    │ GND ────┼─── MH-Z19E GND
                    │         │
                    └─────────┘

                LOGIC LEVEL CONVERTER
                ┌─────────────────────┐
                │                     │
                │  HV (3.3V) Side     │
                │  ┌───────────────┐  │
                │  │ HV1 ──────────┼──┼─── PMS7003T TX
                │  │ HV2 ──────────┼──┼─── PMS7003T RX
                │  │ HV ───────────┼──┼─── 3.3V Power
                │  │ GND ──────────┼──┼─── Ground
                │  └───────────────┘  │
                │                     │
                │  LV (5V) Side       │
                │  ┌───────────────┐  │
                │  │ LV1 ──────────┼──┼─── Arduino Pin 2
                │  │ LV2 ──────────┼──┼─── Arduino Pin 3
                │  │ LV ───────────┼──┼─── 5V Power
                │  │ GND ──────────┼──┼─── Ground
                │  └───────────────┘  │
                └─────────────────────┘

                PMS7003T SENSOR
                ┌─────────────────┐
                │                 │
                │  ┌─────────────┐ │
                │  │   PINS      │ │
                │  │             │ │
                │  │ TX ─────────┼─┼─── Logic Converter HV1
                │  │ RX ─────────┼─┼─── Logic Converter HV2
                │  │ VCC ────────┼─┼─── Logic Converter HV (3.3V)
                │  │ GND ────────┼─┼─── Logic Converter GND
                │  │             │ │
                │  └─────────────┘ │
                └─────────────────┘

                MH-Z19E SENSOR
                ┌─────────────────┐
                │                 │
                │  ┌─────────────┐ │
                │  │   PINS      │ │
                │  │             │ │
                │  │ TX ─────────┼─┼─── Arduino Pin 4
                │  │ RX ─────────┼─┼─── Arduino Pin 5
                │  │ VCC ────────┼─┼─── 5V Power
                │  │ GND ────────┼─┼─── Ground
                │  │             │ │
                │  └─────────────┘ │
                └─────────────────┘
```

### Connection Summary

#### PMS7003T (via Logic Level Converter)
```
PMS7003T    Logic Converter    Arduino UNO
TX    --->  HV1 (3.3V side)   --->  Pin 2 (RX)
RX    <---  HV2 (3.3V side)   <---  Pin 3 (TX)
VCC   --->  HV (3.3V side)    --->  3.3V
GND   --->  GND (3.3V side)   --->  GND

Logic Converter    Arduino UNO
LV1 (5V side)  --->  Pin 2 (RX)
LV2 (5V side)  <---  Pin 3 (TX)
LV             --->  5V
GND            --->  GND
```

#### MH-Z19E CO2 Sensor
```
MH-Z19E      Arduino UNO
TX    --->  Pin 4 (RX)
RX    <---  Pin 5 (TX)
VCC   --->  5V
GND   --->  GND
```

## Pin Summary
- **Pin 2**: PMS7003T TX (via logic converter)
- **Pin 3**: PMS7003T RX (via logic converter)
- **Pin 4**: MH-Z19E TX
- **Pin 5**: MH-Z19E RX
- **5V**: Power for both sensors
- **GND**: Ground for both sensors

## Required Libraries

You need to install these libraries in your Arduino IDE:

1. **PMS Library** by Joel Gaehwiler
   - Go to Tools > Manage Libraries
   - Search for "PMS"
   - Install "PMS" by Joel Gaehwiler

2. **SoftwareSerial** (built into Arduino)

## Setup Instructions

1. **Install Libraries**: Open Arduino IDE and install the required libraries
2. **Connect Components**: Follow the wiring diagram above
3. **Upload Code**: Upload `air_quality_monitor.ino` to your Arduino
4. **Open Serial Monitor**: Set baud rate to 9600
5. **Wait for Warm-up**: The sensors need 30 seconds to warm up

## Features

### Real-time Monitoring
- **PM1.0, PM2.5, PM10.0** particulate matter levels (μg/m³)
- **Temperature** and **Humidity** from PMS7003T
- **CO2** concentration (ppm) from MH-Z19E
- **Air Quality Assessment** based on EPA standards

### Data Intervals
- **PMS7003T**: Reads every 30 seconds
- **MH-Z19E**: Reads every 10 seconds
- **Display**: Updates every 5 seconds

### Air Quality Standards

#### PM2.5 (EPA Standards)
- **0-12 μg/m³**: Good
- **12.1-35.4 μg/m³**: Moderate
- **35.5-55.4 μg/m³**: Unhealthy for Sensitive Groups
- **55.5-150.4 μg/m³**: Unhealthy
- **150.5-250.4 μg/m³**: Very Unhealthy
- **>250.4 μg/m³**: Hazardous

#### CO2 Levels
- **≤400 ppm**: Excellent
- **401-1000 ppm**: Good
- **1001-2000 ppm**: Moderate (consider ventilation)
- **2001-5000 ppm**: Poor (ventilation required)
- **>5000 ppm**: Very Poor (immediate ventilation needed)

## Troubleshooting

### Common Issues

1. **"Failed to read PMS7003T data"**
   - Check wiring connections
   - Ensure logic converter is properly connected
   - Verify sensor is powered (3.3V)

2. **"CO2 sensor timeout"**
   - Check wiring connections
   - Ensure sensor is powered (5V)
   - Try power cycling the sensor

3. **Incorrect readings**
   - Allow sensors to warm up (30+ seconds)
   - Check for interference from other devices
   - Verify baud rate settings

### Logic Level Converter Setup
The PMS7003T operates at 3.3V while Arduino UNO uses 5V logic. The logic converter:
- Converts 3.3V signals from PMS7003T to 5V for Arduino
- Converts 5V signals from Arduino to 3.3V for PMS7003T

## Data Output Example

```
=== AIR QUALITY DATA ===
Timestamp: 45 seconds

--- Particulate Matter (μg/m³) ---
PM1.0: 8
PM2.5: 12
PM10.0: 18

--- Environmental Conditions ---
Temperature: 22.5 °C
Humidity: 45.2 %

--- CO2 Levels ---
CO2: 450 ppm

--- Air Quality Assessment ---
PM2.5 Quality: Good
CO2 Quality: Good
==================================
```

## Power Requirements

- **Arduino UNO**: 5V via USB or external power supply
- **PMS7003T**: 3.3V (via logic converter)
- **MH-Z19E**: 5V
- **Logic Converter**: 3.3V and 5V

## Calibration Notes

- **PMS7003T**: Factory calibrated, no additional calibration needed
- **MH-Z19E**: May need calibration in outdoor air (400 ppm baseline)
- Allow sensors to stabilize for 24-48 hours for best accuracy

## Future Enhancements

- Add LCD display for standalone operation
- Implement data logging to SD card
- Add WiFi module for remote monitoring
- Create web dashboard for data visualization
- Add alerts for poor air quality conditions 
