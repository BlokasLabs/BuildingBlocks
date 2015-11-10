/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef BLOKAS_MAINBOARD_H
#define BLOKAS_MAINBOARD_H

#include <SpiShield.h>

/// Mainboard supports up to 6 modules.
class Mainboard : public SpiShield
{
public:
	/// Constructor for when only module A is attached.
	explicit Mainboard(SpiDevice &devA);

	/// Constructor for when A and B have modules attached.
	Mainboard(SpiDevice &devA, SpiDevice &devB);

	/// Constructor for when A, B and C have modules attached.
	Mainboard(SpiDevice &devA, SpiDevice &devB, SpiDevice &devC);

	/// Constructor for when A, B, C and D have modules attached.
	Mainboard(SpiDevice &devA, SpiDevice &devB, SpiDevice &devC, SpiDevice &devD);

	/// Constructor for when A, B, C, D and E have modules attached.
	Mainboard(SpiDevice &devA, SpiDevice &devB, SpiDevice &devC, SpiDevice &devD, SpiDevice &devE);

	/// Constructor for when A, B, C, D, E and F have modules attached.
	Mainboard(SpiDevice &devA, SpiDevice &devB, SpiDevice &devC, SpiDevice &devD, SpiDevice &devE, SpiDevice &devF);

	virtual uint8_t getNumDevices() const;
	virtual SpiDevice *getDevice(uint8_t i) const;

private:
	enum { MAX_DEVICES = 6 };

	void commonInit(SpiDevice *devA, SpiDevice *devB, SpiDevice *devC, SpiDevice *devD, SpiDevice *devE, SpiDevice *devF);

	/// Array of SpiDevices.
	SpiDevice *m_devices[MAX_DEVICES];

	/// Count of devices that were passed in to one of the constructors.
	uint8_t m_numDevices;
};

#endif // BLOKAS_MAINBOARD_H
