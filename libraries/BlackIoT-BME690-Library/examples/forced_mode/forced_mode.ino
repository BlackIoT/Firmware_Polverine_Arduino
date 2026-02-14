/**
 * NOTE!
 * Remember to set USB CDC on Boot Enabled to use the Serial.
 * 
 */
 
#include "bme690Library.h"


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

	/* Set the heater configuration to 300 deg C for 100ms for Forced mode */
	bme.setHeaterProf(300, 100);
	Serial.println("Sensor Init OK. Start reading data:");

	Serial.println("TimeStamp(ms), Temperature(deg C), Pressure(Pa), Humidity(%), Gas resistance(ohm), Status");
}

void loop(void) {
	bme69xData data;

	bme.setOpMode(BME69X_FORCED_MODE);
	delayMicroseconds(bme.getMeasDur());

	if (bme.fetchData()) {
		bme.getData(data);
		Serial.print(String(millis()) + ", ");
		Serial.print(String(data.temperature) + ", ");
		Serial.print(String(data.pressure) + ", ");
		Serial.print(String(data.humidity) + ", ");
		Serial.print(String(data.gas_resistance) + ", ");
		Serial.println(data.status, HEX);
	}
}
