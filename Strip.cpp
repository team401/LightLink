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

uint32_t Strip::wheel(uint8_t pos) {
	pos = 255 - pos;
	if (pos < 85) {
		return pixels.Color(255 - pos * 3, 0, pos * 3);
	}
	if (pos < 170) {
		pos -= 85;
		return pixels.Color(0, pos * 3, 255 - pos * 3);
	}
	pos -= 170;
	return pixels.Color(pos * 3, 255 - pos * 3, 0);
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
	if (action != lastAction) {
		lastAction = action;
		counter = -1;
		state = 0;
		next = 0;
	}
	switch (action) {
	case SOLID:
		counter = 0;
		setAll(r, g, b, w);
		break;
	case BLINK:
	case SIGNAL:
		if (lastUpdated > next) { //It's time to schedule a new blink
			state = !state;
			if (speed == SLOW) next = lastUpdated + SLOW_BLINK_SPEED;
			if (speed == FAST || action == SIGNAL) next = lastUpdated + FAST_BLINK_SPEED;
			if (action == SIGNAL && state == STATE_ON) counter++;
		}
		if (counter > NUM_BLINKS) { //Blink is over
			state = STATE_OFF;
			counter = 0;
			color = BLACK;
			action = SOLID;
		}
		if (state) {
			setAll(r, g, b, w);
		} else {
			setAll(0, 0, 0, 0);
		}
		break;
	case RACE:
		if (lastUpdated > next) {
			if (speed == SLOW) next = lastUpdated + SLOW_RACE_SPEED;
			if (speed == FAST) next = lastUpdated + FAST_RACE_SPEED;
			counter++;
		}
		if (counter >= length) {
			counter = 0;
		}
		setAll(0, 0, 0, 0);
		pixels.setPixelColor(counter, r, g, b, w);
		break;
	case BOUNCE:
		if (lastUpdated > next) {
			if (speed == SLOW) next = lastUpdated + SLOW_BOUNCE_SPEED;
			if (speed == FAST) next = lastUpdated + FAST_BOUNCE_SPEED;
			counter++;
		}
		if (counter >= length) {
			counter = 0;
			state = !state;
		}
		setAll(0, 0, 0, 0);
		if (state) {
			pixels.setPixelColor((length - 1) - counter, r, g, b, w);
		} else {
			pixels.setPixelColor(counter, r, g, b, w);
		}
		break;
	case SPLIT:
		if (lastUpdated > next) {
			if (speed == SLOW) next = lastUpdated + SLOW_SPLIT_SPEED;
			if (speed == FAST) next = lastUpdated + FAST_SPLIT_SPEED;
			counter++;
		}
		setAll(0, 0, 0, 0);
		if (length % 2 == 0) {
			if (counter >= length / 2) {
				counter = 0;
			}
			pixels.setPixelColor(length / 2 - counter - 1, r, g, b, w);
			pixels.setPixelColor(length / 2 + counter, r, g, b, w);
		} else {
			if (counter > length / 2) {
				counter = 0;
			}
			pixels.setPixelColor(length / 2 + counter, r, g, b, w);
			pixels.setPixelColor(length / 2 - counter, r, g, b, w);
		}
		break;
	case BREATHE:
		if (lastUpdated > next) {
			if (speed == SLOW) next = lastUpdated + SLOW_BREATHE_SPEED;
			if (speed == FAST) next = lastUpdated + FAST_BREATHE_SPEED;
			counter++;
		}
		if (counter >= 255) {
			counter = 0;
			state = !state;
		}
		if (state) {
			setAll(map(255 - counter, 0, 255, 0, r), map(255 - counter, 0, 255, 0, g), map(255 - counter, 0, 255, 0, b), map(255 - counter, 0, 255, 0, w));
		} else {
			setAll(map(counter, 0, 255, 0, r), map(counter, 0, 255, 0, g), map(counter, 0, 255, 0, b), map(counter, 0, 255, 0, w));
		}
		break;
	case RAINBOW:
		if (lastUpdated > next) {
			if (speed == SLOW) next = lastUpdated + SLOW_RAINBOW_SPEED;
			if (speed == FAST) next = lastUpdated + FAST_RAINBOW_SPEED;
			counter++;
		}
		if (counter >= 255) {
			counter = 0;
		}
		for (int i = 0; i < length; i++) {
			pixels.setPixelColor(i, wheel(((i * 256 / length) + counter) & 255));
		}
		break;
	}
}