/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"

uint16_t translate(uint16_t x)
{
	// Bit-shift map for the keys, results in the natural numbering:
	// bit:  0  1  2  3  4  5  6  7  8  9 10 11 12 13       14
	// note: C  C# D  D# E  F  F# G  G# A A# B  C  OCT_DOWN OCT_UP
	static const uint8_t s_keyShiftMap[15] =
	{
		14, 5, 3, 12, 8, 9, 1, 13, 10, 7, 0, 11, 6, 4, 2
	};

	uint16_t result = 0;

	// Ignore the first bit, the switch corresponding to it is not in the matrix.
	x >>= 1;

	for (uint8_t i=0; i<15; ++i)
	{
		// If the bit is LOW (0), the switch is down.
		if ((x & 1) == 0)
			result |= 1 << s_keyShiftMap[i];

		x >>= 1;
	}

	return result;
}

int main(void)
{
	// Variable for keeping the raw state of the matrix.
	uint16_t rawButtonState = 0xffff;

	// Variable for keeping the translated / mapped state of the matrix, to send over SPI.
	uint16_t translatedButtonState = 0;

	spi_init();

	// Configure the pins used for scanning the matrix.
	DDRA = 0xf0;
	PORTA = 0xff;

	uint8_t column = 0;
	for (;;)
	{
		// Check the SS pin, if it is low, send the data through SPI.
		if (!(PINB & 0x08))
		{
			spi_enable();
			spi_transfer16(translatedButtonState);
			spi_disable();
		}

		// Set one of the column pin low, to allow current to flow.
		PORTA = 0xff ^ (1 << (column + 4));

		// Give some time for voltage to change on the pin network.
		_delay_us(10);

		// Read the line of rows.
		uint16_t row = PINA & 0x0f;

		// Clear the stored values for this row.
		rawButtonState &= ~(0x000f << (column*4));

		// Store the new values for this row.
		rawButtonState |= row << (column*4);
		translatedButtonState = translate(rawButtonState);

		// Set the next column.
		column = (column+1) & 3;
	}
}
