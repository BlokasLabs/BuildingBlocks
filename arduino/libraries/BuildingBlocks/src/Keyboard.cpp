/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "Keyboard.h"

// Constant to be used for 'no note'
enum { INVALID_NOTE = 0xff };

OctaveKeyboard::OctaveKeyboard()
	:m_callback(NULL)
	,m_userdata(NULL)
	,m_octave(0)
{
	for (uint8_t i=0; i<13; ++i)
	{
		m_notePlayed[i] = INVALID_NOTE;
	}
	Keyboard::setCallback(&internalKeypadCallback, this);
}

OctaveKeyboard::~OctaveKeyboard()
{
	Keyboard::setCallback(NULL, NULL);
}

void OctaveKeyboard::setOctave(int8_t octave)
{
	if (octave < -5) octave = -5;
	else if (octave > 5) octave = 5;

	m_octave = octave;
}

int8_t OctaveKeyboard::getOctave() const
{
	return m_octave;
}

void OctaveKeyboard::setCallback(void (*fptr)(void *userdata, uint8_t note, bool isDown), void *userdata)
{
	m_callback = fptr;
	m_userdata = userdata;
}

void OctaveKeyboard::internalKeypadCallback(void *userdata, uint8_t key, bool isDown)
{
	OctaveKeyboard *thisKeyboard = static_cast<OctaveKeyboard*>(userdata);
	thisKeyboard->handleKeypadEvent(key, isDown);
}

void OctaveKeyboard::handleKeypadEvent(uint8_t key, bool isDown)
{
	if (key == OctaveDown)
	{
		if (isDown) setOctave(getOctave() - 1);
	}
	else if (key == OctaveUp)
	{
		if (isDown) setOctave(getOctave() + 1);
	}
	else
	{
		// Check if a note is already being played, if so, fire the note off event.
		if (m_notePlayed[key] != INVALID_NOTE)
			(*m_callback)(m_userdata, m_notePlayed[key], false);

		if (isDown)
		{
			uint8_t note = translateKeyToNote(key);

			if (note != INVALID_NOTE)
			{
				m_notePlayed[key] = note;
				(*m_callback)(m_userdata, note, true);
			}
		}
		else
		{
			m_notePlayed[key] = INVALID_NOTE;
		}
	}
}

uint8_t OctaveKeyboard::translateKeyToNote(uint8_t key) const
{
	int16_t value = BASE_NOTE + getOctave() * 12 + key;

	// Check the validity of value.
	if (value >= 0x80) value = INVALID_NOTE;
	else if (value < 0) value = INVALID_NOTE;

	return value;
}
