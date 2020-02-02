#include <WS2812B.h>

#define NUM_LEDS 56
uint8_t leftside[NUM_LEDS/2];
uint8_t rightside[NUM_LEDS/2];

/*
 * Note. Library uses SPI1
 * Connect the WS2812B data input to MOSI on your board.
 * 
 */
WS2812B strip = WS2812B(NUM_LEDS);
// Note. Gamma is not really supported in the library, its only included as some functions used in this example require Gamma
uint8_t LEDGamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

void setup() 
{
  strip.begin();// Sets up the SPI
  strip.show();// Clears the strip, as by default the strip data is set to all LED's off.
  strip.setBrightness(80);
  for (uint8_t i=0; i<NUM_LEDS/2; i++) {
    leftside[i] = i;
    rightside[i] = NUM_LEDS-1-i;
  }
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop() 
{
  for (uint8_t rpts=0; rpts<4; rpts++) {
    fallingColorFade(3);
    delay(4000);
  }
  shiftInRainbowCycle(200);
  rainbowCycle(10);
  shifttoblack(200);
  for (uint8_t rpts=0; rpts<4; rpts++) {
    fullFade(1,24);
  }
  
  randomSolid(36,0, 28);
  for (uint8_t rpts=0; rpts<4; rpts++) {
    fullFade(0,24);
  }
  /*
  fallingDigit(400);
  fallingDigit(200);
  fallingDigit(100);
  //colorWipe(strip.Color(0, 255, 0), 20); // Green
  //colorWipe(strip.Color(255, 0, 0), 20); // Red
  //colorWipe(strip.Color(0, 0, 255), 20); // Blue
  rainbow(10);
  rainbowCycle(10);
  //theaterChase(strip.Color(255, 0, 0), 20); // Red
  //theaterChase(strip.Color(0, 255, 0), 20); // Green
  //theaterChase(strip.Color(0, 0, 255), 20); // Blue
  //theaterChaseRainbow(10);
  whiteOverRainbow(20,75,5);  
  pulseWhite(5); 
  delay(250);
  fullWhite();
  delay(2500);
  rainbowFade2White(3,3,1);
  */
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void randomSolid(uint8_t totaltransitions, uint8_t c, uint16_t waittime) {
  uint8_t pixelbuff[NUM_LEDS] = {};
  for (uint8_t j=0; j<NUM_LEDS; j++) {
    pixelbuff[j] = 0;
    strip.setPixelColor(j,0); 
  }
  strip.show();
  for (int8_t i=0; i<totaltransitions; i++) {
    uint8_t rando = random(NUM_LEDS);
    pixelbuff[rando] ^= 1;
    fadePixel(rando, c, pixelbuff[rando], waittime);
  }
  //Fade out for nice transition to next animation:
  for (uint8_t i=0; i<NUM_LEDS; i++) {
    if (pixelbuff[i]) fadePixel(i,c,0,waittime);
  }
}

void fadePixel(uint8_t pixelnum, uint8_t c, uint8_t onoff, uint16_t waittime) {
  if (pixelnum > NUM_LEDS) return;
  if (c>2) c=0;
  if (onoff>1) onoff=1;
  uint8_t colorbuff[3] = { 0,0,0 };
  for (uint8_t i=0; i<32; i++) {
    if (onoff) colorbuff[c] = (i*8)+7;
    else colorbuff[c] = 255-(i*8);
    int32_t thiscolor = strip.Color(LEDGamma[colorbuff[0]],LEDGamma[colorbuff[1]],LEDGamma[colorbuff[2]]);
    strip.setPixelColor(pixelnum,thiscolor);
    strip.show();
    delay(waittime);
  }
}

void fullFade(uint8_t c, uint16_t waittime) {
  int8_t direction = 1;
  uint8_t colorbuff[3] = { 0,0,0 };
  if (c>2) c=0;
  for (int16_t i=0; i>-1; i+=direction) {
    colorbuff[c] = i;
    uint32_t thiscolor = strip.Color(LEDGamma[colorbuff[0]],LEDGamma[colorbuff[1]],LEDGamma[colorbuff[2]]);
    for (uint8_t j=0; j<NUM_LEDS; j++) strip.setPixelColor(j,thiscolor);
    strip.show();
    if (i==190) { direction = -1; delay(2000); }
    delay(waittime);
  }
}

void fallingDigit(uint16_t waittime) {
  for (uint8_t i=0; i<(NUM_LEDS/2); i++) {
    for (uint8_t j=0; j<NUM_LEDS; j++) strip.setPixelColor(j,0);
    
    strip.setPixelColor(leftside[i],strip.Color(0,0,127));
    strip.setPixelColor(rightside[i],strip.Color(0,0,127));
    strip.show();
    delay(waittime);
  }
}

void fallingColorFade(uint16_t waittime) {
  int8_t lightlocation[6] = { 0, -1, -2, -3, -4, -5 };
  uint8_t fadearrayidx[6] = {16, 96, 176, 255, 175, 95 };

  //Move the pixels
  while (lightlocation[5] < NUM_LEDS/2) {
    //Turn all pixels off
    for (uint8_t j=0; j<NUM_LEDS; j++) strip.setPixelColor(j,0);
    //Fade each pixel through 80 steps
    for (uint8_t j=0; j<80; j++) {
      for (uint8_t i=0; i<6; i++) {
        //0-79 80-159 160-239 239-160 159-80 79-0  (Bias all of these by +16
        if ((lightlocation[i] >= 0) && (lightlocation[i] < NUM_LEDS/2)) {
          if (i<3) {
            strip.setPixelColor(leftside[lightlocation[i]],strip.Color(0,0,LEDGamma[fadearrayidx[i]+j]));
            strip.setPixelColor(rightside[lightlocation[i]],strip.Color(0,0,LEDGamma[fadearrayidx[i]+j])); 
          }
          else {
            strip.setPixelColor(leftside[lightlocation[i]],strip.Color(0,0,LEDGamma[fadearrayidx[i]-j]));
            strip.setPixelColor(rightside[lightlocation[i]],strip.Color(0,0,LEDGamma[fadearrayidx[i]-j])); 
          }
        }
      }
      strip.show();
      delay(waittime);
    }
    for (uint8_t i=0; i<6; i++) lightlocation[i] += 1;
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) 
  {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Make a nice transition into rainbowCycle
void shiftInRainbowCycle(uint8_t wait) 
{
  uint16_t i, j;
  uint32_t colorbuff[NUM_LEDS] = {};

  for(j=0; j<256*5; j++) 
  { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) 
    {
      colorbuff[i] = Wheel(((i * 256 / strip.numPixels()) + j) & 255);
    }
  }
  for (int8_t h=NUM_LEDS-1; h>-1; h--) {
    for(i=0; i<NUM_LEDS; i++) strip.setPixelColor(i,0);
    for (uint8_t k=h; k<NUM_LEDS; k++) {
      strip.setPixelColor(k,colorbuff[k-h]);
    }
    strip.show();
    delay(wait);
  }
}

//Shifts pixels off from last pixel to first
void shifttoblack(uint16_t waittime) {
  for (int8_t i=NUM_LEDS-1; i>-1; i--) {
    strip.setPixelColor(i,0);
    strip.show();
    delay(waittime);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) 
{
  uint16_t i, j;

  for(j=0; j<256*5; j++) 
  { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) 
{
  if(WheelPos < 85) 
  {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else 
  {
    if(WheelPos < 170) 
    {
     WheelPos -= 85;
     return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } 
    else 
    {
     WheelPos -= 170;
     return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void pulseWhite(uint8_t wait) {
  for(int j = 0; j < 256 ; j++){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0,0,0, LEDGamma[j] ) );
        }
        delay(wait);
        strip.show();
      }

  for(int j = 255; j >= 0 ; j--){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0,0,0, LEDGamma[j] ) );
        }
        delay(wait);
        strip.show();
      }
}


void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops) {
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;

  for(int k = 0 ; k < rainbowLoops ; k ++){
    
    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel

      for(int i=0; i< strip.numPixels(); i++) {

        wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);

        redVal = red(wheelVal) * float(fadeVal/fadeMax);
        greenVal = green(wheelVal) * float(fadeVal/fadeMax);
        blueVal = blue(wheelVal) * float(fadeVal/fadeMax);

        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );

      }

      //First loop, fade in!
      if(k == 0 && fadeVal < fadeMax-1) {
          fadeVal++;
      }

      //Last loop, fade out!
      else if(k == rainbowLoops - 1 && j > 255 - fadeMax ){
          fadeVal--;
      }

        strip.show();
        delay(wait);
    }
  
  }



  delay(500);


  for(int k = 0 ; k < whiteLoops ; k ++){

    for(int j = 0; j < 256 ; j++){

        for(uint16_t i=0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(0,0,0, LEDGamma[j] ) );
          }
          strip.show();
        }

        delay(2000);
    for(int j = 255; j >= 0 ; j--){

        for(uint16_t i=0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(0,0,0, LEDGamma[j] ) );
          }
          strip.show();
        }
  }

  delay(500);


}

void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength ) {
  
  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int head = whiteLength - 1;
  int tail = 0;

  int loops = 3;
  int loopNum = 0;

  static unsigned long lastTime = 0;


  while(true){
    for(int j=0; j<256; j++) {
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ){
          strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
        }
        else{
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        
      }

      if(millis() - lastTime > whiteSpeed) {
        head++;
        tail++;
        if(head == strip.numPixels()){
          loopNum++;
        }
        lastTime = millis();
      }

      if(loopNum == loops) return;
    
      head%=strip.numPixels();
      tail%=strip.numPixels();
        strip.show();
        delay(wait);
    }
  }
  
}
void fullWhite() {
  
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(255,255,255, 255 ) );
    }
      strip.show();
}

uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}
