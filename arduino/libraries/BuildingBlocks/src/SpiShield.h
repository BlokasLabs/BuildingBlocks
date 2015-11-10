/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef BLOKAS_SPI_SHIELD_H
#define BLOKAS_SPI_SHIELD_H

#include <Arduino.h>
#include <SPI.h>

// Forward declaration.
class SpiDevice;

/// SpiShield is an abstract base class for the SPI boards.
class SpiShield
{
public:
	/// Returns the number of connected devices supported.
	virtual uint8_t getNumDevices() const = 0;

	/// Returns the device assigned to the given port number.
	virtual SpiDevice *getDevice(uint8_t i) const = 0;

	/// Initializes all the attached devices. Should be called from setup.
	void init();

	/// Updates the state of all the attached devices by requesting new info through the SPI connection. Should be called from loop.
	void update();

protected:
	void initDevice(SpiDevice &device, int slaveSelectPin);
};

/// SpiMiniShield supports up to 2 devices.
class SpiMiniShield : public SpiShield
{
public:
	/// Constructor for when only device A is attached.
	explicit SpiMiniShield(SpiDevice &devA);

	/// Constructor for when both A and B have modules attached.
	SpiMiniShield(SpiDevice &devA, SpiDevice &devB);

	virtual uint8_t getNumDevices() const;
	virtual SpiDevice *getDevice(uint8_t i) const;

private:
	enum { MAX_DEVICES = 2 };

	/// Array of SpiDevices.
	SpiDevice *m_devices[MAX_DEVICES];

	/// Count of devices that were passed in to one of the constructors.
	uint8_t m_numDevices;
};

/// Base class for the individual types of Modules.
class SpiDevice
{
public:
	/// Default constructor.
	SpiDevice();

protected:
	/// Method to initialize the state. Called by SpiShield during setup.
	virtual void init() {}

	/// Method to get an update from the attached module. Called by SpiShield during loop.
	virtual void update() {}

	/// Sets the SPI settings, such as frequency, for the communication with the attached module.
	void setSpiSettings(const SPISettings &settings);

	/// Returns the stored settings.
	const SPISettings &getSpiSettings() const;

	/// Returns the slave select pin that this device should use. Automatically initialized by SpiShield.
	int getSlaveSelectPin() const;

private:
	SPISettings m_settings;
	int m_slaveSelectPin;

	/** SpiShield is declared as a friend class, so it can have special access to the private member variables
	 * and protected methods. This enables reducing the number of publicly accessible methods, and reduces
	 * complexity for users of the modules by keeping the internals hidden by protected and private access
	 * specifiers.
	 */
	friend class SpiShield;
};

#endif // BLOKAS_SPI_SHIELD_H
