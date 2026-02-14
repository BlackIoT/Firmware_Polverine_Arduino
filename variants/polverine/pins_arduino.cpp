
#include "pins_arduino.h"
#include "Arduino.h"

void initVariant() {
    pinMode(RED_LED_PIN, OUTPUT);      // Set OUTPUT PIN
    digitalWrite(RED_LED_PIN, LOW);    // LED off
    pinMode(GREEN_LED_PIN, OUTPUT);    // Set OUTPUT PIN
    digitalWrite(GREEN_LED_PIN, LOW);  // LED off
    pinMode(BLUE_LED_PIN, OUTPUT);     // Set OUTPUT PIN
    digitalWrite(BLUE_LED_PIN, LOW);   // LED off
}
