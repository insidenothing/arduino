int X=13;

void setup() {
  // put your setup code here, to run once:
 pinMode(X, OUTPUT);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(X, HIGH);
  delay(1000);
  digitalWrite(X, LOW);
  delay(1000);
}
