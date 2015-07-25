#include <SPI.h>
#include <WiFi.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SD.h>
TinyGPSPlus gps;
SoftwareSerial ss(2, 3);
File myFile;
void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  SD.begin(4);
}
void loop() {
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < 1000);
  byte numSsid = WiFi.scanNetworks();
  if (ss.available() > 0) {
    for (int thisNet = 0; thisNet < numSsid; thisNet++) {
     char* wi = WiFi.SSID(thisNet);
     if (String(wi) != "") {
        Serial.print("Dynamic Log: ");
        Serial.println(String(gps.date.value()));
        myFile = SD.open(String(gps.date.value()), FILE_WRITE);
        if (myFile) {
          myFile.print(" DATE: ");
          myFile.print(gps.date.value());
          myFile.print(" TIME: ");
          myFile.print(gps.time.value());
          myFile.print(" SAT COUNT: ");
          myFile.print(gps.satellites.value());
          myFile.print(" LAT: ");
          myFile.print(gps.location.lat(), 6);
          myFile.print(" LNG: ");
          myFile.print(gps.location.lng(), 6);
          myFile.print(" MPH: ");
          myFile.print(gps.speed.mph(), 6);
          myFile.print(" FEET: ");
          myFile.print(gps.altitude.feet(), 6);
          myFile.print(" HEADING: ");
          myFile.print(gps.course.deg(), 6);
          myFile.print(" SSID: ");
          myFile.print(WiFi.SSID(thisNet));
          myFile.println("");
          myFile.close();
        } else {
          Serial.println("SD card error.");
        }
      }
    }
  }
}
