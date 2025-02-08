#define LED_PIN 2    // LED connected to GPIO 2 (D0)
#define BUTTON_PIN 4 // Button connected to GPIO 4 (D2)

void setup() {
    pinMode(LED_PIN, OUTPUT);      // Set LED as output
    pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button as input with pull-up resistor
}

void loop() {
    int buttonState = digitalRead(BUTTON_PIN); // Read button state

    if (buttonState == LOW) { // Button pressed (LOW because of pull-up)
        digitalWrite(LED_PIN, HIGH); // Turn on LED
    } else {
        digitalWrite(LED_PIN, LOW); // Turn off LED
    }
}
