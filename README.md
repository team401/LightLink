#LightLink

LightLink provides code for driving Adafruit’s Neopixel addressable LED strips and rings in a variety of patterns useful for FRC. It employs a simple I2C based command system for transferring commands from the RIO to the Arduino. The Arduino code is fully state machine based, so no calls to “delay" when rendering effects. This means that transitions between effects are instant. In addition, a special mode “SIGNAL" is provided, which quickly pulses the strip in the commanded color 6 times. Signals interrupt any previously running effects, and cannot be interrupted by anything but other signals. When a signal is finished, the strip returns to its previous effect. LightLink supports up to 16 light strips simultaneously, and is capable of rendering each strip separately.

A web-app for configuring a LightLink ino file is available at [https://team401.github.io/LightLinkBuilder/](https://team401.github.io/LightLinkBuilder/).
