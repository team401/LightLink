// Includes
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

// Word definitions
#define START 0x00 //The commmand starting character
#define END 0xFF //The command ending character

// Color characters
#define RED 0x01
#define ORANGE 0x02
#define YELLOW 0x03
#define GREEN 0x04
#define BLUE 0x05
#define VIOLET 0x06
#define WHITE 0x07
#define BLACK 0x08

// Speed characters
#define SLOW 0x01
#define FAST 0x02

// Action characters
#define SOLID 0x01
#define BLINK 0x02
#define SIGNAL 0x03
#define RACE 0x04
#define BOUNCE 0x05
#define SPLIT 0x06
#define BREATHE 0x07
#define RAINBOW 0x08

// Speeds and Numbers
#define SLOW_BLINK_SPEED 300
#define FAST_BLINK_SPEED 75
#define NUM_BLINKS 5
#define SLOW_RACE_SPEED 30
#define FAST_RACE_SPEED 15
#define SLOW_BOUNCE_SPEED 30
#define FAST_BOUNCE_SPEED 15
#define SLOW_SPLIT_SPEED 60
#define FAST_SPLIT_SPEED 30
#define SLOW_BREATHE_SPEED 6
#define FAST_BREATHE_SPEED 3
#define SLOW_RAINBOW_SPEED 6
#define FAST_RAINBOW_SPEED 2

// States
#define STATE_OFF 0
#define STATE_ON 1
#define STATE_FORWARD 0
#define STATE_REVERSE 1


// Strip class
class Strip {
public:
  void init(uint8_t pin, uint16_t length_, neoPixelType type = NEO_GRBW + NEO_KHZ800, uint16_t offset_ = 0, bool reverse_ = false) {
  	initialized = true;
	length = length_;
  offset = offset_;
  reverse = reverse_;
	pixels = Adafruit_NeoPixel(length, pin, type);
	pixels.begin();
	showPixels();
  }
  void setAll(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  	if (initialized) {
		for (int i = offset; i < offset + length; i++) {
			pixels.setPixelColor(i, r, g, b, w);
		}
	}
  }

  void setPixelColor(uint16_t pixel, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    if (reverse) {
      pixels.setPixelColor((length + offset) - pixel, r, g, b, w);
    } else {
      pixels.setPixelColor(pixel + offset, r, g, b, w);
    }
  }

  void setPixelColor(uint16_t pixel, uint32_t c) {
    if (reverse) {
      pixels.setPixelColor((length + offset) - pixel, c);
    } else {
      pixels.setPixelColor(pixel + offset, c);
    }
  }
  
  void update() {
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
  uint16_t length;
  uint16_t offset;
  bool reverse;
  uint8_t color = BLACK;
  uint8_t action = SOLID;
  uint8_t speed = SLOW;
  uint8_t lastAction = SOLID;
  uint8_t colorFromSignal = BLACK;
  uint8_t actionFromSignal = SOLID;
  uint8_t speedFromSignal = SLOW;
    bool state = 0;
  unsigned long lastUpdated = 0;
  unsigned long next = 0;
  long counter = 0;
  Adafruit_NeoPixel pixels;
  void showPixels() { pixels.show(); }
  void setBrightness(uint8_t brightness) { pixels.setBrightness(brightness); }
private:
  bool initialized = false;
    uint32_t wheel(uint8_t pos) {
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
  void doAction(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
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
			color = colorFromSignal;
			action = actionFromSignal;
			speed = speedFromSignal;
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
		setPixelColor(counter, r, g, b, w);
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
			setPixelColor((length - 1) - counter, r, g, b, w);
		} else {
			setPixelColor(counter, r, g, b, w);
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
			setPixelColor(length / 2 - counter - 1, r, g, b, w);
			setPixelColor(length / 2 + counter, r, g, b, w);
		} else {
			if (counter > length / 2) {
				counter = 0;
			}
			setPixelColor(length / 2 + counter, r, g, b, w);
			setPixelColor(length / 2 - counter, r, g, b, w);
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
			setPixelColor(i, wheel(((i * 256 / length) + counter) & 255));
		}
		break;
	}
  }
};

// ino start

/*
 Name:		LightLink.ino
 Created:	9/21/2017 8:11:22 AM
 Author:	cameronearle
*/

uint8_t command[4]; //Array to hold the command being built
uint8_t cursor = 0; //Cursor to track array position
uint8_t current; //Current character being received
Strip strips[16]; //Array of strips 
uint8_t stripCursor = 0;

void addStrip(uint8_t pin, uint16_t length, neoPixelType type = NEO_GRBW + NEO_KHZ800) {
	if (stripCursor < 16) {
		strips[stripCursor++].init(pin, length, type);
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
		if (cursor > 3) {
			cursor = 0;
		}
	}
}

// Runs the current command

void update() {
	uint8_t strip = command[0] - 1;
	uint8_t color = command[1];
	uint8_t action = command[2];
	uint8_t speed = command[3];

	if (action == SIGNAL && strips[strip].action == SIGNAL) {
		strips[strip].color = color;
		strips[strip].action = action;
		strips[strip].speed = speed;
	} else {
		if (action != SIGNAL) {
			strips[strip].colorFromSignal = color;
			strips[strip].actionFromSignal = action;
			strips[strip].speedFromSignal = speed;
		}
		if (strips[strip].action != SIGNAL) {
			strips[strip].color = color;
			strips[strip].action = action;
			strips[strip].speed = speed;
		}
	}


}


