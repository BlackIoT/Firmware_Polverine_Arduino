/**
 * NOTE!
 * Remember to set USB CDC on Boot Enabled to use the Serial.
 * 
 */

#include "bme690Library.h"

#define NEW_GAS_MEAS (BME69X_GASM_VALID_MSK | BME69X_HEAT_STAB_MSK | BME69X_NEW_DATA_MSK)


Bme69x bme;


void setup(void) {

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
	uint16_t tempProf[10] = { 100, 200, 320 };
	/* Heating duration in milliseconds */
	uint16_t durProf[10] = { 150, 150, 150 };

	bme.setSeqSleep(BME69X_ODR_250_MS);
	bme.setHeaterProf(tempProf, durProf, 3);
	bme.setOpMode(BME69X_SEQUENTIAL_MODE);
	Serial.println("Sensor Init OK. Start reading data:");

	Serial.println("TimeStamp(ms), Temperature(deg C), Pressure(Pa), Humidity(%), Gas resistance(ohm), Status, Gas index");
}

void loop(void) {
	bme69xData data;
	uint8_t nFieldsLeft = 0;

	delay(150);

	if (bme.fetchData()) {
		do {
			nFieldsLeft = bme.getData(data);
			//if (data.status == NEW_GAS_MEAS)
			{
				Serial.print(String(millis()) + ", ");
				Serial.print(String(data.temperature) + ", ");
				Serial.print(String(data.pressure) + ", ");
				Serial.print(String(data.humidity) + ", ");
				Serial.print(String(data.gas_resistance) + ", ");
				Serial.print(String(data.status, HEX) + ", ");
				Serial.println(data.gas_index);
				if (data.gas_index == 2) /* Sequential mode sleeps after this measurement */
					delay(250);
			}
		} while (nFieldsLeft);
	}
}
