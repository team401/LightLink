#pragma once

#define SLOW_BLINK_SPEED 500
#define FAST_BLINK_SPEED 100
#define NUM_BLINKS 9

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

// Action characters
#define SOLID 0x01
#define BLINK_SLOW 0x02
#define BLINK_FAST 0x03

// States
#define STATE_OFF 0
#define STATE_ON 1