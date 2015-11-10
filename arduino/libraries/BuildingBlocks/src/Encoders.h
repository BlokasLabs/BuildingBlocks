/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef BLOKAS_ENCODERS_H
#define BLOKAS_ENCODERS_H

#include <SpiShield.h>

/// Encoders class.
class Encoders : public SpiDevice
{
public:
	/// The number of encoders supported by this board.
	enum { MAX_ENCODERS = 4 };

	/// Default constructor.
	Encoders();

	/// Returns the current value of a given encoder, or 0 if enc argument is out of range.
	int16_t getValue(uint8_t enc) const;

	/// Returns whether the given encoder is currently down, or false if en argument is out of range.
	bool isDown(uint8_t enc) const;

	/** Sets the callback method which gets called when various events occur.
	 *  An example of a suitable signature for the callback function:
	 *
	 *    void myEncoderCallback(uint8_t enc, int8_t change, bool isDown);
	 *
	 *  \param fptr Pointer to function which will get called when Encoder events happen. Can be set to NULL to disable the callbacks.
	 *  \param enc The encoder id.
	 *  \param change The change in encoder's value since the last time this callback was called.
	 *  \param isDown True if the encoder for which this callback was invoked is currently pushed down.
	 */
	void setCallback(void (*fptr)(uint8_t enc, int8_t change, bool isDown));

protected:
	virtual void init();
	virtual void update();

private:
	/// Stores the values of the encoders.
	int16_t m_encValues[MAX_ENCODERS];

	/// Stores the bitmask of encoders down states.
	uint8_t m_encDownMask;

	/// Stores the pointer to function for event callbacks.
	void (*m_callback)(uint8_t enc, int8_t change, bool isDown);
};

#endif // BLOKAS_ENCODERS_H
