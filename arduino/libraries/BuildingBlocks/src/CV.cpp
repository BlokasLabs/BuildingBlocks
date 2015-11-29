/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "CV.h"

#include <SpiShield.h>

CV_2x2::CV_2x2()
	:m_dirty(true)
{
	m_channels[CH_1] = 0;
	m_channels[CH_2] = 0;
}

void CV_2x2::setCV(Channel ch, uint16_t value)
{
	if (ch != CH_1 && ch != CH_2)
		return;

	m_channels[ch] = (m_channels[ch] & 0x8000) | (value & 0x0fff);
	m_dirty = true;
}

void CV_2x2::setGate(Channel ch, bool on)
{
	if (ch != CH_1 && ch != CH_2)
		return;

	m_channels[ch] = (m_channels[ch] & 0x0fff) | (on ? 0x8000 : 0);
	m_dirty = true;
}

uint16_t CV_2x2::getCV(Channel ch) const
{
	if (ch != CH_1 && ch != CH_2)
		return 0;

	return m_channels[ch] & 0x0fff;
}

bool CV_2x2::getGate(Channel ch) const
{
	if (ch != CH_1 && ch != CH_2)
		return false;

	return (m_channels[ch] & 0x8000) != 0;
}

void CV_2x2::init()
{
	setSpiSettings(SPISettings(125000, MSBFIRST, SPI_MODE1));
}

void CV_2x2::update()
{
	if (!m_dirty)
		return;

	m_dirty = false;

	digitalWrite(getSlaveSelectPin(), 0);
	SPI.beginTransaction(getSpiSettings());
	delay(1);

	uint16_t result[NUM_CHANNELS];

	for (uint8_t i=0; i<NUM_CHANNELS; ++i)
	{
		result[i] = SPI.transfer16(m_channels[i]);
		delayMicroseconds(100);
	}

	SPI.endTransaction();
	digitalWrite(getSlaveSelectPin(), 1);
	delay(1);
}
