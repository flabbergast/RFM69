// based on jeelib

#if !defined(__AVR_ATtiny84__)
  #error "This SPI (using USI module) is only for attinys! (ATM t84)."
#endif

#ifndef USISPI_h
#define USISPI_h

#include <stdint.h>

#if defined(__AVR_ATtiny84__) // attiny84 pin assignment
  #define USI_DDR               DDRA
  #define USI_PORT              PORTA
  #define DI_BIT                6
  #define DO_BIT                5
  #define USCK_BIT              4
#else
  #error "Define USI pins for your tiny!"
#endif

class SPIclass {
  public:
    static void begin(void);
    static uint8_t transfer(uint8_t out);
};

extern SPIclass SPI;

#endif
