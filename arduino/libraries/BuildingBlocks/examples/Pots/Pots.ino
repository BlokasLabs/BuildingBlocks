// Pots example. Assumes Arduino connected to the Mainboard, and the Pots module connected to the first port (SV1).

#include <SPI.h>
#include <BuildingBlocks.h>

// Define an instance of Pots module.
Pots pots;

// Define an instance of the Mainboard module.
Mainboard mainboard(pots);

void setup() {
	// Setup serial communication for the MIDI port.
	Serial1.begin(31250);

	// Initialize the Mainboard module
	mainboard.init();

	// Set the function which will get called when potentiometer events are detected.
	pots.setCallback(&onPotEvent);
}

// Just update the Mainboard in order to receive the events that are happening in the attached modules.
void loop() {
	mainboard.update();
}

// This will get called while executing mainboard.update(), if events were detected.
void onPotEvent(uint8_t pot, uint16_t value) {
	sendMidiCC(0, pot, value >> 3); // Shift 3 bits to the right (equivalent of dividing by 8) in order to remap values from [0;1023] to [0;127].
}

// Send the 3 byte MIDI Control Change message through the Serial port.
void sendMidiCC(uint8_t channel, uint8_t control, uint8_t value) {
	Serial1.write(0xB0 | channel);
	Serial1.write(control);
	Serial1.write(value);
}
