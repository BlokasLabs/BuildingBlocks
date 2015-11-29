// CV_2x2 example. Assumes Arduino connected to the Mainboard, the CV_2x2 module connected to the first port (SV1) and a Pots module connected to the second port (SV2).

#include <SPI.h>
#include <BuildingBlocks.h>

// Define instances of the modules.
CV_2x2 cv;
Pots pots;

// Define an instance of the Mainboard module.
Mainboard mainboard(cv, pots);

void setup() {
	// Setup serial communication for the MIDI port.
	Serial1.begin(31250);

	// Initialize the Mainboard module
	mainboard.init();

	pots.setCallback(&onPotCallback);
}

void onPotCallback(void *userdata, uint8_t pot, uint16_t value) {
	if (pot == 0) {
		// Translate the value from 10 bit to 12 bit precision by shifting 2 bits left.
		cv.setCV(CV_2x2::CH_1, value << 2);
	} else if (pot == 1) {
		cv.setGate(CV_2x2::CH_1, value > 511);
	} else if (pot == 2) {
		cv.setCV(CV_2x2::CH_2, value << 2);
	} else if (pot == 3) {
		cv.setGate(CV_2x2::CH_2, value > 511);
	}
}

// Just update the Mainboard in order to receive the events that are happening in the attached modules,
// as well as get the updates sent to modules with output.
void loop() {
	mainboard.update();
}

