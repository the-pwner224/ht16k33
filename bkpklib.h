#ifndef Backpack_H
#define Backpack_H

#include <Wire.h>
#include "Arduino.h"

class Backpack {
	public:
		Backpack(byte addrOffset);     // I2C address = 0x70 + addrOffset
		void init();                   // initialize HT16K33 over i2c

		void clear();                  // all segments off, does not flush()
		void fill();                   // all segments on, does not flush()
		void flush();                  // write changes to display over i2c

		// these two have immediate effect, with no flush() required
		void setBrightness(byte);      // 0-15; 0 is dim, not off
		void setBlinkRate(byte);       // 0/1/2/3 for no/fast/med/slow blink


		// index = 0/1/2/3 left to right; number = [0..9], any other to clear digit
		void setDigit(byte index, byte number);
		void setColon(bool);           // middle colon

		void setClock(byte h, byte m); // set pairs of digits, and colon
		void setNumber(unsigned int);  // set entire display to number [0..9999]


		// 0.56" display only, decimal point after each digit (index 0/1/2/3)
		void setDecimal(byte index, bool);
		void setDecimals(bool, bool, bool, bool); // left to right

		// 1.2" display only, additional dots
		void setDotLowerLeft(bool);
		void setDotUpperLeft(bool);
		void setDotUpperRight(bool);
		void setDots(bool lowerLeft, bool upperLeft, bool upperRight);

	private:
		byte addr;
		byte buf[5];
};

#endif
