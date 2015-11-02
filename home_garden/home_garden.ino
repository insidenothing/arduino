#include <Wire.h>
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
// https://learn.adafruit.com/adafruit-data-logger-shield/using-the-real-time-clock
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include "pitches.h"
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//const int backlight =  13;

// PWM LED Digital Pins
const int redLED =  11;
const int blueLED =  10;
const int greenLED =  9;
// Digital Sensor Pins
const int DHpin = 8;
const int motion = 7;
const int speakerPin =  6;
// Digital Relay Pins
const int dehumidifyerRELAY =  5;
const int watervalveRELAY =  4;
const int sprinklerRELAY =  3;
const int growlightsRELAY =  2;

// Analog Sensor Pins
int MQ = A0; // Gas Probe
int LP = A1; // Light Probe
int SM = A2; // Soil Moisture Probe
int WL = A3; // Water Level Probe

// working on max/min for photosensor
int daylight = 0;
int night = 10000;
int lightrange = 0;

int thisHour = 14; // 2 PM - Daylight

RTC_DS1307 RTC;

// Bits of Data to Tune Alerts
int TUNE1 = 1;

// DH Code
byte dat [5];
byte read_data () {
  byte data;
  for (int i = 0; i < 8; i ++) {
    if (digitalRead (DHpin) == LOW) {
      while (digitalRead (DHpin) == LOW); // wait for 50us
      delayMicroseconds (30); // determine the duration of the high level to determine the data is '0 'or '1'
      if (digitalRead (DHpin) == HIGH)
        data |= (1 << (7 - i)); // high front and low in the post
      while (digitalRead (DHpin) == HIGH); // data '1 ', wait for the next one receiver
    }
  }
  return data;
}

