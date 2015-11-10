/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "Keypad.h"

Keypad::Keypad()
	:m_keyStates(0)
	,m_callback(NULL)
{
}

void Keypad::init()
{
	setSpiSettings(SPISettings(250000, MSBFIRST, SPI_MODE1));
}

void Keypad::update()
{
	digitalWrite(getSlaveSelectPin(), 0);
	delay(1);
	SPI.beginTransaction(getSpiSettings());

	uint16_t oldStates = m_keyStates;
	m_keyStates = SPI.transfer16(0);

	digitalWrite(getSlaveSelectPin(), 1);
	SPI.endTransaction();

	uint8_t i=0;
	uint16_t changes = m_keyStates ^ oldStates;
	while (m_callback && changes)
	{
		if (changes & 1)
		{
			(*m_callback)(i, isKeyDown(i));
		}

		changes >>= 1;
		++i;
	}
}

uint16_t Keypad::getKeyStates() const
{
	return m_keyStates;
}

bool Keypad::isKeyDown(uint8_t key) const
{
	return getKeyStates() & (1 << key);
}

void Keypad::setCallback(void (*fptr)(uint8_t key, bool isDown))
{
	m_callback = fptr;
}
