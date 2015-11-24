/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef BLOKAS_COMMON_SPI_H
#define BLOKAS_COMMON_SPI_H

#include <avr/io.h>

#ifdef __cplusplus
extern "C" {
#endif

void spi_init(void);
void spi_enable(void);
void spi_disable(void);

uint8_t spi_transfer8(uint8_t val);
uint16_t spi_transfer16(uint16_t val);

#include "spi_impl.h"

#ifdef __cplusplus
} // extern "C"
#endif

#endif // BLOKAS_COMMON_SPI_H
