// FILE: DS18B20.cpp
// AUTHOR: lee@bussy.org
// VERSION: 0.1.1a
// DATE: 2020-03-15
//
// PUPROSE: library for DS18B20 temperature sensor with minimal footprint
//
// HISTORY:
// 0.1.0	2017-07-25 initial version
// 0.1.1 	2020-02-18 added getAddress()
// 0.1.1a   2020-03-15 added getTempF()

#include "Arduino.h"
#include "DS18B20.h"

DS18B20::DS18B20(OneWire *_oneWire)
{
    _wire = _oneWire;
    configured = false;
}

bool DS18B20::begin(void)
{
    _wire->reset_search();
    _wire->search(deviceAddress);
    configured = _wire->crc8(deviceAddress, 7) == deviceAddress[7] && deviceAddress[0] != 0x00;
    return configured;
}

void DS18B20::readScratchPad(uint8_t *scratchPad, uint8_t fields)
{
    _wire->reset();
    _wire->select(deviceAddress);
    _wire->write(READSCRATCH);

    for (uint8_t i = 0; i < fields; i++)
    {
        scratchPad[i] = _wire->read();
    }
    _wire->reset();
}

bool DS18B20::isConversionComplete(void)
{
    return (_wire->read_bit() == 1);
}

void DS18B20::requestTemperatures(void)
{
    _wire->reset();
    _wire->skip();
    _wire->write(STARTCONVO, 0);
}

float DS18B20::getTempC(void)
{
    ScratchPad scratchPad;
    readScratchPad(scratchPad, 2);

    int16_t rawTemperature = (((int16_t)scratchPad[TEMP_MSB]) << 8) | scratchPad[TEMP_LSB];

    if (rawTemperature == -1)
        return DEVICE_DISCONNECTED_C;

    float temp = 0.0625 * rawTemperature;
    return temp;
}

float DS18B20::getTempF(void)
{
    float retVal = getTempC();

    if (retVal == (float)DEVICE_DISCONNECTED_C)
        return DEVICE_DISCONNECTED_F;
    else
        return retVal * 1.8 + 32;
}

void DS18B20::setResolution(uint8_t newResolution)
{
    _wire->reset();
    _wire->select(deviceAddress);
    _wire->write(WRITESCRATCH);
    // two dummy values for LOW & HIGH ALARM
    _wire->write(0);
    _wire->write(100);
    switch (newResolution)
    {
    case 12:
        _wire->write(TEMP_12_BIT);
        break;
    case 11:
        _wire->write(TEMP_11_BIT);
        break;
    case 10:
        _wire->write(TEMP_10_BIT);
        break;
    case 9:
    default:
        _wire->write(TEMP_9_BIT);
        break;
    }
    _wire->reset();
}

bool DS18B20::getAddress(uint8_t *buf)
{
    if (configured)
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            buf[i] = deviceAddress[i];
        }
    }
    return configured;
}

//  END OF FILE