// DH Code
void start_test () {
  //Serial.print("start_test() load");
  delay (1000);
  //Serial.print("start_test() begin");
  digitalWrite (DHpin, LOW); // bus down, send start signal
  delay (30); // delay greater than 18ms, so DHT11 start signal can be detected
  digitalWrite (DHpin, HIGH);
  delayMicroseconds (40); // Wait for DHT11 response
  pinMode (DHpin, INPUT);
  while (digitalRead (DHpin) == HIGH);
  delayMicroseconds (80); // DHT11 response, pulled the bus 80us
  if (digitalRead (DHpin) == LOW);
  delayMicroseconds (80); // DHT11 80us after the bus pulled to start sending data
  for (int i = 0; i < 4; i ++) { // receive temperature and humidity data, the parity bit is not considered
    dat[i] = read_data ();
  }
  pinMode (DHpin, OUTPUT);
  digitalWrite (DHpin, HIGH); // send data once after releasing the bus, wait for the host to open the next Start signal
  //Serial.print("start_test() end");
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // uncomment it & upload to set the time, date and start run the RTC!
    //RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("501 Spring Ave");
  lcd.setCursor(0, 1);
  lcd.print("Home Garden");
  pinMode (DHpin, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode (dehumidifyerRELAY, OUTPUT);
  pinMode (watervalveRELAY, OUTPUT);
  pinMode (sprinklerRELAY, OUTPUT);
  pinMode (growlightsRELAY, OUTPUT);
  digitalWrite(redLED, HIGH);
  delay(500);
  digitalWrite(redLED, LOW);
  delay(50);
  digitalWrite(blueLED, HIGH);
  delay(500);
  digitalWrite(blueLED, LOW);
  delay(50);
  digitalWrite(greenLED, HIGH);
  delay(500);
  digitalWrite(greenLED, LOW);
  delay(50);
  digitalWrite(redLED, HIGH);
  digitalWrite(blueLED, HIGH);
  digitalWrite(greenLED, HIGH);
  delay(500);
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(greenLED, LOW);
  delay(1000);
  lcd.clear();
}

void loop() {
  start_test ();
  DateTime now = RTC.now();
  //Serial.print(now.year(), DEC);
  //Serial.print('/');
  //Serial.print(now.month(), DEC);
  //Serial.print('/');
  //Serial.print(now.day(), DEC);
  //Serial.print(' ');
  //Serial.print(now.hour(), DEC);
  //Serial.print(':');
  //Serial.print(now.minute(), DEC);
  //Serial.print(':');
  //Serial.print(now.second(), DEC);
  //Serial.println();
  if (now.hour() > 8 && now.hour() < 17) {
    // Lights on from 7AM to 6PM
    digitalWrite(growlightsRELAY, HIGH);
  } else {
    digitalWrite(growlightsRELAY, LOW);
  }
  // http://www.seeedstudio.com/wiki/Grove_-_Gas_Sensor(MQ2)
  // MQ-2 Gas Detection : Calibrated
  float sensor_volt;
  float RS_gas; // Get value of RS in a GAS
  float ratio; // Get ratio RS_GAS/RS_air
  int sensorValue = analogRead(MQ);
  sensor_volt = (float)sensorValue / 1024 * 5.0;
  RS_gas = (5.0 - sensor_volt) / sensor_volt; // omit *RL
  ratio = RS_gas / -0.10; // ratio = RS/R0
  //Serial.print("sensor_volt = ");
  //Serial.println(sensor_volt);
  //Serial.print("RS_ratio = ");
  //Serial.println(RS_gas);
  //Serial.print("Rs/R0 = ");
  //Serial.println(ratio);
  //Serial.print("\n\n");
  // this will be adjusted when we know a "good" value, we can test against outdoors
  if (RS_gas < TUNE1) {
    digitalWrite(redLED, HIGH);
  }
  // http://playground.arduino.cc/Learning/PhotoResistor
  // Write the value of the photoresistor to the serial monitor.
  //Serial.println(analogRead(LP));
  if (analogRead(LP) > daylight) {
    char* test = "MAX LIGHT";
    MYalert (test);
    daylight = analogRead(LP);
  }
  if (analogRead(LP) < night) {
    char* test = "LOW LIGHT";
    MYalert (test);
    night = analogRead(LP);
  }
  //Serial.println(night + 200);
  if (analogRead(LP) < night + 200) {
    digitalWrite(blueLED, HIGH);
  } else {
    digitalWrite(blueLED, LOW);
  }
  // display on LCD
  lcd.setCursor(0, 0);
  lcd.print(dat [2], DEC);
  lcd.setCursor(2, 0);
  lcd.print('C');
  // 32% humidity at time of dev
  if (dat[0] > 40) {
    digitalWrite(dehumidifyerRELAY, HIGH);
  } else {
    digitalWrite(dehumidifyerRELAY, LOW);
  }
  lcd.setCursor(4, 0);
  lcd.print(dat [0], DEC);
  lcd.setCursor(6, 0);
  lcd.print('%');
  lcd.setCursor(8, 0);
  lcd.print(RS_gas);
  lcd.setCursor(13, 0);
  lcd.print(analogRead(LP));
  lcd.setCursor(0, 1);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  // outside plants measure ~300
  // dry indoor soil (not livable) 50-100
  if (analogRead(SM) < 200) {
    // water plants to half sensor level
    digitalWrite(sprinklerRELAY, HIGH);
  } else {
    digitalWrite(sprinklerRELAY, LOW);
  }
  lcd.setCursor(9, 1);
  lcd.print(analogRead(SM));
  lcd.setCursor(12, 1);
  lcd.print("/");
  // Approximate values with a glass of water
  // 680+ Sound Buzzer - About to Contact Electronics
  // 640 Submerged
  // 600 1/2 Submerged
  // 500 Tip in Water
  // -400 Out of Water
  if (analogRead(WL) < 100) {
    digitalWrite(watervalveRELAY, HIGH);
    noTone(speakerPin);
  } else if (analogRead(WL) > 680) {
    digitalWrite(watervalveRELAY, LOW);
    tone(speakerPin, 2000, 1000);
  } else if (analogRead(WL) < 500) {
    // run refill water pump to half sensor
    digitalWrite(watervalveRELAY, HIGH);
    tone(speakerPin, 100, 100);
  } else if (analogRead(WL) < 250) {
    digitalWrite(watervalveRELAY, HIGH);
    noTone(speakerPin);
  } else {
    noTone(speakerPin);
    digitalWrite(watervalveRELAY, LOW);
  }
  lcd.setCursor(13, 1);
  lcd.print(analogRead(WL));
  delay(1000);
}

void MYalert (char* msg) {
  //lcd.setCursor(7, 1);
  //lcd.print(msg);
  if (msg == "MAX LIGHT") {
    digitalWrite(blueLED, HIGH);
    delay(500);
    digitalWrite(blueLED, LOW);
  }
  if (msg == "LOW LIGHT") {
    digitalWrite(greenLED, HIGH);
    delay(500);
    digitalWrite(greenLED, LOW);
  }
  //digitalWrite(redLED, HIGH);
  //digitalWrite(blueLED, HIGH);

  //lcd.clear();
  //digitalWrite(redLED, LOW);
  //digitalWrite(blueLED, LOW);

}

