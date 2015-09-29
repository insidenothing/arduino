#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>
#include <SeeedTouchScreen.h>

TouchScreen ts = TouchScreen(XP, YP, XM, YM);
String wanted;
int refresh;
//char disp;
void setup() {
   Serial.begin(9600);
   Tft.TFTinit();  // init TFT library 
   wanted="50";
}

void loop() {
  refresh = 0;
  int sensorValue = analogRead(A0);
  // put your main code here, to run repeatedly:   
  Point p = ts.getPoint();
  
  if (p.z > __PRESURE) {
     Serial.print("Raw X = "); Serial.print(p.x);
     Serial.print("\tRaw Y = "); Serial.print(p.y);
     Serial.print("\tPressure = "); Serial.println(p.z);
     if (p.y < 1000){
      Tft.fillScreen(0, 1000, 0, 1000, BLACK);
      Tft.drawString("TEMP UP",40,180,3,CYAN);  
     // wanted = wanted + 1; 
      
      refresh = 1;  
      delay(500);    
     }else{
      Tft.fillScreen(0, 1000, 0, 1000, BLACK);
      Tft.drawString("TEMP DOWN",30,180,3,CYAN); 
     //wanted = wanted - 1;    
       
      refresh = 1;     
      delay(500);  
     }
  }
  //char disp = char(wanted);
 // String c = "hi";
 // Tft.drawChar(wanted,190,220,4,GREEN);
  Tft.drawString("Target: ",30,220,3,BLUE);
 


  //delay(10);
  //Tft.fillScreen(5, 260, 50, 200, BLACK);
  if (refresh == 1){
    Tft.fillScreen(0, 1000, 0, 1000, BLACK);
  }
  Serial.println("Analog 8");
  Serial.println(sensorValue);
}
