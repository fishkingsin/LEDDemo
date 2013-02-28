#include "LPD8806.h"
#include "SPI.h"
#include "TimerOne.h"
// Simple test for 160 (5 meters) of LPD8806-based RGB LED strip

/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs = 384;

// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 2;
int clockPin = 3;
float count = 0;
int r=255,g=0,b=0;
int mode = 0;
int fade = 10;
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
int delay_ = 100;
int v = 1;
uint32_t pixels[384];
int color=0;
void setup() {
  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
  Timer1.initialize();
  Timer1.attachInterrupt(callback, 1000000 / 60); // 60 frames/second
  delay_ = 0;
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  color = 1;
}

void loop() {

  if (Serial.available() > 0) {
    int thisChar = Serial.read();
    if(thisChar=='1')
    {
      //      r = 255;

      //      r =255;
      //      g = 0;
      //      b = 0;
      color=1;
    }
    else if(thisChar=='2')
    {
      //      g =255;
      //      r = 0;
      //      b = 0;
      color=2;
    }
    else if(thisChar=='3')
    {

      color=3;
    }
    else if(thisChar=='4')
    {

      color=4;

    }
    else if(thisChar=='5')
    {

      color=5;
    }
    else if(thisChar=='6')
    {

      color = 6;
    }
    else if(thisChar=='0')
    {

      color = 0;
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
      for(int i=0; i<strip.numPixels(); i++) {

        strip.setPixelColor(i, 0);
      }
      count=0;
    }
    else if( thisChar =='d' && delay_>0)
    {
      delay_-=10;
    }
    else if( thisChar =='D')
    {
      delay_+=10;
    }

  }

  switch(color)
  {
  case 1:
    if(g>0)g--;
    else if(b>0)b--;
    else if(r<255)r++;
    delay(fade);
    break;
  case 2:
    {

      if(r>0)r--;
      else if(b>0)b--;
      else if(g<255)g++;
      delay(fade);    
    }
    break;
  case 3:
    {

      if(r>0)r--;
      else if(g>0)g--;
      else if(b<255)b++;
      delay(fade);    
    }
    break;
  case 4:
    {

      if(r<255)r++;
      if(g<255)g++;
      if(b<255)b++;
      delay(fade);    
    }
    break;
  case 5:
    {

      if(r>0)r--;
      else if(g<255)g++;
      else if(b<255)b++;
      delay(fade);    
    }
    break;
  case 6:
    {
      if(g>0)g--;
      else if(r<255)r++;
      else if(b<255)b++;
      delay(fade);    
    }
    break;
  case 0:
    {

      if(r>0)r--;
      else if(g>0)g--;
      else if(b>0)b--;
      delay(fade);    
    }
    break;
  }
//if(mode==0)
//{
//  count+=v;
//
//  count=int(count)%strip.numPixels();
//
//}
  //  delay(delay_);
}
void callback() {
//if(mode!=0)
{
  count+=v;

  count=int(count)%strip.numPixels();
}
  if(count==0)
  {
    if(count<0 && v<0)
    {
      count = strip.numPixels();
    }

  }  
  colorChase(r,g,b, delay_);
}

// Chase one dot down the full strip.  Good for testing purposes.
void colorChase(uint8_t r,uint8_t g,uint8_t b, uint8_t wait) {
  int i;
  uint32_t c  = strip.Color(r,g,b);
  // Start by turning all pixels off:
  //  for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

  // Then display one pixel at a time:
  for(i=0; i<strip.numPixels(); i++) {
    //    strip.setPixelColor(i, pixels[(i+count)%strip.numPixels()]);
    //    strip.setPixelColor(i, c); // Set new pixel 'on'
    //    strip.show();              // Refresh LED states

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
      }
      else
      {
        if(i>count)
          strip.setPixelColor(i, c); // Erase pixel, but don't refresh!
      }

    }
    else if(mode == 2)
    {
      if(abs(count-i)<1)
      {        
        strip.setPixelColor(i, c); // Erase pixel, but don't refresh!
      }
      else{
        strip.setPixelColor(i, 0);
      }

    }
  }

  strip.show(); // Refresh to turn off last pixel
  delay(wait);
}











