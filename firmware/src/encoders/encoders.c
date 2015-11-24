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

enum
{
	PIN_A = 4,
	PIN_B = 5,
	PIN_C = 6,
};

enum
{
	PIN_CONFIG = 0,
	PIN_STATE = 1,
};

enum
{
	SWITCH_COUNT = 4,
	ENCODER_COUNT = 4,
};

enum
{
	INPUT = 0,
	OUTPUT = 1,
};

enum
{
	LOW = 0,
	HIGH = 1,
};

static int16_t g_encoderValues[ENCODER_COUNT] = { 0, 0, 0, 0 };
static uint8_t g_downMask = 0;

static const uint8_t g_matrix[SWITCH_COUNT][2][3] = {
	//        PIN_CONFIG              PIN_STATE
	//    A       B       C         A     B    C
	{ { INPUT, INPUT, OUTPUT }, { HIGH, LOW, LOW } }, // 0001
	{ { INPUT, OUTPUT, INPUT }, { LOW, LOW, HIGH } }, // 0010
	{ { INPUT, INPUT, OUTPUT }, { LOW, HIGH, LOW } }, // 0100
	{ { OUTPUT, INPUT, INPUT }, { LOW, LOW, HIGH } }, // 1000
};

static const uint8_t g_pinMap[SWITCH_COUNT] =
{
	PIN_A, PIN_C, PIN_B, PIN_C
};

static void pinMode(int pin, bool value)
{
	if (value) DDRB |= 1 << pin;
	else DDRB &= ~(1 << pin);
}

static void digitalWrite(int pin, bool value)
{
	if (value) PORTB |= 1 << pin;
	else PORTB &= ~(1 << pin);
}

static bool digitalRead(int pin)
{
	return (PINB & (1 << pin)) != 0;
}

static bool switch_read(uint8_t sw)
{
	if (sw >= SWITCH_COUNT)
		return false;

	pinMode(PIN_A, g_matrix[sw][PIN_CONFIG][0]);
	pinMode(PIN_B, g_matrix[sw][PIN_CONFIG][1]);
	pinMode(PIN_C, g_matrix[sw][PIN_CONFIG][2]);
	digitalWrite(PIN_A, g_matrix[sw][PIN_STATE][0]);
	digitalWrite(PIN_B, g_matrix[sw][PIN_STATE][1]);
	digitalWrite(PIN_C, g_matrix[sw][PIN_STATE][2]);

	_delay_us(1);

	return digitalRead(g_pinMap[sw]);
}

static const uint8_t g_encoderMap[ENCODER_COUNT] =
{
	3, 1, 2, 0
};

static const int8_t g_encoderPolarity[ENCODER_COUNT] =
{
	1, 1, 1, -1
};

void encoder_update(void)
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
		s_encPrevState |= (value & 0x03) << (i*2);
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
			g_encoderValues[i] += -g_encoderPolarity[i];
			s_encVal[i] = 0;
		}
		else if (s_encVal[i] < -3)
		{
			g_encoderValues[i] += g_encoderPolarity[i];
			s_encVal[i] = 0;
		}
	}
}

void encoders_init(void)
{
	DDRA = 0x00;
	PORTA = 0xff;
}

int main(void)
{
	spi_init();
	spi_disable();

	encoders_init();

	uint8_t enc = 0;
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

		g_downMask &= ~(1 << enc);
		bool val = !switch_read(enc);
		g_downMask |= val << enc;

		//g_encoderValues[enc] += encoder_update(enc);
		encoder_update();

		enc = (enc+1) % ENCODER_COUNT;
	}
}
