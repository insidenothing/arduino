#include <SPI.h>
#include <SoftwareSerial.h>
#include "FastLED.h"
const int sio = 2;			// ColorPAL connected to pin 2
const int unused = 255; 		// Non-existant pin # for SoftwareSerial
const int sioBaud = 4800;
const int waitDelay = 200;

// Received RGB values from ColorPAL
int red;
int grn;
int blu;

#define NUM_LEDS 1
#define DATA_PIN 4
#define CLOCK_PIN 5
CRGB leds[NUM_LEDS];
// Set up two software serials on the same pin.
SoftwareSerial serin(sio, unused);
SoftwareSerial serout(unused, sio);

void setup() {
  Serial.begin(9600);
  delay(2000);
  FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  reset();				  // Send reset to ColorPal
  serout.begin(sioBaud);
  pinMode(sio, OUTPUT);
  serout.print("= (00 $ m) !"); // Loop print values, see ColorPAL documentation
  serout.end();			  // Discontinue serial port for transmitting

  serin.begin(sioBaud);	        // Set up serial port for receiving
  pinMode(sio, INPUT);
}

void loop() {
  readData();
}

// Reset ColorPAL; see ColorPAL documentation for sequence
void reset() {
  delay(200);
  pinMode(sio, OUTPUT);
  digitalWrite(sio, LOW);
  pinMode(sio, INPUT);
  while (digitalRead(sio) != HIGH);
  pinMode(sio, OUTPUT);
  digitalWrite(sio, LOW);
  delay(80);
  pinMode(sio, INPUT);
  delay(waitDelay);
}

void readData() {
  char buffer[32];

  if (serin.available() > 0) {
    // Wait for a $ character, then read three 3 digit hex numbers
    buffer[0] = serin.read();
    if (buffer[0] == '$') {
      for (int i = 0; i < 9; i++) {
        while (serin.available() == 0);     // Wait for next input character
        buffer[i] = serin.read();
        if (buffer[i] == '$')               // Return early if $ character encountered
          return;
      }
      parseAndPrint(buffer);
      delay(10);
    }
  }
}

// Parse the hex data into integers
void parseAndPrint(char * data) {
  sscanf (data, "%3x%3x%3x", &red, &grn, &blu);
  char buffer[32];
  sprintf(buffer, "R%4.4d G%4.4d B%4.4d", red, grn, blu);
  if(blu > 10 || grn > 10 || red > 10){
    leds[0] = CRGB(blu, grn, red);
  }else{
    leds[0] = CRGB(0, 0, 0);
  }
  FastLED.show();
  //Serial.println('RGB');
  //Serial.println(red);
  //Serial.println(grn);
  //Serial.println(blu);
  delay(10);
  
}
