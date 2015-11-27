/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "Encoders.h"

Encoders::Encoders()
	:m_callback(NULL)
	,m_userdata(NULL)
	,m_encDownMask(0)
{
	for (uint8_t i=0; i<MAX_ENCODERS; ++i)
	{
		m_encValues[i] = 0;
	}
}

void Encoders::init()
{
	setSpiSettings(SPISettings(125000, MSBFIRST, SPI_MODE1));
}

int16_t Encoders::getValue(uint8_t enc) const
{
	if (enc < MAX_ENCODERS)
		return m_encValues[enc];

	return 0;
}

void Encoders::setCallback(void (*fptr)(void *userdata, uint8_t enc, int8_t changeDirection, bool isDown), void *userdata)
{
	m_callback = fptr;
	m_userdata = userdata;
}

void Encoders::update()
{
	int16_t oldValues[MAX_ENCODERS];
	memcpy(oldValues, m_encValues, sizeof(m_encValues));

	uint8_t oldDownMask = m_encDownMask;

	digitalWrite(getSlaveSelectPin(), 0);
	delay(1);
	SPI.beginTransaction(getSpiSettings());

	for (uint8_t i=0; i<MAX_ENCODERS; ++i)
	{
		m_encValues[i] = SPI.transfer16(0);
		delayMicroseconds(50);
	}

	m_encDownMask = SPI.transfer(0);

	SPI.endTransaction();
	digitalWrite(getSlaveSelectPin(), 1);

	uint8_t downChanges = m_encDownMask ^ oldDownMask;

	for (uint8_t i=0; m_callback && i<MAX_ENCODERS; ++i)
	{
		if (m_encValues[i] != oldValues[i] || (downChanges&1))
		{
			(*m_callback)(m_userdata, i, m_encValues[i] - oldValues[i], (m_encDownMask & (1 << i)) != 0);
		}
		downChanges >>= 1;
	}
}
