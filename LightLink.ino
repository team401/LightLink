/*
 Name:		LightLink.ino
 Created:	9/21/2017 8:11:22 AM
 Author:	cameronearle
*/

#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "Strip.h"
#include "Definitions.h"

volatile uint8_t command[3]; //Array to hold the command being built
uint8_t cursor = 0; //Cursor to track array position
uint8_t current; //Current character being received
Strip strips[16] = { Strip() }; //Array of strips 
uint8_t stripCursor = 0;

void addStrip(uint8_t pin, int length, neoPixelType type = NEO_GRBW + NEO_KHZ800) {
	if (stripCursor < 16) {
		strips[stripCursor++] = Strip(pin, length, type);
	}
}

void setup() {
	Wire.begin(0x42); //<- SET I2C ADDRESS HERE
	Wire.onReceive(listener);

	//ADD STRIPS HERE
	//addStrip(pin, length); <- Strip 0
	//addStrip(pin, length); <- Strip 1
}

// the loop function runs over and over again until power down or reset
void loop() {
	for (int i = 0; i < 16; i++) {
		strips[i].update();
	}
	delay(1);
}

// Listens for I2C events and builds the command
void listener(int numBytes) {
	while (Wire.available()) {
		current = Wire.read();
		if (current == START) {
			cursor = 0;
		} else if (current == END) {
			update();
		} else {
			command[cursor++] = current;
		}
		if (cursor > 2) {
			cursor = 0;
		}
	}
}

// Runs the current command
void update() {
	uint8_t strip = command[0] - 1;
	uint8_t color = command[1];
	uint8_t action = command[2];

	strips[strip].color = color;
	strips[strip].action = action;
}

