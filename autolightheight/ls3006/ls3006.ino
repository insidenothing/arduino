// works for LS-3006

#include <Servo.h>

Servo servo;  // create servo object to control a servo
int testing = 100;
// twelve servo objects can be created on most boards
const int pingPin = 7;
int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(19200);
  Serial.println("attached to pin 9");
  servo.attach(8);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  long duration, inches, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  if(inches > 40){
    // move down
    servoTurnLeft();
    delay(1000);
    servoStop();
  }
  if (inches < 10){
    servoTurnRight();
    delay(1000);
    servoStop();
  }
  //servoTurnLeft();
  //delay(10000);
  //servoStop();
  //servoTurnRight();
  //delay(10000);
 // servoStop();
 delay(100);
}

void servoStop(){
  servo.writeMicroseconds(1500);            //pulse width that tells servo to stop turning
  Serial.println("STOP");
  delay(1000);                              // need this delay before sending new command
}


void servoTurnLeft(){
  servo.writeMicroseconds(1200);            //full speed clockwise rotation
  Serial.println("DOWN");
  //delay(1000);
}

void servoTurnRight(){
  servo.writeMicroseconds(1800);            //full speed counter clockwise rotation
  Serial.println("UP");
  //delay(1000);
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
