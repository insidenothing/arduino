#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
const int redLED =  11;
const int blueLED =  10;
const int greenLED =  9;
int DHpin = 8;

char msg = 'test';

const int dehumidifyerRELAY =  5;
const int watervalveRELAY =  4;
const int sprinkletRELAY =  3;
const int growlightsRELAY =  2;


int MQ = A0;
int LP = A1;

int daylight = 0;
int night = 10000;
int lightrange = 0;

int TUNE1 = 1;


byte dat [5];
byte read_data () {
  byte data;
  for (int i = 0; i < 8; i ++) {
    if (digitalRead (DHpin) == LOW) {
      while (digitalRead (DHpin) == LOW); // wait for 50us
      delayMicroseconds (30); // determine the duration of the high level to determine the data is '0 'or '1'
      if (digitalRead (DHpin) == HIGH)
        data |= (1 << (7-i)); // high front and low in the post
      while (digitalRead (DHpin) == HIGH); // data '1 ', wait for the next one receiver
     }
  }
return data;
}
 
void start_test () {
  digitalWrite (DHpin, LOW); // bus down, send start signal
  delay (30); // delay greater than 18ms, so DHT11 start signal can be detected
 
  digitalWrite (DHpin, HIGH);
  delayMicroseconds (40); // Wait for DHT11 response
 
  pinMode (DHpin, INPUT);
  while (digitalRead (DHpin) == HIGH);
  delayMicroseconds (80); // DHT11 response, pulled the bus 80us
  if (digitalRead (DHpin) == LOW);
  delayMicroseconds (80); // DHT11 80us after the bus pulled to start sending data
 
  for (int i = 0; i < 4; i ++) // receive temperature and humidity data, the parity bit is not considered
    dat[i] = read_data ();
 
  pinMode (DHpin, OUTPUT);
  digitalWrite (DHpin, HIGH); // send data once after releasing the bus, wait for the host to open the next Start signal
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2); 
  lcd.setCursor(0,0);
  lcd.print("501 Spring Ave");
  lcd.setCursor(0,1);
  lcd.print("Home Garden");
  pinMode (DHpin, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
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
  // http://www.seeedstudio.com/wiki/Grove_-_Gas_Sensor(MQ2)
  // MQ-2 Gas Detection : Calibrated
  float sensor_volt;
  float RS_gas; // Get value of RS in a GAS
  float ratio; // Get ratio RS_GAS/RS_air
  int sensorValue = analogRead(MQ);
  sensor_volt = (float)sensorValue / 1024 * 5.0;
  RS_gas = (5.0 - sensor_volt) / sensor_volt; // omit *RL
  ratio = RS_gas / -0.10; // ratio = RS/R0
  // Serial.print("sensor_volt = ");
  //Serial.println(sensor_volt);
  //Serial.print("RS_ratio = ");
  Serial.println(RS_gas);
  //Serial.print("Rs/R0 = ");
  //Serial.println(ratio);
  //Serial.print("\n\n");
  // this will be adjusted when we know a "good" value, we can test against outdoors
  if (RS_gas < TUNE1) {
    digitalWrite(redLED, HIGH);
  }
  // http://playground.arduino.cc/Learning/PhotoResistor
  // Write the value of the photoresistor to the serial monitor.
  Serial.println(analogRead(LP));
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
   Serial.println(night+200);
  if (analogRead(LP) < night+200){
    digitalWrite(blueLED, HIGH);
  }else{
    digitalWrite(blueLED, LOW);
  }
  // display on LCD
  lcd.setCursor(0,0);
  lcd.print(dat [2], DEC); 
  lcd.setCursor(2,0);
  lcd.print('C');
  lcd.setCursor(4,0);
  lcd.print(dat [0], DEC);
  lcd.setCursor(6,0);
  lcd.print('%');
  lcd.setCursor(8,0);
  lcd.print(RS_gas);
  lcd.setCursor(0,1);
  lcd.print(analogRead(LP));
  delay(1000);
}

void MYalert (char* msg){
  lcd.setCursor(0,0);
  lcd.print("Special Alert");
  lcd.setCursor(0,1);
  lcd.print(msg);
  digitalWrite(redLED, HIGH);
  digitalWrite(blueLED, HIGH);
  digitalWrite(greenLED, HIGH);
  delay(500);
  lcd.clear();
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(greenLED, LOW);
}

