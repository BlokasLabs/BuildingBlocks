/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

// Common code for ATtiny 261A, 461A, 861A and other variations.

inline void spi_init(void)
{
	// Initialize to three wire mode, positive edge shift register clock.
	USICR = (1 << USIWM0) + (1 << USICS1);
	spi_disable();
}

inline void spi_enable(void)
{
	// Enable SPI output.
	DDRB	|= 0x02;
	PORTB	|= 0x08;
}

inline void spi_disable(void)
{
	// Disable SPI output, set to high impedance in order to not affect the shared SPI bus.
	DDRB	&= ~0x02;
	PORTB	&= ~0x08;
}

inline bool spi_slave_selected(void)
{
	static bool was_selected = false;

	if (was_selected)
	{
		if (PINB & 0x08)
		{
			was_selected = false;
		}
		return false;
	}
	else
	{
		if (!(PINB & 0x08))
		{
			was_selected = true;
			return true;
		}
		return false;
	}
}

inline uint8_t spi_transfer8(uint8_t val)
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

inline uint16_t spi_transfer16(uint16_t val)
{
	uint16_t result = spi_transfer8(val>>8) << 8;
	result |= spi_transfer8(val);
	return result;
}
