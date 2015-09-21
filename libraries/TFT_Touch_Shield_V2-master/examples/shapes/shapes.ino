// Draw Boxes - Demonstrate drawRectangle and fillRectangle

#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>
int one;
int two;

void setup()
{
    //TFT_BL_ON;                                          // turn on the background light
    Tft.TFTinit();                                      // init TFT library
}

void loop()
{
    //TFT_BL_ON;   
    one = random(0,200); 
    two = random(0,200);
    for(int r=0;r<50;r=r+2)                            //set r : 0--115
    {
        Tft.drawCircle(one,two,r,random(0xFFFF));       //draw circle, center:(119, 160), color: random
        Tft.drawCircle(two,one,r,random(0xFFFF));       //draw circle, center:(119, 160), color: random
    }
    //TFT_BL_OFF; 
    delay(1000);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
