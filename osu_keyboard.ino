#include "Keyboard.h"
// připojení potřebné knihovny
#include <Adafruit_NeoPixel.h>
#include <FastLED.h>

#define KEY_1_CHAR      'c'
#define KEY_2_CHAR      'z'
#define KEY_3_CHAR      'x'

#define KEY_1_PIN       2
#define KEY_2_PIN       3
#define KEY_3_PIN       1

#define KEY_1_PORT      1<<1
#define KEY_2_PORT      1<<0
#define KEY_3_PORT      1<<3
 
#define LED_DATA_PIN    1<<5
#define LED_CLOCK_PIN   1<<4

#define NR_OF_LEDS      4
#ifdef __AVR__
#include <avr/power.h> 
#endif


// nastavení propojovacího pinu
#define pinDIN 9
// nastavení počtu LED modulů
#define pocetLED 4

#define LED_MODE "RGB_SMOOTH"

#define PIN 9
#define NUM_LEDS 4
Adafruit_NeoPixel rgbWS = Adafruit_NeoPixel(pocetLED, pinDIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pocetLED, pinDIN, NEO_GRB + NEO_KHZ800);

void nastavRGB (byte r, byte g, byte b, int cislo) {
  uint32_t barva;
  barva = rgbWS.Color(r, g, b);
  rgbWS.setPixelColor(cislo - 1, barva);
  rgbWS.show();
}


uint16_t colorAngle = 0;

void setup() {

  
  DDRB |= LED_DATA_PIN | LED_CLOCK_PIN;
  DDRD &= ~KEY_1_PORT & ~KEY_2_PORT & ~KEY_3_PORT;

  Keyboard.begin();
  
  attachInterrupt(digitalPinToInterrupt(KEY_1_PIN), key1Interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(KEY_2_PIN), key2Interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(KEY_3_PIN), key3Interrupt, CHANGE);

  rgbWS.begin();
  loadAnim();
}

void loadAnim() {
  FadeInOut(0, 255, 0);
  FadeInOut(0, 255, 0);
  FadeInOut(0, 255, 0);
}

void animloop () {
  nastavRGB(0, 0, 0, 4);
  nastavRGB(255, 255, 255, 1);
  delay(1000);
  nastavRGB(0, 0, 0, 1);
  nastavRGB(255, 255, 255, 2);
  delay(1000);
  nastavRGB(0, 0, 0, 2);
  nastavRGB(255, 255, 255, 3);
  delay(1000);
  nastavRGB(0, 0, 0, 3);
  nastavRGB(255, 255, 255, 4);
  delay(1000);
}

void loop() {
  if(LED_MODE == "RGB_SMOOTH") {
    rainbowCycle(20);
  }
  if(LED_MODE == "TWINKLE") {
    Twinkle(random(0, 255), random(0, 255), random(0, 255), 10, 100, false);
  }
  if(LED_MODE == "WIPE") {
    colorWipe(random(0, 255),random(0, 255),random(0, 255), 100);
    colorWipe(0,0,0, 100);
    }
  if(LED_MODE == "FADE") {
    FadeInOut(random(0, 255), random(0, 255), random(0, 255));
  }
    
}

void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;
     
  for(int k = 0; k < 256; k=k+1) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
}

void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { 
    for(i=0; i< 5; i++) {
      c=Wheel(((i * 256 / pocetLED) + j) & 255);
      nastavRGB(*c, *(c+1), *(c+2), i);
    }
    showStrip();
    delay(SpeedDelay);
  }
}

void showStrip() {
  strip.show();
}



void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<5; i++) {
      nastavRGB(red, green, blue, i);
      showStrip();
      delay(SpeedDelay);
  }
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < 5; i++ ) {
    nastavRGB(red, green, blue, i);
  }
  showStrip();
}
byte * Wheel(byte WheelPos) {
  static byte c[3];
 
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}

void Twinkle(byte red, byte green, byte blue, int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
 
  for (int i=0; i<Count; i++) {
     nastavRGB(red,green,blue, random(5));
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) {
       setAll(0,0,0);
     }
   }
 
  delay(SpeedDelay);
}
