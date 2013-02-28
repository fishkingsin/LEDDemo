#include <Wire.h>

#define LED_PIN 13
byte x = 0;

void setup()
{
  Wire.begin(); // Start I2C Bus as Master
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

}
void loop()
{

  Wire.beginTransmission(9 Wire.beginTransmission); // transmit to device #9
  Wire.send(x);              // sends x 
  Wire.endTransmission();    // stop transmitting
  x++;
  if (x > 5) x=0;
  delay(450);
}

