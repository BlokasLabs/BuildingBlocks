/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "Mainboard.h"

enum
{
	PIN_SS1 = 8,
	PIN_SS2 = 7,
	PIN_SS3 = 6,
	PIN_SS4 = 5,
	PIN_SS5 = 9,
	PIN_SS6 = 10
};

void Mainboard::commonInit(SpiDevice *devA, SpiDevice *devB, SpiDevice *devC, SpiDevice *devD, SpiDevice *devE, SpiDevice *devF)
{
	m_devices[0] = devA;
	m_devices[1] = devB;
	m_devices[2] = devC;
	m_devices[3] = devD;
	m_devices[4] = devE;
	m_devices[5] = devF;

	// Configure the Slave Select pins as OUTPUT.
	pinMode(PIN_SS1, OUTPUT);
	pinMode(PIN_SS2, OUTPUT);
	pinMode(PIN_SS3, OUTPUT);
	pinMode(PIN_SS4, OUTPUT);
	pinMode(PIN_SS5, OUTPUT);
	pinMode(PIN_SS6, OUTPUT);

	// Disable all the slaves by setting the Slave Select pin to HIGH.
	digitalWrite(PIN_SS1, 1);
	digitalWrite(PIN_SS2, 1);
	digitalWrite(PIN_SS3, 1);
	digitalWrite(PIN_SS4, 1);
	digitalWrite(PIN_SS5, 1);
	digitalWrite(PIN_SS6, 1);

	if (devA) initDevice(*devA, PIN_SS1);
	if (devB) initDevice(*devB, PIN_SS2);
	if (devC) initDevice(*devC, PIN_SS3);
	if (devD) initDevice(*devD, PIN_SS4);
	if (devE) initDevice(*devE, PIN_SS5);
	if (devF) initDevice(*devF, PIN_SS6);
}

Mainboard::Mainboard(SpiDevice &devA)
	:m_numDevices(1)
{
	commonInit(&devA, NULL, NULL, NULL, NULL, NULL);
}

Mainboard::Mainboard(SpiDevice &devA, SpiDevice &devB)
	:m_numDevices(2)
{
	commonInit(&devA, &devB, NULL, NULL, NULL, NULL);
}

Mainboard::Mainboard(SpiDevice &devA, SpiDevice &devB, SpiDevice &devC)
	:m_numDevices(3)
{
	commonInit(&devA, &devB, &devC, NULL, NULL, NULL);
}

Mainboard::Mainboard(SpiDevice &devA, SpiDevice &devB, SpiDevice &devC, SpiDevice &devD)
	:m_numDevices(4)
{
	commonInit(&devA, &devB, &devC, &devD, NULL, NULL);
}

Mainboard::Mainboard(SpiDevice &devA, SpiDevice &devB, SpiDevice &devC, SpiDevice &devD, SpiDevice &devE)
	:m_numDevices(5)
{
	commonInit(&devA, &devB, &devC, &devD, &devE, NULL);
}

Mainboard::Mainboard(SpiDevice &devA, SpiDevice &devB, SpiDevice &devC, SpiDevice &devD, SpiDevice &devE, SpiDevice &devF)
	:m_numDevices(6)
{
	commonInit(&devA, &devB, &devC, &devD, &devE, &devF);
}

uint8_t Mainboard::getNumDevices() const
{
	return m_numDevices;
}

SpiDevice *Mainboard::getDevice(uint8_t i) const
{
	if (i < m_numDevices)
		return m_devices[i];

	return NULL;
}
