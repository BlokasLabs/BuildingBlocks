/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef BLOKAS_MINI_KEYBOARD_H
#define BLOKAS_MINI_KEYBOARD_H

#include <SpiShield.h>

/// MiniKeyboard class. A specialization of Keypad.
class MiniKeyboard : public Keypad
{
public:
	/// Enum of all they possible keys on the module.
	enum keys_e
	{
		C		= 0,
		CSharp	= 1,
		D		= 2,
		DSharp	= 3,
		E		= 4,
		F		= 5,
		FSharp	= 6,
		G		= 7,
		GSharp	= 8,
		A		= 9,
		ASharp	=10,
		B		=11,
		C2		=12,
		Shift	=13,
	};

	/// isNoteDown returns true if the key, corresponding to the passed in note, is down.
	inline bool isNoteDown(keys_e key) const { return (getKeyStates() & (1 << key)) != 0; }

	/// isShiftDown returns true if the shift key is down.
	inline bool isShiftDown() const { return (getKeyStates() & (1 << Shift)) != 0; }
};

#endif // BLOKAS_MINI_KEYBOARD_H