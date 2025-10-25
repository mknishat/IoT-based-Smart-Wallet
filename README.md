# IoT-based-Smart-Wallet
This project implements a Smart Wallet that automatically recognizes Bangladeshi paper currency, detects cash inflow/outflow, and maintains a real-time balance. It displays the current balance on a 16×2 I²C LCD and simultaneously uploads structured JSON telemetry to Google Sheets (cloud logging) and a mobile app for history and alerts. 
Core Features

BDT Note Recognition: Color sensing (TCS3200) maps note RGB profiles (2/5/10/20/50/100/500/1000).

Direction-Aware Counting: Dual HC-SR04 ultrasonic sensors infer in vs out movements.

Live Display: 16×2 I²C LCD shows current transaction and wallet balance.

Cloud Sync: ESP8266 posts JSON payloads to Google Sheets (every ~10s) for logging/analytics.

Mobile App View: See current balance and last month’s transactions; low-balance notifications.

Lightweight Architecture: Arduino handles sensing + JSON serialization; ESP8266 handles Wi-Fi + upload. 

Manuscript

System Architecture

Sensing → Processing → Internet → Cloud/App

Sensors: TCS3200 (RGB), two HC-SR04 (distance).

MCU: Arduino Mega 2560 (sensor fusion, JSON build).

Connectivity: ESP-12E (ESP8266) for Wi-Fi and HTTP post.

UI: 16×2 I²C LCD (PCF8574).

Cloud/App: Google Sheets (via Apps Script endpoint) + mobile app dashboard. 

Manuscript

Algorithm (High Level)

Detect presence at Sensor-1 / Sensor-2 (distance ≤ ~20 cm).

Read currency RGB via TCS3200 → classify denomination.

If inflow (S1=hit, S2=clear): add value; if outflow (S2=hit, S1=clear): subtract value; otherwise no change.

Update LCD (last note, new balance).

Serialize to JSON on Arduino → send to ESP8266 → upload to cloud at fixed intervals. 

Manuscript

Bill of Materials

Arduino Mega 2560

ESP-12E / NodeMCU V3 (ESP8266)

TCS3200 Color Sensor

HC-SR04 Ultrasonic Sensors ×2

16×2 LCD + I²C (PCF8574) Adapter

Breadboard, jumper wires, 5 V supply/cell pack.
