#include "Keyboard.h"
#include <Adafruit_NeoPixel.h>
#include <FastLED.h>
#ifdef __AVR__
#include <avr/power.h> 
#endif

/*
   _____      _   _   _                 
  / ____|    | | | | (_)                
 | (___   ___| |_| |_ _ _ __   __ _ ___ 
  \___ \ / _ \ __| __| | '_ \ / _` / __|
  ____) |  __/ |_| |_| | | | | (_| \__ \
 |_____/ \___|\__|\__|_|_| |_|\__, |___/
                               __/ |    
                              |___/     
*/

//LED Animation mode. You can choose from "RGB_SMOOTH", "TWINKLE", "WIPE", "RACTIVE" and "FADE"
#define LED_MODE "REACTIVE"
//Pin of your LEDS/LED Stripe
#define LED_PIN 9
//Number of how many LEDS do you have
#define LED_COUNT 4

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




















/*
   _____          _      
  / ____|        | |     
 | |     ___   __| | ___ 
 | |    / _ \ / _` |/ _ \
 | |___| (_) | (_| |  __/
  \_____\___/ \__,_|\___|
  
*/

byte cr;
byte cg;
byte cb;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setRGB (byte r, byte g, byte b, int ledid) {
  uint32_t color;
  color = strip.Color(r, g, b);
  strip.setPixelColor(ledid - 1, color);
  strip.show();
  cr = r;
  cg = g;
  cb = b;
}

int inputTimer;
uint16_t colorAngle = 0;



/*
showStrip()
@description: Function to update the LEDS/LED Strip 
*/
void showStrip() {
  strip.show();
}



/*
beginStrip()
@description: Function to initiate the LEDS/LED Strip on setup()
*/
void beginStrip() {
  strip.begin();
}


/*
setAll(byte red, byte green, byte blue)
@description: Function to set the color of all leds using RGB value
@param r: the "r" value of RGB color
@param g: the "g" value of RGB color
@param b: the "b" value of RGB color
*/
void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < 5; i++ ) {
    setRGB(red, green, blue, i);
  }
  showStrip();
}

void setup() {

  DDRB |= LED_DATA_PIN | LED_CLOCK_PIN;
  DDRD &= ~KEY_1_PORT & ~KEY_2_PORT & ~KEY_3_PORT;

  Keyboard.begin();
  
  attachInterrupt(digitalPinToInterrupt(KEY_1_PIN), key1Interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(KEY_2_PIN), key2Interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(KEY_3_PIN), key3Interrupt, CHANGE);

  strip.begin();
  loadAnim();
}


void loop() {
  if(LED_MODE == "RGB_SMOOTH") {
    rainbowCycle(20);
  }
  if(LED_MODE == "TWINKLE") {
    Twinkle(random(0, 255), random(0, 255), random(0, 255), 100, 10, false);
  }
  if(LED_MODE == "WIPE") {
    colorWipe(random(0, 255),random(0, 255),random(0, 255), 200);
    colorWipe(0,0,0, 200);
    }
  if(LED_MODE == "FADE") {
    FadeInOut(random(0, 255), random(0, 255), random(0, 255));
  }
  if(LED_MODE == "REACTIVE") {
    if(inputTimer == 0) {
      //setAll(0,0,0);
      FadeOut(cr,cg,cb);
      inputTimer = -1;
    } else {
      inputTimer--;
      delay(1);
    }
  }

}




/*
                 _                 _   _                 
     /\         (_)               | | (_)                
    /  \   _ __  _ _ __ ___   __ _| |_ _  ___  _ __  ___ 
   / /\ \ | '_ \| | '_ ` _ \ / _` | __| |/ _ \| '_ \/ __|
  / ____ \| | | | | | | | | | (_| | |_| | (_) | | | \__ \
 /_/    \_\_| |_|_|_| |_| |_|\__,_|\__|_|\___/|_| |_|___/

                                                         
*/


void loadAnim() {
  FadeInOut(0, 255, 0);
  FadeInOut(0, 255, 0);
  FadeInOut(0, 255, 0);
}


/*
FadeInOut(byte red, byte green, byte blue)
@description: Slowly fades between colors
@param r: the "r" value of RGB color
@param g: the "g" value of RGB color
@param b: the "b" value of RGB color
*/
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


void FadeOut(byte red, byte green, byte blue) {
  float r, g, b;
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
}



/*
rainbowCycle(int SpeedDelay)
@description: Cycles between colors in wheel pattern between all leds
@param SpeedDelay: How fast should the colors change (In milliseconds)
*/
void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { 
    for(i=0; i< 5; i++) {
      c=Wheel(((i * 256 / 5) + j) & 255);
      setRGB(*c, *(c+1), *(c+2), i);
    }
    showStrip();
    delay(SpeedDelay);
  }
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








/*
colorWipe(byte red, byte green, byte blue, int SpeedDelay)
@description: Switches the colors of all leds in "chain reaction"
@param r: the "r" value of RGB color
@param g: the "g" value of RGB color
@param b: the "b" value of RGB color
@param SpeedDelay: How fast should the chain reaction be (In milliseconds)
*/
void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<5; i++) {
      setRGB(red, green, blue, i);
      showStrip();
      delay(SpeedDelay);
  }
}




void Twinkle(byte red, byte green, byte blue, int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
 
  for (int i=0; i<Count; i++) {
     setRGB(red,green,blue, random(5));
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) {
       setAll(0,0,0);
     }
   }
 
  delay(SpeedDelay);
}
