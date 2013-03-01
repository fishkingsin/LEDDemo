#include "LPD8806.h"
#include "SPI.h"
#include <Easing.h>
#include "TimerOne.h"
// Simple test for 160 (5 meters) of LPD8806-based RGB LED strip

/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs = 384;

// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 2;
int clockPin = 3;
int  count = 0;
float r=0,g=0,b=0;
float targetRed=0,targetGreen=0,targetBlue=0;
float curR=0,curG=0,curB=0;
float time;
int mode = 0;
int fade = 60*2;//2 sec uder 60 fps
boolean isLoop = false;
// First parameter is the number of LEDs in the strand.  The LED strips
// are 32 LEDs per meter but you can extend or cut the strip.  Next two
// parameters are SPI data and clock pins:
LPD8806 strip = LPD8806(nLEDs);//, dataPin, clockPin);

// You can optionally use hardware SPI for faster writes, just leave out
// the data and clock pin parameters.  But this does limit use to very
// specific pins on the Arduino.  For "classic" Arduinos (Uno, Duemilanove,
// etc.), data = pin 11, clock = pin 13.  For Arduino Mega, data = pin 51,
// clock = pin 52.  For 32u4 Breakout Board+ and Teensy, data = pin B2,
// clock = pin B1.  For Leonardo, this can ONLY be done on the ICSP pins.
//LPD8806 strip = LPD8806(nLEDs);
int step_ = 16;

int v = 1;
int acc = 0;

int _delay = 4;
void setColor(int RED , int GREEN ,int BLUE);
void setup() {
  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
  Timer1.initialize();
  Timer1.attachInterrupt(callback, 1000000 / 60); // 60 frames/second
  //  delay_ = 0;
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }



  //  int  i=0;

  //  for(i=0; i<strip.numPixels(); i++) {
  //    strip.setPixelColor(i, strip.Color(255,255,255));
  //  }
  //  delay(500);
  //  for(i=0; i<strip.numPixels(); i++) {
  //    strip.setPixelColor(i, 0);
  //  }
  //  delay(500);
}

void loop() {

  if (Serial.available() > 0) {
    int thisChar = Serial.read();
    if(thisChar=='1')
    {
      setColor(255,0,0);
    }
    else if(thisChar=='2')
    {
      setColor(0,255,0);
    }
    else if(thisChar=='3')
    {

      setColor(0,0,255);
    }
    else if(thisChar=='4')
    {

      setColor(255,255,255);
    }
    else if(thisChar=='5')
    {
      setColor(255,0,255);
    }
    else if(thisChar=='6')
    {
      setColor(0,255,255);
    }
    else if(thisChar=='0')
    {  
      setColor(0,0,0);

    }
    else if(thisChar=='-')
    {
      v--;
    }
    else if(thisChar=='=')
    {
      v++;
    }
    else if(thisChar=='>')
    {
      step_++;
    }
    else if(thisChar=='<')
    {
      step_--;
    }
    else if(thisChar=='m')
    {
      mode++;
      mode%=4;
      int i=0;
      for(i=0; i<strip.numPixels(); i++) {

        strip.setPixelColor(i, 0);
      }
      count=0;
    }
    else if (thisChar=='L')
    {
      isLoop = false;
    }
    else if (thisChar=='l')
    {
      isLoop = true;
    }
    else if( thisChar =='d' && _delay>1)
    {
      _delay--;

    }
    else if( thisChar =='D')
    {
      _delay++;
    }
    else if (thisChar == 'f')
    {
      if(fade-100>0)fade-=100;
    }
    else if (thisChar == 'F')
    {
      fade+=100;
    }

  }
  //  Serial.println(r);


  //if(mode==0)
  //{
  //  count+=v;
  //
  //  count=int(count)%strip.numPixels();
  //
  //}
  //  delay(delay_);
  //      Serial.print("RED: ");
  //      Serial.println(r);
}
void setColor(int RED , int GREEN ,int BLUE)
{
  curR = r ;
  curG = g ;
  curB = b ;
  targetRed = RED-curR;
  targetGreen = GREEN-curG;
  targetBlue = BLUE-curB;
  time = 0;
  Serial.print("RED: ");
  Serial.print(r);
  Serial.print("\tcurR: ");
  Serial.print(curR);
  Serial.print("\ttargetRed: ");
  Serial.println(targetRed);
  
    Serial.print("GREEN: ");
  Serial.print(g);
  Serial.print("\tcurG: ");
  Serial.print(curG);
  Serial.print("\ttargetGreen: ");
  Serial.println(targetGreen);
  
    Serial.print("BLUE: ");
  Serial.print(b);
  Serial.print("\tcurB: ");
  Serial.print(curB);
  Serial.print("\ttargetBLUE: ");
  Serial.println(targetBlue);
}
void callback() {

  if(time<fade)
  {
    time++;

    r = Easing::easeInOutCubic(time, curR, targetRed, fade);
    g = Easing::easeInOutCubic(time, curG, targetGreen, fade);
    b = Easing::easeInOutCubic(time, curB, targetBlue, fade);

  }

  acc++;
  if(acc%_delay==0)
  {
    acc=0;

    count+=v;

    count=int(count)%strip.numPixels();
  }
  if(count==0 && isLoop)
  {
    if(count<0 && v<0)
    {
      count = strip.numPixels();
    }

  }  
  colorChase(r,g,b);
}

// Chase one dot down the full strip.  Good for testing purposes.
void colorChase(uint8_t red,uint8_t green,uint8_t blue) {
  int i;
  uint32_t c  = strip.Color(red,green,blue);

  for(i=0; i<strip.numPixels(); i++) {

    if(mode==0)
    {
      strip.setPixelColor(i, 0);
      if(int(count+i)%step_==0)
        strip.setPixelColor(i, c); // Erase pixel, but don't refresh!
    }
    else if(mode == 1)
    {
      if(v>0)
      {
        if(i<count)
          strip.setPixelColor(i, c); // Erase pixel, but don't refresh!
        else
          strip.setPixelColor(i, 0);
      }
      else
      {
        if(i>count)
          strip.setPixelColor(i, c); // Erase pixel, but don't refresh!
        else
          strip.setPixelColor(i, 0);
      }

    }
    else if(mode == 2)
    {
      if(count==i)
      {        
        strip.setPixelColor(i, c); // Erase pixel, but don't refresh!
      }
      else{
        strip.setPixelColor(i, 0);
      }

    }
  }

  strip.show(); // Refresh to turn off last pixel

}
















