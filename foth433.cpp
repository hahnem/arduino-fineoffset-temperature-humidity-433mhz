/**
 * FOTH433.cpp - FineOffset temperature and humidity transmitter.
 *
 * Library for sending temperature and humidity data over 433Mhz, using the Fineoffset protocol.
 * Note: Checksum is not implemented, but still works for tellstick duo.
 *
 * Protocol description from:
 * http://lucsmall.com/2012/04/27/weather-station-hacking-part-1/
 *
 * Created by Martin Hahne, 2016
 * MIT Licensed, see LICENSE.md
*/
#include "foth433.h"

/**
* Create new instance of 
* @param txPin Pin where 433 Transmitter is connected.
* @param deviceId Value of 0 to 255
* @param repeats Number of time to send data. 3 is a good value.
*/
Foth433::Foth433(int txPin, uint8_t deviceId, int repeats)
{
	pinMode(txPin, OUTPUT);
	_txPin = txPin;
	_humidity = 0;
	_deviceId = deviceId;
	_temperature = 0;
	_repeats = repeats;
}

/**
* Set device id, 0-255
* @param deviceId Value of 0 to 255
*/
void Foth433::setDeviceId(uint8_t deviceId)
{
	_deviceId = deviceId;
}

/**
* Set humidity percentage.
* @param humidity A value from 0 to 100
*/
void Foth433::setHumidity(uint8_t humidity)
{
	// Cap value to 100
	if (humidity > 100)
		humidity = 100;

	_humidity = humidity;
}

/**
* Set temperature.
* @param temperature Value between -204.7 to 204.7
*/
void Foth433::setTemperature(float temperature)
{
	_temperature = 0;

	// If temperature is negative, set MSB "sign bit" to 1
	if (temperature < 0) {
		_temperature = 0x8000; 
		temperature *= -1;
	}

	/*
	 * Create 12 bit temperature data.
	 * 1. intValue = floatValue * 10.0f
	 * 2. Cap to 0x7FF (keep 11 bits)
	 * 3. Shift lower 11 bits temperature data
	 * 4. OR with "sign bit" in MSB
	 */
	_temperature |= (((uint16_t)(temperature * 10.0f)) & 0x7FF) << 4;
}

/**
* Send data on TX pin
*/
void Foth433::sendData(void)
{
	for (int i=0; i<_repeats; i++)
	{
		sendStart();
		sendDeviceCode();
		sendTemperature();
		sendHumidity();
		sendChecksum();
		sendStop();
	}
}

/**
* Send 1
*/
void Foth433::send1(void)
{
	digitalWrite(_txPin, HIGH);
	delayMicroseconds(PULSE_LENGTH);
	digitalWrite(_txPin, LOW);
	delayMicroseconds(PULSE_LENGTH * 2);
}

/**
* Send 0
*/
void Foth433::send0(void)
{
	digitalWrite(_txPin, HIGH);
	delayMicroseconds(PULSE_LENGTH * 3);
	digitalWrite(_txPin, LOW);
	delayMicroseconds(PULSE_LENGTH * 2);
}

/**
* Send START data seq.
* Eight ONES
*/
void Foth433::sendStart(void)
{
	for (int i=0; i<8; i++) {
		send1();
	}
}

/**
* End of data, not sure its needed.
* Turn off tx and sleep for 9 PULSE_LENGTH
*/
void Foth433::sendStop(void)
{
	digitalWrite(_txPin, LOW);
	delayMicroseconds(PULSE_LENGTH * 9);
}

/**
* Send byte of data.
*/
void Foth433::sendByte(uint8_t data)
{
  for (int i=0; i<8; i++)
  {
    if (data & 0x80) {
      send1();
    }
    else {
      send0();
    }
    data <<= 1;
  }
}

/**
* Send device code, 12 bits.
* AAAA - 4 bits unknown function.
* BBBB BBBB - 8 bits device id, 0 to 255
*/
void Foth433::sendDeviceCode(void) {
	send0();
	send0();
	send0();
	send0();

	sendByte(_deviceId);
	return;
}
/**
* Temperature is sent using 12 bits
* A = 1 for negative, 0 for positive
* BBBBBBBBBBB 11 bit int value, 0 to 2047 (meaning 0 to 204.7)
*/
void Foth433::sendTemperature(void) {
	uint16_t data = _temperature;
	for (int i=0; i<12; i++)
	{
		if (data & 0x8000) {
			send1();
		}
		else {
			send0();
		}
		data <<= 1;
	}
}

/**
 * Send humidity byte
 */
void Foth433::sendHumidity(void) {
	sendByte(_humidity);
}

/**
 * Send checksum byte
 * NOTE: Checksum is not implemented, and will only send 0xFF
 */
void Foth433::sendChecksum(void) {
	sendByte(0xFF);
}
