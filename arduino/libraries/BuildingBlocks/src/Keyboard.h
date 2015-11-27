/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef BLOKAS_KEYBOARD_H
#define BLOKAS_KEYBOARD_H

#include <Keypad.h>

/// Keyboard class. A specialization of Keypad.
class Keyboard : public Keypad
{
public:
	/// Enum of all they possible keys on the module.
	enum keys_e
	{
		C			= 0,
		CSharp		= 1,
		D			= 2,
		DSharp		= 3,
		E			= 4,
		F			= 5,
		FSharp		= 6,
		G			= 7,
		GSharp		= 8,
		A			= 9,
		ASharp		=10,
		B			=11,
		C2			=12,
		OctaveDown	=13,
		OctaveUp	=14,
	};
};

/** A keyboard class which implements switching of the octaves.
 * It also exposes key press callbacks using the actual note values.
 */
class OctaveKeyboard : public Keyboard
{
public:
	OctaveKeyboard();
	~OctaveKeyboard();

	/// The middle C is the base note.
	enum { BASE_NOTE = 60 };

	/// Sets the current octave.
	void setOctave(int8_t octave);
	int8_t getOctave() const;

	void setCallback(void (*fptr)(void *userdata, uint8_t note, bool isDown), void *userdata = NULL);

private:
	/** This method is used with Keyboard / Keypad class, to receive events
	 * and to translate them.
	 */
	static void internalKeypadCallback(void *userdata, uint8_t key, bool isDown);
	void handleKeypadEvent(uint8_t key, bool isDown);

	uint8_t translateKeyToNote(uint8_t key) const;

	void (*m_callback)(void *userdata, uint8_t note, bool isDown);
	void *m_userdata;

	/// Current octave.
	int8_t m_octave;

	/** Stores the note value played by each key. Referenced to send a
	 * note off when the octave is changed, and the key is released.
	 */
	uint8_t m_notePlayed[13];
};

#endif // BLOKAS_KEYBOARD_H
