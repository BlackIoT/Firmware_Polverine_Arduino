/**
 * NOTE!
 * Remember to set USB CDC on Boot Enabled to use the Serial.
 * 
 */

#include "bme690Library.h"

#define NEW_GAS_MEAS (BME69X_GASM_VALID_MSK | BME69X_HEAT_STAB_MSK | BME69X_NEW_DATA_MSK)
#define MEAS_DUR 140


Bme69x bme;


void setup(void) {
	pinMode(RED_LED_PIN, OUTPUT);      // Set OUTPUT PIN
	digitalWrite(RED_LED_PIN, LOW);    // LED off
	pinMode(GREEN_LED_PIN, OUTPUT);    // Set OUTPUT PIN
	digitalWrite(GREEN_LED_PIN, LOW);  // LED off
	pinMode(BLUE_LED_PIN, OUTPUT);     // Set OUTPUT PIN
	digitalWrite(BLUE_LED_PIN, LOW);   // LED off

	Serial.begin(115200);

	while (!Serial)
		delay(10);

	bme.begin();

	if (bme.checkStatus()) {
		if (bme.checkStatus() == BME69X_ERROR) {
			Serial.println("Sensor error:" + bme.statusString());
			return;
		} else if (bme.checkStatus() == BME69X_WARNING) {
			Serial.println("Sensor Warning:" + bme.statusString());
		}
	}

	/* Set the default configuration for temperature, pressure and humidity */
	bme.setTPH();

	/* Heater temperature in degree Celsius */
	uint16_t tempProf[10] = { 320, 100, 100, 100, 200, 200, 200, 320, 320,
		                        320 };
	/* Multiplier to the shared heater duration */
	uint16_t mulProf[10] = { 5, 2, 10, 30, 5, 5, 5, 5, 5, 5 };
	/* Shared heating duration in milliseconds */
	uint16_t sharedHeatrDur = MEAS_DUR - (bme.getMeasDur(BME69X_PARALLEL_MODE) / 1000);

	bme.setHeaterProf(tempProf, mulProf, sharedHeatrDur, 10);
	bme.setOpMode(BME69X_PARALLEL_MODE);
	Serial.println("Sensor Init OK. Start reading data:");

	Serial.println("TimeStamp(ms), Temperature(deg C), Pressure(Pa), Humidity(%), Gas resistance(ohm), Status, Gas index");
}

void loop(void) {
	bme69xData data;
	uint8_t nFieldsLeft = 0;

	/* data being fetched for every 140ms */
	delay(MEAS_DUR);

	if (bme.fetchData()) {
		do {
			nFieldsLeft = bme.getData(data);
			if (data.status == NEW_GAS_MEAS) {
				Serial.print(String(millis()) + ", ");
				Serial.print(String(data.temperature) + ", ");
				Serial.print(String(data.pressure) + ", ");
				Serial.print(String(data.humidity) + ", ");
				Serial.print(String(data.gas_resistance) + ", ");
				Serial.print(String(data.status, HEX) + ", ");
				Serial.println(data.gas_index);
			}
		} while (nFieldsLeft);
	}
}
