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

void spi_init(void)
{
	// Initialize to three wire mode, positive edge shift register clock.
	USICR = (1 << USIWM0) + (1 << USICS1);
}

uint8_t spi_transfer(uint8_t val)
{
	// Set the value to send.
	USIDR = val;

	// Clear the counter overflow flag.
	USISR = 1 << USIOIF;

	// Block until transfer finishes.
	while (!(USISR & (1 << USIOIF)));

	// Return received byte.
	return USIDR;
}

void spi_enable(void)
{
	// Enable SPI output.
	DDRB	= 0x02;
	PORTB	= 0x08;
}

void spi_disable(void)
{
	// Disable SPI output, set to high impedance in order to not affect the shared SPI bus.
	DDRB	= 0x00;
	PORTB	= 0x00;
}

uint16_t translate(uint16_t x)
{
	// Bit-shift map for the keys, results in the natural numbering:
	// bit:  0  1  2  3  4  5  6  7  8  9 10 11 12 13
	// note: C  C# D  D# E  F  F# G  G# A A# B  C  SHIFT
	static const uint16_t s_keyShiftMap[14] =
	{
		5, 3, 12, 8, 9, 1, 13, 10, 7, 0, 11, 6, 4, 2
	};

	uint16_t result = 0;

	// Ignore the first 2 bits, the switches corresponding to them are not in the matrix.
	x >>= 2;

	for (uint16_t i=0; i<14; ++i)
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
	spi_disable();

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
			spi_transfer(translatedButtonState >> 8);
			spi_transfer(translatedButtonState & 0xff);
			spi_disable();
		}

		// Set one of the column pin low, to allow current to flow.
		PORTA = 0xff ^ (1 << (column + 4));

		// Give some time for voltage change on the pin network.
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
