/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef BLOKAS_COMMON_ADC_H
#define BLOKAS_COMMON_ADC_H

#include <avr/io.h>

#ifdef __cplusplus
extern "C" {
#endif

void adc_init(void);
uint16_t adc_read(uint8_t channel);

#include "adc_impl.h"

#ifdef __cplusplus
} // extern "C"
#endif

#endif // BLOKAS_COMMON_SPI_H
