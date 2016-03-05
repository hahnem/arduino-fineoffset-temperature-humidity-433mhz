/**
 * FOTH433.h - FineOffset temperature and humidity transmitter.
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
#ifndef FOTH433_h
#define FOTH433_h

#include <Arduino.h>

class Foth433
{
  public:
    Foth433(int txPin, uint8_t deviceId,  int repeats);
    void setDeviceId(uint8_t deviceId);
    void setHumidity(uint8_t humidity);
    void setTemperature(float temperature);
    void sendData();

  private:
  	// pulse length in micro seconds
  	const int PULSE_LENGTH = 500;

    int _txPin;
    int _repeats;
    
    uint16_t _temperature;
    uint8_t _humidity;
    uint8_t _deviceId;

    void send0(void);
    void send1(void);
    void sendByte(uint8_t data);

    void sendStart(void);
    void sendStop(void);
    void sendDeviceCode(void);
    void sendTemperature(void);
    void sendHumidity(void);
    void sendChecksum(void);
};

#endif
