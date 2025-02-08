# S.H.A.K.T.I - Safety Help Alert Key Tracker with Instant Response

### 🚨 Why This Project?
Every year, thousands of **rape cases** and **assault incidents** occur worldwide. Many locations have become **hotspots for crime**, making personal safety a major concern. **S.H.A.K.T.I** is designed to **empower individuals** with a smart, wearable safety device that provides real-time alerts and tracking.

---

##  Features
**Fall & Assault Detection** (MPU6050)  
**Heart Rate Monitoring** (PPG Sensor)  
**SOS Emergency Alerts** via BLE  
**Location Tracking** via GPS  
**Buzzer Alarm** for immediate response  
**Safe/Unsafe Area Categorization**  
**Bluetooth Connectivity** with Mobile App  

---

## Tech Stack
- **Hardware:** ESP32, MPU6050, PPG Sensor, GPS Module, Buzzer  
- **Software:** Arduino, MIT App Inventor (for mobile app)  
- **Communication:** Bluetooth Low Energy (BLE)  

---

##  Required Libraries  
Make sure to install the following **Arduino libraries** before uploading the code:  

| Library Name | Purpose | Installation Command |
|-------------|---------|----------------------|
| **Wire.h** | Communicates with MPU6050 via I2C | Included by default |
| **Adafruit_MPU6050.h** | Reads accelerometer and gyroscope data | `Arduino Library Manager` |
| **Adafruit_Sensor.h** | Provides sensor data handling | `Arduino Library Manager` |
| **BLEDevice.h** | Enables Bluetooth Low Energy (BLE) communication | `Arduino Library Manager` |
| **BLEServer.h** | Sets up BLE server for communication | `Arduino Library Manager` |
| **BLEUtils.h** | Provides BLE utilities | `Arduino Library Manager` |
| **BLE2902.h** | Configures BLE properties | `Arduino Library Manager` |

---



