#include <SPI.h>

// Use if you want to force the software SPI subsystem to be used for some reason (generally, you don't)
// #define FASTLED_FORCE_SOFTWARE_SPI
// Use if you want to force non-accelerated pin access (hint: you really don't, it breaks lots of things)
// #define FASTLED_FORCE_SOFTWARE_SPI
// #define FASTLED_FORCE_SOFTWARE_PINS
#include "FastLED.h"

///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to configure the leds,
// and then how to turn a single pixel white and then off, moving down the line of pixels.
//

// How many leds are in the strip?
#define NUM_LEDS 5

// Data pin that led data will be written out over
#define DATA_PIN 4

// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define CLOCK_PIN 5

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

// This function sets up the ledsand tells the controller about them
void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop() {

    // loop between ornage and white
      
      leds[0] = CRGB(255, 255, 255);
      leds[1] = CRGB(0, 100, 255);
      leds[2] = CRGB(0, 100, 255);
      leds[3] = CRGB(0, 100, 255);
      leds[4] = CRGB(0, 0, 255);
      FastLED.show();
      delay(100);
      leds[0] = CRGB(0, 100, 255);
      leds[1] = CRGB(255, 255, 255);
      leds[2] = CRGB(0, 100, 255);
      leds[3] = CRGB(0, 100, 255);
      //red
      leds[4] = CRGB(0, 0, 255);
      FastLED.show();
      delay(100);
      leds[0] = CRGB(0, 100, 255);
      leds[1] = CRGB(0, 100, 255);
      leds[2] = CRGB(255, 255, 255);
      leds[3] = CRGB(0, 100, 255);
      //white
      leds[4] = CRGB(255, 255, 255);
      FastLED.show();
      delay(100);
      leds[0] = CRGB(0, 100, 255);
      leds[1] = CRGB(0, 100, 255);
      leds[2] = CRGB(0, 100, 255);
      leds[3] = CRGB(255, 255, 255);
      leds[4] = CRGB(0, 0, 0);
      FastLED.show();
      delay(100);
      leds[0] = CRGB(0, 100, 255);
      leds[1] = CRGB(0, 100, 255);
      leds[2] = CRGB(0, 100, 255);
      leds[3] = CRGB(0, 100, 255);
      // white
      leds[4] = CRGB(255, 255, 255);
      FastLED.show();
      delay(100);
      // all orange
      leds[0] = CRGB(0, 100, 255);
      leds[1] = CRGB(0, 100, 255);
      leds[2] = CRGB(0, 100, 255);
      leds[3] = CRGB(0, 100, 255);
      // blue
      leds[4] = CRGB(255, 0, 0);
      FastLED.show();
      delay(500);
}
