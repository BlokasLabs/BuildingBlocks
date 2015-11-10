/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "SpiShield.h"

enum
{
	PIN_SS1 = 8,
	PIN_SS2 = 7,
};

void SpiShield::init()
{
	SPI.begin();

	const uint8_t n = getNumDevices();
	for (uint8_t i=0; i<n; ++i)
	{
		getDevice(i)->init();
	}
}

void SpiShield::update()
{
	const uint8_t n = getNumDevices();
	for (uint8_t i=0; i<n; ++i)
	{
		SpiDevice *device = getDevice(i);

		if (!device)
			continue;

		device->update();
	}
}

void SpiShield::initDevice(SpiDevice &device, int slaveSelectPin)
{
	device.m_slaveSelectPin = slaveSelectPin;
}

SpiMiniShield::SpiMiniShield(SpiDevice &devA)
	:m_numDevices(1)
{
	m_devices[0] = &devA;
	m_devices[1] = NULL;

	pinMode(PIN_SS1, OUTPUT);
	pinMode(PIN_SS2, OUTPUT);

	digitalWrite(PIN_SS1, 1);
	digitalWrite(PIN_SS2, 1);

	initDevice(devA, PIN_SS1);
}

SpiMiniShield::SpiMiniShield(SpiDevice &devA, SpiDevice &devB)
	:m_numDevices(2)
{
	m_devices[0] = &devA;
	m_devices[1] = &devB;

	pinMode(PIN_SS1, OUTPUT);
	pinMode(PIN_SS2, OUTPUT);

	digitalWrite(PIN_SS1, 1);
	digitalWrite(PIN_SS2, 1);

	initDevice(devA, PIN_SS1);
	initDevice(devB, PIN_SS2);
}

uint8_t SpiMiniShield::getNumDevices() const
{
	return m_numDevices;
}

SpiDevice *SpiMiniShield::getDevice(uint8_t i) const
{
	if (i < m_numDevices)
		return m_devices[i];

	return NULL;
}

SpiDevice::SpiDevice()
	:m_slaveSelectPin(-1)
{
}

int SpiDevice::getSlaveSelectPin() const
{
	return m_slaveSelectPin;
}

void SpiDevice::setSpiSettings(const SPISettings &settings)
{
	m_settings = settings;
}

const SPISettings &SpiDevice::getSpiSettings() const
{
	return m_settings;
}
