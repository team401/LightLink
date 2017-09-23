#pragma once
#include <Adafruit_NeoPixel.h>
#include "Definitions.h"
class Strip {
public:
	void init(uint8_t pin, uint16_t length_, neoPixelType type = NEO_GRBW + NEO_KHZ800);
	void setAll(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
	void update();
	uint16_t length;
	uint8_t color = BLACK;
	uint8_t action = SOLID;
    bool blinkState = 0;
	unsigned long lastUpdated = 0;
	unsigned long nextBlink = 0;
	uint8_t blinkCounter = 0;
	Adafruit_NeoPixel pixels;
	void showPixels() { pixels.show(); }
	void setBrightness(uint8_t brightness) { pixels.setBrightness(brightness); }
private:
	bool initialized = false;
	void doAction(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
};