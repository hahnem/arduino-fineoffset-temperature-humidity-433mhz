# arduino-fineoffset-temperature-humidity-433mhz
Library for sending temperature and humidity data from an arduino, to a tellstick duo using Fineoffset 433Mhz protocol, using a TX433N.

## Example usage


```
#include "foth433.h"

// Device id to use for Foth433
const uint8_t deviceId = 0;

// Number of time to send data
const int repeats = 3;

// Connected 433Mhz transmitter on pin7
const int txPin = 7;

uint8_t hum = 0;
float temp = -10.0f;

Foth433 *fo;

void setup()
{
  fo = new Foth433(txPin, deviceId, repeats);
}

void loop()
{
  // TODO, read some values from real sensors..
  fo->setHumidity(hum);
  fo->setTemperature(temp);

  // Send data over 433 Mhz, using fine offset protocol.
  fo->sendData();

  // Increase values, to simulate changed sensor readings..
  hum += 1;
  temp += 0.5;

  // Sleep for a while.
  delay(2000); 
}

```
