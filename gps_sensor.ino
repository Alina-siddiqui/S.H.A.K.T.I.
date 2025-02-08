#include <TinyGPS++.h>
#include <HardwareSerial.h>

static const int RXPin = 16, TXPin = 17;  // ESP32 UART2 pins
static const uint32_t GPSBaud = 9600;     // Default GPS baud rate

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);  // Use UART2

void setup() {
    Serial.begin(115200);
    gpsSerial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);  // Initialize GPS module

    Serial.println("GPS Module Initialized");
}

void loop() {
    while (gpsSerial.available()) {
        gps.encode(gpsSerial.read());  // Parse GPS data

        if (gps.location.isUpdated()) {
            Serial.print("Latitude: "); Serial.println(gps.location.lat(), 6);
            Serial.print("Longitude: "); Serial.println(gps.location.lng(), 6);
            Serial.print("Satellites: "); Serial.println(gps.satellites.value());
            Serial.print("Speed (km/h): "); Serial.println(gps.speed.kmph());
            Serial.print("Altitude (m): "); Serial.println(gps.altitude.meters());
            Serial.println("--------------------");
        }
    }
}