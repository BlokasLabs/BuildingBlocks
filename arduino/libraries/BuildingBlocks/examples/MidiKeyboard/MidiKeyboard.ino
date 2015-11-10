// MidiKeyboard example. Assumes Arduino connected to the Mainboard, and the MiniKeyboard module connected to the first port (SV1).

#include <SPI.h>
#include <BuildingBlocks.h>

// Define an instance of MiniKeyboard module.
MiniKeyboard keyboard;

// Define an instance of the Mainboard module.
Mainboard mainboard(keyboard);

void setup() {
	// Setup serial communication for the MIDI port.
	Serial1.begin(31250);

	// Initialize the Mainboard module
	mainboard.init();

	// Set the function which will get called when keyboard events are detected.
	keyboard.setCallback(&onKeyEvent);
}

// Just update the Mainboard in order to receive the events that are happening in the attached modules.
void loop() {
	mainboard.update();
}

// This will get called while executing mainboard.update(), if events were detected.
void onKeyEvent(uint8_t key, bool isDown) {
	sendMidiNote(0, 60 + key, isDown ? 100 : 0);
}

// Send the 3 byte MIDI Note On / Off message through the Serial port.
void sendMidiNote(uint8_t channel, uint8_t pitch, uint8_t velocity) {
	Serial1.write(0x90 | channel);
	Serial1.write(pitch);
	Serial1.write(velocity);
}
