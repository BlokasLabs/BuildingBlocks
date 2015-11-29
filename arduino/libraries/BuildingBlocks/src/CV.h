/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef BLOKAS_CV_H
#define BLOKAS_CV_H

#include <SpiShield.h>

class CV_2x2 : public SpiDevice
{
public:
	CV_2x2();

	enum Channel
	{
		CH_1 = 0,
		CH_2 = 1,

		NUM_CHANNELS
	};

	/// Set the 12bit CV value for a channel. Value can be in the range of [0;4095].
	void setCV(Channel ch, uint16_t value);

	/// Set the gate on / off for a channel.
	void setGate(Channel ch, bool on);

	/// Get the 12 bit CV value of a channel.
	uint16_t getCV(Channel ch) const;

	/// Get the gate state of a channel.
	bool getGate(Channel ch) const;

protected:
	virtual void init();
	virtual void update();

private:
	uint16_t m_channels[NUM_CHANNELS];

	bool m_dirty;
};

#endif // BLOKAS_CV_H
