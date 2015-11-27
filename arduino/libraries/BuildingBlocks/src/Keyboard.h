/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef BLOKAS_KEYBOARD_H
#define BLOKAS_KEYBOARD_H

#include <SpiShield.h>

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

#endif // BLOKAS_KEYBOARD_H
