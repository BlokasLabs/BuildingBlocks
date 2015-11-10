/* 
 * Copyright (C) 2015 Blokas Labs
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef BLOKAS_KEYPAD_H
#define BLOKAS_KEYPAD_H

#include <SpiShield.h>

/// Keypad class. Supports up to 16 keys.
class Keypad : public SpiDevice
{
public:
	/// Default constructor.
	Keypad();

	/// getKeyStates returns all of the 16 tracked key states at once, as a bitmask.
	uint16_t getKeyStates() const;

	/// isKeyDown checks if the given key is down, and returns true if so. If the passed in key is out of range, it will return false.
	bool isKeyDown(uint8_t key) const;

	/** Sets the callback method which gets called when various events occur.
	 *  An example of a suitable signature for the callback function:
	 *
	 *    void myKeypadCallback(uint8_t key, bool isDown);
	 *
	 *  \param fptr Pointer to function which will get called when Keypad events happen. Can be set to NULL to disable the callbacks.
	 *  \param key The key id.
	 *  \param isDown True if the key for which this callback was invoked is currently pushed down.
	 */
	void setCallback(void (*fptr)(uint8_t key, bool isDown));

protected:
	virtual void init();
	virtual void update();

private:
	/// Stores the isDown states of the 16 keys.
	uint16_t m_keyStates;

	/// Stores the pointer to function for event callbacks.
	void (*m_callback)(uint8_t key, bool isDown);
};

#endif // BLOKAS_KEYPAD_H
