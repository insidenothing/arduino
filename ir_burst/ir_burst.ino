/* Example program for from IRLib – an Arduino library for infrared encoding and decoding
 * Version 1.3   January 2014
 * Copyright 2014 by Chris Young http://cyborg5.com
 * Based on original example sketch for IRremote library 
 * Version 0.11 September, 2009
 * Copyright 2009 Ken Shirriff
 * http://www.righto.com/
 */
#include <IRLib.h>

IRsend My_Sender;

void setup()
{
  Serial.begin(9600);
   pinMode(10, OUTPUT);
}

void loop() {
   digitalWrite(10, HIGH);
   My_Sender.send(SONY,0xa8bca, 20);
   delay(1000);
   //send code every 2 seconds
   //Sony DVD power A8BCA
   digitalWrite(10, LOW);
   delay(1000);
}

