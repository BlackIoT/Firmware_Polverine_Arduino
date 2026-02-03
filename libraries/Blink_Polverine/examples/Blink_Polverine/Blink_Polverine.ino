

void setup() {
  pinMode(RED_LED_PIN, OUTPUT);      // Set OUTPUT PIN
  digitalWrite(RED_LED_PIN, LOW);    // LED off
  pinMode(GREEN_LED_PIN, OUTPUT);    // Set OUTPUT PIN
  digitalWrite(GREEN_LED_PIN, LOW);  // LED off
  pinMode(BLUE_LED_PIN, OUTPUT);     // Set OUTPUT PIN
  digitalWrite(BLUE_LED_PIN, LOW);   // LED off
}

void loop() {
  digitalWrite(RED_LED_PIN, HIGH);   // Turn on RED LED
  delay(1000);                       // 1 sec wait
  digitalWrite(RED_LED_PIN, LOW);    // Turn Off RED LED
  delay(1000);                       // 1 sec wait
  digitalWrite(GREEN_LED_PIN, HIGH); // Turn on GREEN LED
  delay(1000);                       // 1 sec wait
  digitalWrite(GREEN_LED_PIN, LOW);  // Turn Off GREEN LED
  delay(1000);                       // 1 sec wait
  digitalWrite(BLUE_LED_PIN, HIGH);  // Turn on BLUE LED
  delay(1000);                       // 1 sec wait
  digitalWrite(BLUE_LED_PIN, LOW);   //  Turn Off BLUE LED
  delay(10 00);                      // 1 sec wait
}