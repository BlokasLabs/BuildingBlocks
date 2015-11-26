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
#include "adc.h"

enum
{
	SWITCH_COUNT = 4,
	ENCODER_COUNT = 4,
};

static uint8_t g_downMask = 0;
static int16_t g_encoderValues[ENCODER_COUNT] = { 0, 0, 0, 0 };

enum
{
	SW_1_PIN = 4,
	SW_3_PIN = 5,
	SW_2_4_ADC_CH = 9,
};

void switches_update()
{
	uint8_t value = PINB;

	uint8_t newMask = 0;

	// The switch is closed if 0 is read.
	newMask |= ((value & (1 << SW_1_PIN)) == 0) << 3; // The rightmost << is meant for mapping the value to the correct control on the board.
	newMask |= ((value & (1 << SW_3_PIN)) == 0) << 1;

	uint16_t adcValue = adc_read(SW_2_4_ADC_CH);

	bool down2 = false;
	bool down4 = false;

	// These 2 switches are connected to a resistor divider network,
	// using a single pin, so it is read as analog input, and its state
	// is determined by checking which value range the read analog value
	// falls to.
	if (adcValue >= 750)
	{
		down2 = true;
		down4 = true;
	}
	else if (adcValue >= 600)
	{
		down2 = true;
	}
	else if (adcValue >= 400)
	{
		down4 = true;
	}

	newMask |= down2 ? 1 << 2 : 0;
	newMask |= down4 ? 1 : 0;

	g_downMask = newMask;
}

static const uint8_t g_encoderMap[ENCODER_COUNT] =
{
	3, 1, 2, 0
};

static const int8_t g_encoderPolarity[ENCODER_COUNT] =
{
	1, 1, 1, -1
};

void encoders_update(void)
{
	// All the possible encoder states.
	static const int8_t enc_states[16] =
	{
		 0, // 0000
		-1, // 0001
		 1, // 0010
		 0, // 0011
		 1, // 0100
		 0, // 0101
		 0, // 0110
		-1, // 0111
		-1, // 1000
		 0, // 1001
		 0, // 1010
		 1, // 1011
		 0, // 1100
		 1, // 1101
		-1, // 1110
		 0  // 1111
	};

	static int8_t s_encVal[ENCODER_COUNT];
	static uint16_t s_encPrevState = 0;

	s_encPrevState <<= 2;
	s_encPrevState &= 0xcccc;

	uint8_t value = PINA;

	for (uint8_t j=0; j<ENCODER_COUNT; ++j)
	{
		uint8_t i = g_encoderMap[j];
		s_encPrevState |= (value & 0x03) << (i*4);
		value >>= 2;
	}

	uint16_t state = s_encPrevState;
	for (uint8_t j=0; j<ENCODER_COUNT; ++j)
	{
		uint8_t i = g_encoderMap[j];
		s_encVal[i] += enc_states[state & 0x0f];
		state >>= 4;
	}

	for (uint8_t j=0; j<ENCODER_COUNT; ++j)
	{
		uint8_t i = g_encoderMap[j];
		if (s_encVal[i] > 3)
		{
			g_encoderValues[j] += g_encoderPolarity[i];
			s_encVal[i] = 0;
		}
		else if (s_encVal[i] < -3)
		{
			g_encoderValues[j] -= g_encoderPolarity[i];
			s_encVal[i] = 0;
		}
	}
}

void switches_init(void)
{
	DDRB &= ~((1 << SW_1_PIN) | (1 << SW_3_PIN)); // Configure the digital switch pins as input.
	PORTB = (1 << SW_1_PIN) | (1 << SW_3_PIN); // Enable the internal pull-up resistors.
}

void encoders_init(void)
{
	DDRA = 0x00; // Set all PORTA pins as input.
	PORTA = 0xff; // Enable the internal pull-up resistors.
}

int main(void)
{
	adc_init();

	spi_init();
	spi_disable();

	switches_init();
	encoders_init();

	for (;;)
	{
		// Check the SS pin, if it is low, send the data through SPI.
		if (!(PINB & 0x08))
		{
			spi_enable();
			for (uint8_t i=0; i<ENCODER_COUNT; ++i)
			{
				spi_transfer16(g_encoderValues[i]);
			}
			spi_transfer8(g_downMask);
			spi_disable();
		}

		switches_update();
		encoders_update();
	}
}
