// Draw Boxes - Demonstrate drawRectangle and fillRectangle

#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>
int one;
int two;
int oneb;
int twob;
int base;

void setup()
{
    //TFT_BL_ON;                                          // turn on the background light
    Tft.TFTinit();                                      // init TFT library
}

void loop()
{
    //TFT_BL_ON;   
    one = random(0,100); 
    two = random(0,100);
    oneb = random(0,100);
    twob = random(0,100);
    base = random(0,100);
    for(int r=0;r<base;r=r+2)                            //set r : 0--115
    {
        Tft.drawCircle(one,two,r,random(0xFFFF));       //draw circle, center:(119, 160), color: random
        Tft.drawCircle(oneb,twob,r,random(0xFFFF));       //draw circle, center:(119, 160), color: random
    }
    //TFT_BL_OFF; 
    delay(1000);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
