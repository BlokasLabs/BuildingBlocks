/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef BLOKAS_POTS_H
#define BLOKAS_POTS_H

#include <SpiShield.h>

/// Pots class.
class Pots : public SpiDevice
{
public:
	/// The number of pots supported by this board.
	enum { MAX_POTS = 4 };

	/// Default constructor.
	Pots();

	/// Returns the value of a given potentiometer, or 0 if pot is out of range.
	uint16_t getValue(uint8_t pot) const;

	/** Sets the callback method which gets called when various events occur.
	 *  An example of a suitable signature for the callback function:
	 *
	 *    void myPotentiometerCallback(uint8_t pot, uint16_t value);
	 *
	 *  \param fptr Pointer to function which will get called when Pots events happen. Can be set to NULL to disable the callbacks.
	 *  \param pot The potentiometer id.
	 *  \param value The absolute value of the potentiometer.
	 */
	void setCallback(void (*fptr)(uint8_t pot, uint16_t value));

protected:
	virtual void init();
	virtual void update();

private:
	/// Stores the values of the potentiometers.
	uint16_t m_potValues[MAX_POTS];

	/// Stores the pointer to function for event callbacks.
	void (*m_callback)(uint8_t pot, uint16_t value);
};

#endif // BLOKAS_POTS_H
