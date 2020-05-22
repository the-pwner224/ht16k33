/*
 * This controls the HT16K33, as connected in the Adafruit
 *   I2C backpacks with 4-digit 7-segment displays. This
 *   description focues on implementation usability, not
 *   technical correctness of all the minor details.
 *
 * The HT16K33 has 5 16-bit 'memory cells' mapped to the LED
 *   pins. With the backpack, only the first 8 bits of each
 *   cell are used, so it can be thought of as having 8-bit
 *   memory cells with a gap cell between actual cells.
 *
 * Memory cells 0/1/3/4 control the digits, from left to right.
 * Memory cell 2 controls the middle dots, and on the 1.2"
 *   display, also the accessory dots.
 *
 * Memory cell format:
 * labels: dGFE DCBA (padding)
 *   bits: 0000 0000 0000 0000
 *
 *     A
 *   /---\
 * F |   | B
 *   >-G-<
 * E |   | C
 *   \---/ .
 *     D   d
 *
 * Bits A-F control the segments of the 7-segment display,
 *   starting at the top and going clockwise around the edge.
 *   Bit G controls the middle bar segment.
 * On both displays, bit B (0x20) of memory cell 2 controls
 *   both middle dots (no individual control).
 *
 * On the 0.56" display, the 8th bit (label d) of cells 0/1/3/4
 *   controls the decimal point right after that digit.
 *
 * On the 1.2" display, the other dots are mapped to these bits:
 *   /--------------\
 *   |C     .   E   |
 *   |D     .       |
 *   \--------------/
 *
 * To set the display, send over I2C 0x00 and then the memory
 * contents. For example, to turn everything on:
 *   0x00 // start indicator
 *   0xFF // leftmost digit
 *   0x00 // padding - can be any number?
 *   0xFF // mid-left digit
 *   0x00
 *   0xFF // dots
 *   0x00
 *   0xFF // mid-right digit
 *   0x00
 *   0xFF // rightmost digit
 *   0x00
 *
 * There are a few other control codes for brightness, blinking, etc.
 * Those are pretty straighforward, just find them in the code below.
 */

#include "bkpklib.h"



Backpack::Backpack(byte addrOffset) {
	addr = 0x70 + addrOffset;
}

void Backpack::init() {
	Wire.begin();
	Wire.beginTransmission(addr);
	Wire.write(0x21);
	Wire.endTransmission();

	Backpack::clear();
	Backpack::flush();
	Backpack::setBrightness(15);
	Backpack::setBlinkRate(0);
}





void Backpack::clear() {
	for (int i = 0; i < 5; ++i)
		buf[i] = 0;
}

void Backpack::fill() {
	for (int i = 0; i < 5; ++i)
		buf[i] = 0xFF;
}

void Backpack::flush() {
	Wire.beginTransmission(addr);
	Wire.write(0x00);
	for (int i = 0; i < 5; ++i) {
		Wire.write(buf[i]);
		Wire.write((byte)0x00);
	}
	Wire.endTransmission();
}





void Backpack::setBrightness(byte b) {
	if (b > 15) b = 15;
	Wire.beginTransmission(addr);
	Wire.write(0xE0 | b);
	Wire.endTransmission();
}

void Backpack::setBlinkRate(byte rate) {
	if (rate > 3) rate = 3;
	Wire.beginTransmission(addr);
	Wire.write(0x81 | rate << 1);
	Wire.endTransmission();
}





void Backpack::setDigit(byte index, byte number) {
	if (index > 1) ++index;
	switch (number) {
		// on 0.56", don't discard a set decimal point (bit 0x80)
		case 0: buf[index] = 0b0111111 | (buf[index] & 0x80); break;
		case 1: buf[index] = 0b0000110 | (buf[index] & 0x80); break;
		case 2: buf[index] = 0b1011011 | (buf[index] & 0x80); break;
		case 3: buf[index] = 0b1001111 | (buf[index] & 0x80); break;
		case 4: buf[index] = 0b1100110 | (buf[index] & 0x80); break;
		case 5: buf[index] = 0b1101101 | (buf[index] & 0x80); break;
		case 6: buf[index] = 0b1111101 | (buf[index] & 0x80); break;
		case 7: buf[index] = 0b0000111 | (buf[index] & 0x80); break;
		case 8: buf[index] = 0b1111111 | (buf[index] & 0x80); break;
		case 9: buf[index] = 0b1101111 | (buf[index] & 0x80); break;
		default: buf[index] = 0x00     | (buf[index] & 0x80); break;
	}
}

void Backpack::setColon(bool b) {
	if (b) buf[2] |= 0x02;
	else   buf[2] &= ~0x02;
}

void Backpack::setClock(byte h, byte m) {
	Backpack::clear();
	Backpack::setDigit(0, h/10);
	Backpack::setDigit(1, h%10);
	Backpack::setDigit(2, m/10);
	Backpack::setDigit(3, m%10);
	Backpack::setColon(true);
}

void Backpack::setNumber(unsigned int num) {
	Backpack::clear();
	Backpack::setClock(num/100, num%100);
	Backpack::setColon(false);
}





void Backpack::setDecimal(byte index, bool b) {
	if (index > 1) ++index;
	if (b) buf[index] |= 0x80;
	else   buf[index] &= ~0x80;
}

void Backpack::setDecimals(bool a, bool b, bool c, bool d) {
	Backpack::setDecimal(0, a);
	Backpack::setDecimal(1, b);
	Backpack::setDecimal(2, c);
	Backpack::setDecimal(3, d);
}





void Backpack::setDotLowerLeft(bool b) {
	if (b) buf[2] |= 0x08;
	else   buf[2] &= ~0x08;
}

void Backpack::setDotUpperLeft(bool b) {
	if (b) buf[2] |= 0x04;
	else   buf[2] &= ~0x04;
}

void Backpack::setDotUpperRight(bool b) {
	if (b) buf[2] |= 0x10;
	else   buf[2] &= ~0x10;
}

void Backpack::setDots(bool lowerLeft, bool upperLeft, bool upperRight) {
	Backpack::setDotLowerLeft(lowerLeft);
	Backpack::setDotUpperLeft(upperLeft);
	Backpack::setDotUpperRight(upperRight);
}
