#include "LPD8806.h"
#define USE_WIRE
#ifdef USE_WIRE
#include <Wire.h>
#endif
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
float rgb_colors[3]; 
float targetRed=0,targetGreen=0,targetBlue=0;
float curR=0,curG=0,curB=0;
float time;
int mode = 0;
int fade = 60*2;//2 sec uder 60 fps
boolean isLoop = false,bDebug = false;

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
#ifdef USE_WIRE
  Wire.begin(9);                // Start I2C Bus as a Slave (Device Number 9)
  Wire.onReceive(receiveEvent); // register event
#endif
  Serial.begin(57600); 
  while (!Serial) {

    ; // wait for serial port to connect. Needed for Leonardo only
  }
  // Start up the LED strip
  strip.begin();
  strip.show();
  Timer1.initialize();
  Timer1.attachInterrupt(callback, 1000000 / 60); // 60 frames/second



}

void loop() {

  if (Serial.available() > 0) {
    int thisChar = Serial.read();
    parseMessage(thisChar);

  }

}
void setColor(int RED , int GREEN ,int BLUE)
{
  
  curR = rgb_colors[0] ;
  curG = rgb_colors[1] ;
  curB = rgb_colors[2] ;
  targetRed = RED-curR;
  targetGreen = GREEN-curG;
  targetBlue = BLUE-curB;
  time = 0;
  if(bDebug)
  {
    Serial.print("RED: ");
    Serial.print(rgb_colors[0]);
    Serial.print("\tcurR: ");
    Serial.print(curR);
    Serial.print("\ttargetRed: ");
    Serial.println(targetRed);

    Serial.print("GREEN: ");
    Serial.print(rgb_colors[1]);
    Serial.print("\tcurG: ");
    Serial.print(curG);
    Serial.print("\ttargetGreen: ");
    Serial.println(targetGreen);

    Serial.print("BLUE: ");
    Serial.print(rgb_colors[2]);
    Serial.print("\tcurB: ");
    Serial.print(curB);
    Serial.print("\ttargetBLUE: ");
    Serial.println(targetBlue);
  }
//  getRGB(random(60),256,256,rgb_colors);
}
void callback() {

  if(time<fade)
  {
    time++;
//    if(targetRed>0)rgb_colors[0] = Easing::easeOutQuad(time, curR, targetRed, fade);
//    else rgb_colors[0] = Easing::easeInQuad(time, curR, targetRed, fade);
//    if(targetGreen>0)rgb_colors[1] = Easing::easeOutQuad(time, curG, targetGreen, fade);
//    else rgb_colors[1] = Easing::easeInQuad(time, curG, targetGreen, fade);
//    if(targetBlue>0)rgb_colors[2] = Easing::easeOutQuad(time, curB, targetBlue, fade);
//    else rgb_colors[2] = Easing::easeInQuad(time, curB, targetBlue, fade);

  }

  acc++;
  if(acc%_delay==0)
  {
    acc=0;
    //    

    if(count<strip.numPixels() && count >1 && isLoop) count+=v;


    count=int(count)%strip.numPixels();

  }
  if(count==0 && isLoop)
  {
    if(v<0)
    {
      count = strip.numPixels();
    }

  }  
  colorChase(rgb_colors[0],rgb_colors[1],rgb_colors[1]);
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
#ifdef USE_WIRE
void receiveEvent(int howMany) {
  int x = Wire.read();    // receive byte as an integer
  parseMessage(x);
}
#endif
void parseMessage(int thisChar)
{
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
    v=-1;
  }
  else if(thisChar=='=')
  {
    v=1;
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
  else if (thisChar == ' ')
  {
    bDebug = !bDebug;
  }
}



