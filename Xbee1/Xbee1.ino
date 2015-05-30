
#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
const int ledPin = 13; // the pin that the LED is attached to
int incomingByte;      // a variable to read incoming serial data into

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines and turn on backlight
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on  
}

void loop() {
  // see if there's incoming serial data:
  
  while (Serial.available() > 0) {
        // display each character to the LCD
        lcd.write(Serial.read());
  }
  
  
    // if it's a capital H (ASCII 72), turn on the LED:
    //if (incomingByte == 'H') {
      //digitalWrite(ledPin, HIGH);
    //}
    
    // if it's an L (ASCII 76) turn off the LED:
    //if (incomingByte == 'L') {
     // digitalWrite(ledPin, LOW);
    //}
 
    
    
  
}

