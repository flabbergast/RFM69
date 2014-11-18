#include "USISPI.h"
#include <stdint.h>
#include <avr/pgmspace.h>

SPIclass SPI;

void SPIclass::begin(void) {
  USI_DDR &= ~_BV(DI_BIT); // DI / input / PA6/ Dig4 (also called MOSI on the tiny)
  USI_PORT |= _BV(DI_BIT); // enable pull-up
  USI_DDR |= _BV(DO_BIT); // DO / output / PA5 / Dig5 (also called MISO on the tiny)
  USI_DDR |= _BV(USCK_BIT); // USCK / output / PA4 / Dig6
  // Note: CS/Select is done elsewhere
  USICR = _BV(USIWM0);
}

// SPI speed is given just by how fast this code runs
uint8_t SPIclass::transfer(uint8_t out) {
  USIDR = out;
  uint8_t v1 = _BV(USIWM0) | _BV(USITC);
  uint8_t v2 = _BV(USIWM0) | _BV(USITC) | _BV(USICLK);
#if F_CPU <= 5000000
  // only unroll if resulting clock stays under 2.5 MHz
  USICR = v1; USICR = v2;
  USICR = v1; USICR = v2;
  USICR = v1; USICR = v2;
  USICR = v1; USICR = v2;
  USICR = v1; USICR = v2;
  USICR = v1; USICR = v2;
  USICR = v1; USICR = v2;
  USICR = v1; USICR = v2;
#else
  for (uint8_t i = 0; i < 8; ++i) {
      USICR = v1;
      USICR = v2;
  }
#endif
  return USIDR;
}

