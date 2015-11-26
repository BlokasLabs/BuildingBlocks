/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

// Common code for ATtiny 261A, 461A, 861A and other variations.

inline void adc_init(void)
{
	// Set the prescaler to clock/128 & enable ADC
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
}

inline uint16_t adc_read(uint8_t channel)
{
	ADMUX = (ADMUX & ~0x1f) | (channel & 0x1f);

	// Start the conversion
	ADCSRA |= (1 << ADSC);

	// Wait for it to finish
	while (ADCSRA & (1 << ADSC));

	uint16_t result = ADC;

	return result;
}
