// FILE: DS18B20.h
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

//
//  BOTTOM VIEW
//
//   /---+
//  /  o | GND
//  |  o | DATA
//  \  o | VCC
//   \---+
//

#ifndef DS18B20_H
#define DS18B20_H

#define DS18B20_LIB_VERSION "0.1.1a"

#include <OneWire.h>

// OneWire commands
#define STARTCONVO 0x44
#define READSCRATCH 0xBE
#define WRITESCRATCH 0x4E

// Scratchpad locations
#define TEMP_LSB 0
#define TEMP_MSB 1
// #define HIGH_ALARM_TEMP 2
// #define LOW_ALARM_TEMP  3
// #define CONFIGURATION   4
// #define INTERNAL_BYTE   5
// #define COUNT_REMAIN    6
// #define COUNT_PER_C     7
// #define SCRATCHPAD_CRC  8

// Device resolution
#define TEMP_9_BIT 0x1F  //  9 bit
#define TEMP_10_BIT 0x3F // 10 bit
#define TEMP_11_BIT 0x5F // 11 bit
#define TEMP_12_BIT 0x7F // 12 bit

// Error Code
#define DEVICE_DISCONNECTED -127
#define DEVICE_DISCONNECTED_C -127
#define DEVICE_DISCONNECTED_F -196.6

typedef uint8_t DeviceAddress[8];
typedef uint8_t ScratchPad[9];

class DS18B20
{
public:
    explicit DS18B20(OneWire *);
    bool begin(void);
    void setResolution(uint8_t);
    void requestTemperatures(void);
    float getTempC(void);
    float getTempF(void);
    bool isConversionComplete(void);
    bool getAddress(uint8_t *);

private:
    void readScratchPad(uint8_t *, uint8_t);
    DeviceAddress deviceAddress;
    OneWire *_wire;
    bool configured;
};

#endif // DS18B20_H

//  END OF FILE
