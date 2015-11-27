/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "Pots.h"

Pots::Pots()
	:m_callback(NULL)
	,m_userdata(NULL)
{
	for (uint8_t i=0; i<MAX_POTS; ++i)
	{
		m_potValues[i] = 0;
	}
}

void Pots::init()
{
	setSpiSettings(SPISettings(125000, MSBFIRST, SPI_MODE1));
}

uint16_t Pots::getValue(uint8_t pot) const
{
	if (pot < MAX_POTS)
		return m_potValues[pot];

	return 0;
}

void Pots::setCallback(void (*fptr)(void *userdata, uint8_t pot, uint16_t value), void *userdata)
{
	m_callback = fptr;
	m_userdata = userdata;
}

void Pots::update()
{
	uint16_t oldValues[MAX_POTS];
	memcpy(oldValues, m_potValues, sizeof(m_potValues));

	digitalWrite(getSlaveSelectPin(), 0);
	delay(1);
	SPI.beginTransaction(getSpiSettings());

	for (uint8_t i=0; i<MAX_POTS; ++i)
	{
		m_potValues[i] = SPI.transfer16(0);
		delayMicroseconds(50);
	}

	SPI.endTransaction();
	digitalWrite(getSlaveSelectPin(), 1);

	for (uint8_t i=0; m_callback && i<MAX_POTS; ++i)
	{
		if (m_potValues[i] != oldValues[i])
		{
			(*m_callback)(m_userdata, i, m_potValues[i]);
		}
	}
}
