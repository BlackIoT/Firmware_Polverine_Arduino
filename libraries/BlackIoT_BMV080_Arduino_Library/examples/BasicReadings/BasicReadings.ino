/**
 * NOTE!
 * Remember to set USB CDC on Boot Enabled to use the Serial.
 * 
 */
 
 #include <BlackIoT_BMV080_Arduino_Library.h>

// The BMV080 Bosch API requires a larger than usual stack size
// In particular, bmv080_serve_interrupt is the culprit.
// If we are an ESP32 architecture, then we need to increase the loop stack size
// to 60KB. This is because the ESP32 has a 32KB stack size by default.
SET_LOOP_TASK_STACK_SIZE(60 * 1024);  // 60KB

BMV080SPI bmv080; // Create an instance of the BMV080 class

/* Entry point for the example */
void setup(void){
    pinMode(GREEN_LED_PIN, OUTPUT);    // Set OUTPUT PIN
    digitalWrite(GREEN_LED_PIN, LOW);  // LED off

    /* Initialize the communication interfaces */
    Serial.begin(115200);

    while (!Serial)
		delay(10);
    
    Serial.print("MOSI: ");
    Serial.println(BMV080_MOSI);
    Serial.print("MISO: ");
    Serial.println(BMV080_MISO);
    Serial.print("SCK: ");
    Serial.println(BMV080_SCK);
    Serial.print("SS: ");
    Serial.println(BMV080_CS); 
    pinMode(BMV080_CS, OUTPUT);
    digitalWrite(BMV080_CS, HIGH);
   
    Serial.println();
    Serial.println("BMV080 Example");

    if (bmv080.begin() == false) {
        Serial.println("SPI init failure. Freezing...");
        while (1)
        ;
    }
    Serial.println("BMV080 SPI init successful");


    /* Initialize the Sensor (read driver, open, reset, id etc.)*/
    bmv080.init();

    /* Set the sensor Duty Cycling Period (seconds)*/
    uint16_t duty_cycling_period = 0;

    if(duty_cycling_period == 0){
        /* Set the sensor mode to continuous mode*/
        if(bmv080.setMode(0) == true){
            Serial.println("BMV080 set to continuous mode");
        }
        else{
            Serial.println("Error setting BMV080 mode");
        }

    }
    else{
        if(bmv080.setDutyCyclingPeriod(duty_cycling_period) == true){
            Serial.println("BMV080 set to 20 second duty cycle period");
        }
        else{
            Serial.println("Error setting BMV080 duty cycle period");
        }

        /* Set the sensor mode to Duty Cycle mode */
        if(bmv080.setMode(BMV080_MODE_DUTY_CYCLE) == true){
            Serial.println("BMV080 set to Duty Cycle mode");
        }
        else{
            Serial.println("Error setting BMV080 mode");
        }
    }
}


/* Function that is looped forever */
void loop(void){
    /* Call the run function often so that the library can 
     * check if it is time to read new data from the sensor  
     * and process it.
     */

    if(bmv080.readSensor()){   
        float pm25 = bmv080.PM25();
        float pm1 = bmv080.PM1();
        float pm10 = bmv080.PM10();
        
        Serial.print("myPM1 is: ");
        Serial.print(pm1);
        Serial.print("  myPM25 is: ");
        Serial.print(pm25);
        Serial.print("  myPM10 is: ");
        Serial.print(pm10);
        if(bmv080.isObstructed() == true){
            Serial.print("\tObstructed");
        }
        Serial.println();
        digitalWrite(GREEN_LED_PIN, HIGH);
        delay(100); 
        digitalWrite(GREEN_LED_PIN, LOW);
    }
}
