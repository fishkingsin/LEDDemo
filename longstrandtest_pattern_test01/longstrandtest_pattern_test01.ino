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
int  count = 0;
float r=255,g=0,b=0;
float targetRed=255,targetGreen=0,targetBlue=0;
int mode = 0;
float fade = 0.1;
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
//int delay_ = 100;
int v = 1;
int acc = 0;
uint32_t pixels[384];
int color=0;
int _delay = 4;
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
  color = 0;
}

void loop() {

  if (Serial.available() > 0) {
    int thisChar = Serial.read();
    if(thisChar=='1')
    {
      //      r = 255;

      targetRed    =  255;
      targetGreen  =  0;
      targetBlue    =  0;
      color=1;
      
    }
    else if(thisChar=='2')
    {
      targetGreen = 255;
      targetRed = 0;
      targetBlue = 0;
      color=2;
    }
    else if(thisChar=='3')
    {

      targetRed = 0 ; 
      targetGreen = 0;
      targetBlue = 255;
    }
    else if(thisChar=='4')
    {

      targetRed = targetGreen = targetBlue  =255;

    }
    else if(thisChar=='5')
    {

      targetRed = 255;
      targetGreen = 255;
      targetBlue = 0;
    }
    else if(thisChar=='6')
    {

      targetRed = 0;
      targetGreen = 255;
      targetBlue = 255;
    }
    else if(thisChar=='0')
    {
      targetRed = 0;
      targetGreen = 0;
      targetBlue = 0;
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
      if(fade>0)fade-=0.01;
    }
    else if (thisChar == 'F')
    {
      if(fade<1)fade+=0.01;
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
}
void callback() {
  r -= float(r-targetRed)*fade;
  g -= float(g-targetGreen)*fade;
  b -= float(b-targetBlue)*fade;
  /*switch(color)
  {
  case 1:

      if(g>0)g--;
      if(b>0)b--;
      if(r<255)r++;


    break;
  case 2:
      if(r>0)r--;
      if(b>0)b--;
      if(g<255)g++;

    break;
  case 3:
      if(r>0)r--;
      if(g>0)g--;
      if(b<255)b++;
    break;
  case 4:
      if(r<255)r++;
      if(g<255)g++;
      if(b<255)b++;
    break;
  case 5:
      if(r>0)r--;
      if(g<255)g++;
      if(b<255)b++;
    break;
  case 6:
      if(g>0)g--;
      if(r<255)r++;
      if(b<255)b++;
    break;
  case 0:
    if(r>0)r--;
    if(g>0)g--;
    if(b>0)b--;
    break;
  }
   */ 
  acc++;
  if(acc%_delay==0)
  {
    acc=0;

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
  colorChase(r,g,b);
}

// Chase one dot down the full strip.  Good for testing purposes.
void colorChase(uint8_t red,uint8_t green,uint8_t blue) {
  int i;
  uint32_t c  = strip.Color(red,green,blue);
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













