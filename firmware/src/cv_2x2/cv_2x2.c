/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "spi.h"

enum PortAPins
{
	DAC_CS		= 1,
	DAC_SCK		= 2,
	DAC_SDI		= 3,
	DAC_LDAC	= 4,
};

enum PortBPins
{
	GATE_1 = 4,
	GATE_2 = 5,
};

enum CV
{
	CV1 = 0,
	CV2 = 1,
};

void cv_init()
{
	// Set the pins connected to DAC to output mode.
	DDRA = (1 << DAC_CS) | (1 << DAC_SCK) | (1 << DAC_SDI) | (1 << DAC_LDAC);
	PORTA = (1 << DAC_CS);

	// Set the pins Gate 1 and Gate 2 pins as output.
	DDRB = (1 << GATE_1) | (1 << GATE_2);
	PORTB &= ~((1 << GATE_1) | (1 << GATE_2));
}

#define SLEEP_US 1

void cv_send(enum CV cv, uint16_t value, bool gate)
{
	// Construct the data to be sent according to MCP4822 manual.
	uint16_t data = (cv == CV1 ? 0x8000 : 0x0000) | (1 << 12) | (value &0x0fff);

	PORTA &= ~(1 << DAC_CS);

	if (gate)
	{
		if (cv == CV1) PORTB |= 1 << GATE_1;
		else PORTB |= 1 << GATE_2;
	}
	else
	{
		if (cv == CV1) PORTB &= ~(1 << GATE_1);
		else PORTB &= ~(1 << GATE_2);
	}

	_delay_us(SLEEP_US);

	for (uint8_t i=0; i<16; ++i)
	{
		uint8_t portValue = PORTA;

		// Set the new bit value for the SDI pin.
		portValue &= ~(1 << DAC_SDI);
		portValue |= (data & 0x8000) ? 1 << DAC_SDI : 0;

		// Set the SCK pin to low.
		portValue &= ~(1 << DAC_SCK);

		// Bit bang.
		PORTA = portValue;
		_delay_us(SLEEP_US);

		// Set the SCK pin to high.
		PORTA |= 1 << DAC_SCK;
		_delay_us(SLEEP_US);

		data <<= 1;
	}

	PORTA |= (1 << DAC_CS);
	_delay_us(SLEEP_US);
}

int main(void)
{
	cv_init();
	spi_init();

	uint16_t channel1 = 0;
	uint16_t channel2 = 0;

	for (;;)
	{
		if (spi_slave_selected())
		{
			spi_enable();
			channel1 = spi_transfer16(0);
			channel2 = spi_transfer16(0);
			spi_disable();
		}

		cv_send(CV1, channel1 & 0x0fff, (channel1 & 0x8000) != 0);
		cv_send(CV2, channel2 & 0x0fff, (channel2 & 0x8000) != 0);
	}
}
