// Encoders example. Assumes Arduino connected to the Mainboard, and the Encoders module connected to the first port (SV1).

#include <SPI.h>
#include <BuildingBlocks.h>

// Define an instance of Encoders module.
Encoders encoders;

// Define an instance of the Mainboard module.
Mainboard mainboard(encoders);

void setup() {
	// Setup serial communication for the MIDI port.
	Serial1.begin(31250);

	// Initialize the Mainboard module
	mainboard.init();

	// Set the function which will get called when encoder events are detected.
	encoders.setCallback(&onEncEvent);
}

// Just update the Mainboard in order to receive the events that are happening in the attached modules.
void loop() {
	mainboard.update();
}

int16_t clamp(int16_t value, int16_t min, int16_t max) {
	if (value < min) value = min;
	else if (value > max) value = max;
	return value;
}

// This will get called while executing mainboard.update(), if events were detected.
void onEncEvent(uint8_t enc, int8_t change, bool isDown) {
	sendMidiCC(0, enc, clamp(encoders.getValue(enc), 0, 127)); // clamp the value between 0 and 127
}

// Send the 3 byte MIDI Control Change message through the Serial port.
void sendMidiCC(uint8_t channel, uint8_t control, uint8_t value) {
	Serial1.write(0xB0 | channel);
	Serial1.write(control);
	Serial1.write(value);
}
