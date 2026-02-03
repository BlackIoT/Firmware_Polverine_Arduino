#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>
#include "soc/soc_caps.h"

#define USB_VID 0x303a
#define USB_PID 0x1001

static const uint8_t LED_BUILTIN = 47;
#define BUILTIN_LED LED_BUILTIN  // backward compatibility
#define LED_BUILTIN LED_BUILTIN  // allow testing #ifdef LED_BUILTIN

static const uint8_t BUT_BUILTIN = 0;
#define BUILTIN_BUT BUT_BUILTIN  // backward compatibility
#define BUT_BUILTIN BUT_BUILTIN  // allow testing #ifdef BUT_BUILTIN

// MikrcoBUS PIN definition
static const uint8_t TX = 17;
static const uint8_t RX = 18;

static const uint8_t SDA = 8;
static const uint8_t SCL = 9;

static const uint8_t SS = 34;
static const uint8_t MOSI = 35;
static const uint8_t MISO = 37;
static const uint8_t SCK = 36;

static const uint8_t PWR = 2;

static const uint8_t INT = 7;

static const uint8_t AN = 1;

// J3 PIN
static const uint8_t TMS = 42;
static const uint8_t TDI = 41;
static const uint8_t TDO = 40;
static const uint8_t TCK = 39;

// BMV080
static const uint8_t BMV_CS = 10;
static const uint8_t BMV_MOSI = 11;
static const uint8_t BMV_MISO = 13;
static const uint8_t BMV_SCK = 12;
static const uint8_t BMV_INT = 5;

//BME690
static const uint8_t BME690_SDA = 14;
static const uint8_t BME690_SCL = 21;

//RGB
static const uint8_t RED_LED_PIN = 47;
static const uint8_t GREEN_LED_PIN = 48;
static const uint8_t BLUE_LED_PIN = 38;


#endif /* Pins_Arduino_h */
