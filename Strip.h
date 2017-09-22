#pragma once
#include <Adafruit_NeoPixel.h>
#include "Definitions.h"
class Strip {
public:
	Strip() {}
	Strip(uint8_t pin, int length_, neoPixelType type = NEO_GRBW + NEO_KHZ800);
	void setAll(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
	void update();
	int length;
	volatile uint8_t color = BLACK;
	volatile uint8_t action = SOLID;
	bool blinkState = 0;
	unsigned long lastUpdated = 0;
	unsigned long nextBlink = 0;
	uint8_t blinkCounter = 0;
	Adafruit_NeoPixel pixels;
private:
	bool initialized = false;
	void doAction(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
};