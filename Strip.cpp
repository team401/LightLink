#include "Strip.h"
#include "Definitions.h"
#include <Arduino.h>

void Strip::init(uint8_t pin, uint16_t length_, neoPixelType type) {
	initialized = true;
	length = length_;
	pixels = Adafruit_NeoPixel(length, pin, type);
	pixels.begin();
	showPixels();
}

void Strip::setAll(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	if (initialized) {
		for (int i = 0; i < length; i++) {
			pixels.setPixelColor(i, r, g, b, w);
		}
	}
}

void Strip::update() {
	if (initialized) {
		lastUpdated = millis();
		switch (color) {
		case RED:
			doAction(255, 0, 0, 0);
			break;
		case ORANGE:
			doAction(255, 32, 0, 0);
			break;
		case YELLOW:
			doAction(255, 128, 0, 0);
			break;
		case GREEN:
			doAction(0, 255, 0, 0);
			break;
		case BLUE:
			doAction(0, 0, 255, 0);
			break;
		case VIOLET:
			doAction(255, 0, 255, 0);
			break;
		case WHITE:
			doAction(0, 0, 0, 255);
			break;
		case BLACK:
			doAction(0, 0, 0, 0);
			break;
		}
		showPixels();
	}
}

void Strip::doAction(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	switch (action) {
	case SOLID:
		blinkCounter = 0;
		setAll(r, g, b, w);
		break;
	case BLINK_SLOW:
	case BLINK_FAST:
		if (lastUpdated > nextBlink) { //It's time to schedule a new blink
			blinkState = !blinkState;
			if (action == BLINK_SLOW) nextBlink = lastUpdated + SLOW_BLINK_SPEED;
			if (action == BLINK_FAST) nextBlink = lastUpdated + FAST_BLINK_SPEED;
			if (action == BLINK_FAST) blinkCounter++;
		}
		if (blinkCounter > NUM_BLINKS * 2) { //Blink is over
			blinkState = STATE_OFF;
			blinkCounter = 0;
			color = BLACK;
			action = SOLID;
		}
		if (blinkState) {
			setAll(r, g, b, w);
		} else {
			setAll(0, 0, 0, 0);
		}
		break;
	}
}