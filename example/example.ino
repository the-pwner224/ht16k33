// In your code, call Wire.begin() before using this library.

// The base address of the i2c backpacks is 0x70.
// Jumping A0, A1, and A2 add 1, 2, and 4, respectively, to 0x70.
// addrOffset should be just the excess past 0x70. So if you jump
//   A0 to get i2c address 0x71, pass 1 to the Backpack constructor.

#include <Wire.h>
#include <bkpklib.h>

Backpack b = Backpack(0); // address offset, added to 0x70

void setup() {
	Wire.begin();
	b.init();
}

void loop() {
	delay(400);
	b.setBrightness(0);
	b.fill();
	b.flush();

	for (int i = 0; i < 16; ++i) {
		b.setBrightness(i);
		delay(200);
	}
	b.clear();

	for (int i = 3; i >= 0; --i) {
		for (int j = 0; j < 10; ++j) {
			b.setDigit(i, j);
			b.flush();
			delay(100);
		}
		b.clear();
	}

	b.setNumber(1337);
	b.flush();
	b.setBlinkRate(1);
	delay(4000);
	b.setBlinkRate(2);
	delay(4000);
	b.setBlinkRate(3);
	delay(6000);
	b.setBlinkRate(0);
	delay(2000);

	b.setDigit(1, 10);
	b.flush();
	delay(1000);
	b.setColon(true);
	b.flush();
	delay(500);
	b.setColon(false);
	b.flush();
	delay(500);
	b.setColon(true);
	b.flush();
	delay(500);
	b.setColon(false);
	b.flush();
	delay(500);

	b.setClock(24, 59);
	b.flush();
	delay(2000);

	b.setDecimal(0, true);
	b.setDots(true, false, false);
	b.flush();
	delay(1000);
	b.setDecimal(1, true);
	b.setDecimal(0, false);
	b.setDots(false, true, false);
	b.flush();
	delay(1000);
	b.setDecimal(2, true);
	b.setDecimal(1, false);
	b.setDots(false, false, true);
	b.flush();
	delay(1000);
	b.setDecimal(3, true);
	b.setDecimal(2, false);
	b.setDots(true, true, true);
	b.flush();
	delay(1000);

	b.setDecimals(true, true, true, true);
	b.flush();
	delay(500);
	b.setDigit(0, 1);
	b.flush(); delay(500);
	b.setDigit(1, 3);
	b.flush(); delay(500);
	b.setDigit(2, 3);
	b.flush(); delay(500);
	b.setDigit(3, 7);
	b.flush(); delay(500);

	for (int i = 16; i >= 0; --i) {
		b.setBrightness(i);
		delay(200);
	}
	delay(1000);
	b.clear();
	b.flush();
	delay(1000);
}
