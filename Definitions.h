#pragma once

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