/*
DC Voltmeter
An Arduino DVM based on voltage divider concept
T.K.Hareendran
*/


int ledRed = 9;
int ledBlue = 11;
int ledGreen = 10;

int solar1 = 0;
int local5v = 1;
int local33v = 2;
int solar2 = 3;
float vout1 = 0.0;
float vout2 = 0.0;
float vin1 = 0.0;
float vin2 = 0.0;
float R1 = 100000.0; // resistance of R1 (100K) -see text!
float R2 = 10000.0; // resistance of R2 (10K) - see text!

// tuned to 14v
int offset1 = 5.00;
int offset2 = 5.00;

void setup() {
  Serial.begin(9600);
  pinMode(solar1, INPUT);
  pinMode(solar2, INPUT);
  pinMode(local5v, INPUT);
  pinMode(local33v, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  Serial.println("DC VOLTMETER");
}
void loop() {
  // read the value at analog input
  int solarValue1 = analogRead(solar1);
  int solarValue2 = analogRead(solar2);
  int sensorValue5 = analogRead(local5v);
  int sensorValue33 = analogRead(local33v);
  vout1 = (solarValue1 * offset1) / 1024.0;
  vout2 = (solarValue2 * offset2) / 1024.0;
  float voltage5 = sensorValue5 * (5.0 / 1023.0);
  float voltage33 = sensorValue33 * (5.0 / 1023.0);
  vin1 = vout1 / (R2 / (R1 + R2));
  vin2 = vout2 / (R2 / (R1 + R2));
  if (vin1 < 0.09) {
    vin1 = 0.0; //statement to quash undesired reading !
  }
  if (vin2 < 0.09) {
    vin2 = 0.0; //statement to quash undesired reading !
  }
  
  if (voltage33 < 3.30) {
    digitalWrite(ledRed, HIGH);
    Serial.print("[local33:");
    Serial.print(voltage33);
    Serial.println("]");
    delay(1000);
    digitalWrite(ledRed, LOW);
  }
  
  if (voltage5 < 5.00) {
    digitalWrite(ledBlue, HIGH);
    Serial.print("[local5:");
    Serial.print(voltage5);
    Serial.println("]");
    delay(1000);
    digitalWrite(ledBlue, LOW);
  }
  
  if (vin1 < 13.00) {
    digitalWrite(ledGreen, HIGH);
    delay(100);
    digitalWrite(ledGreen, LOW);
  }
  Serial.print("[solar1:");
  Serial.print(vin1);
  Serial.println("]");
  delay(1000);
  
  if (vin2 < 13.00) {
    digitalWrite(ledGreen, HIGH);
    delay(50);
    digitalWrite(ledGreen, LOW);
    delay(50);
    digitalWrite(ledGreen, HIGH);
    delay(50);
    digitalWrite(ledGreen, LOW);
  }
  Serial.print("[solar2:");
  Serial.print(vin2);
  Serial.println("]");

  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledBlue, LOW);
  delay(1000);
}
