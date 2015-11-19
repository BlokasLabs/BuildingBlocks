// https://github.com/BlokasLabs/usbmidi

// Mainboard with Pots module connected to SV1.

#include <PluggableUSB.h>
#include <usbmidi.h>

#include <spi.h>
#include <BuildingBlocks.h>

Pots pots;
Mainboard mainboard(pots);

void setup() {
	Serial.begin(9600);
	mainboard.init();
	pots.setCallback(&onPotCallback);
}

void onPotCallback(uint8_t pot, uint16_t value) {
	sendCC(0, pot, value >> 3);
}

void sendCC(uint8_t channel, uint8_t control, uint8_t value) {
	USBMIDI.write(0xB0 | (channel & 0xf));
	USBMIDI.write(control & 0x7f);
	USBMIDI.write(value & 0x7f);
}

void loop() {
	while (USBMIDI.available()) {
		u8 b = USBMIDI.read();
	}

	mainboard.update();
	USBMIDI.flush();

	for (int i=0; i<4; ++i)
	{
		Serial.print(pots.getValue(i), DEC);
		Serial.write(' ');
	}
	Serial.write('\n');
}
