#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Wire.h>
#include <MPU6050.h>
#include <HardwareSerial.h>

#define DEVICE_NAME "XIAO-ESP32C3"
#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;
bool deviceConnected = false;

// MPU6050
MPU6050 mpu;
unsigned long fallStartTime = 0;
bool fallDetected = false;

// PPG & SOS Button
#define PPG_PIN 2   // GPIO 2 I'm using this Analog Pin A0 is needed
#define SOS_PIN 4   // GPIO 4 

unsigned long ppgStartTime = 0;
bool ppgTriggered = false;

// GPS Module
HardwareSerial GPS(1);
String gpsData = "";

// BLE Callbacks
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override {
        deviceConnected = true;
        Serial.println("BLE Client Connected!");
    }

    void onDisconnect(BLEServer* pServer) override {
        deviceConnected = false;
        Serial.println("BLE Client Disconnected. Restarting advertisement...");
        BLEDevice::startAdvertising();
    }
};

// Initialize all sensors
void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE...");

    // Initialize BLE
    BLEDevice::init(DEVICE_NAME);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create BLE Service & Characteristic
    BLEService* pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY
    );

    pService->start();  
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();
    Serial.println("BLE Ready. Waiting for connections...");

    // Initialize MPU6050
    Wire.begin();
    mpu.initialize();
    if (!mpu.testConnection()) {
        Serial.println("MPU6050 connection failed!");
        while (1);
    }
    Serial.println("MPU6050 initialized successfully.");

    // Initialize PPG & SOS button
    pinMode(PPG_PIN, INPUT);
    pinMode(SOS_PIN, INPUT_PULLUP);

    // Initialize GPS
    GPS.begin(9600, SERIAL_8N1, 3, 1);  // TX=3, RX=1
}

// Check MPU6050 for fall detection
void checkFall() {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    float roll = atan2(ay, az) * 180.0 / M_PI;

    if (abs(roll) < 85) {
        fallStartTime = 0;
        fallDetected = false;
        sendBLE("Standing");
    } else {
        if (fallStartTime == 0) {
            fallStartTime = millis();
        } else if (millis() - fallStartTime > 5000) {
            fallDetected = true;
        }

        if (fallDetected) {
            sendBLE("Fall");
        }
    }
}

// Check PPG sensor for abnormal heart rate spikes
void checkPPG() {
    int ppgValue = analogRead(PPG_PIN);
    if (ppgValue > 800) {  // Threshold for spike
        if (ppgStartTime == 0) {
            ppgStartTime = millis();
        } else if (millis() - ppgStartTime > 5000) {  // 5 sec condition
            ppgTriggered = true;
        }
    } else {
        ppgStartTime = 0;
        ppgTriggered = false;
    }

    if (ppgTriggered) {
        sendBLE("P");
        ppgTriggered = false;
    }
}

// Check GPS module and send coordinates
void checkGPS() {
    if (GPS.available()) {
        char c = GPS.read();
        if (c == '\n') {
            sendBLE(gpsData);
            gpsData = "";  // Reset
        } else {
            gpsData += c;
        }
    }
}

// Check SOS button press
void checkSOS() {
    if (digitalRead(SOS_PIN) == LOW) {
        sendBLE("SOS");
    }
}

// Function to send BLE messages
void sendBLE(String message) {
    if (deviceConnected) {
        pCharacteristic->setValue(message.c_str());
        pCharacteristic->notify();
        Serial.println("Sent: " + message);
    }
}

// Main loop
void loop() {
    checkFall();
    checkPPG();
    checkGPS();
    checkSOS();
    delay(500);
}
