#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
    Serial.begin(115200);
    Wire.begin();

    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050/MPU9250 sensor!");
        while (1);
    }

    Serial.println("MPU6050/MPU9250 Initialized");

    // Configure sensor settings
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    Serial.print("Accel X: "); Serial.print(a.acceleration.x); Serial.print(" m/s², ");
    Serial.print("Y: "); Serial.print(a.acceleration.y); Serial.print(" m/s², ");
    Serial.print("Z: "); Serial.print(a.acceleration.z); Serial.println(" m/s²");

    Serial.print("Gyro X: "); Serial.print(g.gyro.x); Serial.print(" rad/s, ");
    Serial.print("Y: "); Serial.print(g.gyro.y); Serial.print(" rad/s, ");
    Serial.print("Z: "); Serial.print(g.gyro.z); Serial.println(" rad/s");

    Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" °C");
    Serial.println("----------------------------");

    delay(500);
}