#define PULSE_SENSOR_PIN 2  // GPIO 4 (A2)
#define LED_PIN 4 // Built-in LED


const int numReadings = 10; // Number of samples for averaging
int readings[numReadings];  // Array to store last readings
int readIndex = 0;          // Index in the readings array
int total = 0;              // Running sum of readings
int averageSignal = 0;      // Smoothed signal value

int Threshold = 550;   // Default threshold (updated dynamically)
bool lastBeatState = false;
unsigned long lastBeatTime = 0;
unsigned long beatInterval = 0;
float BPM = 0;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);

    // Initialize the readings array
    for (int i = 0; i < numReadings; i++) {
        readings[i] = analogRead(PULSE_SENSOR_PIN);
        total += readings[i];
    }
}

void loop() {
    // Read new sensor value
    int rawSignal = analogRead(PULSE_SENSOR_PIN);

    // Update moving average filter
    total -= readings[readIndex];   // Remove oldest value
    readings[readIndex] = rawSignal; // Add new value
    total += readings[readIndex];   // Update sum
    readIndex = (readIndex + 1) % numReadings; // Move index
    averageSignal = total / numReadings;  // Compute average

    // Auto-adjust threshold based on baseline
    Threshold = averageSignal + 30; // Adjust as needed

    Serial.print("Filtered Signal: ");
    Serial.print(averageSignal);

    // Detect heartbeat (rising edge detection)
    bool currentBeatState = (averageSignal > Threshold);
    if (currentBeatState && !lastBeatState) {
        // Beat detected, calculate time interval
        unsigned long currentTime = millis();
        beatInterval = currentTime - lastBeatTime;
        lastBeatTime = currentTime;

        if (beatInterval > 300 && beatInterval < 2000) { // Ignore noise
            BPM = 60000.0 / beatInterval; // Convert time to BPM
        }

        digitalWrite(LED_PIN, HIGH); // Blink LED on beat
    } else {
        digitalWrite(LED_PIN, LOW);
    }
    
    lastBeatState = currentBeatState; // Store last state

    // Print BPM
    Serial.print(" | BPM: ");
    Serial.println(BPM);

    delay(10);  // Short delay for stability
}
