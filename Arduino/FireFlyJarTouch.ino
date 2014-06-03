#include <Adafruit_NeoPixel.h> 
#include <CapacitiveSensor.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)

// USER VARS
int mode = 1;
int numMode = 12;

// INIT STUFF

int idex = 0;
int j = 0;
unsigned long mark;
int direct = 1;
int counter = 0;
int TOP_INDEX = 3;
int BOTTOM_INDEX = 0;
int EVENODD = 6 % 2;
int direction = 1;

// Start Sensor
CapacitiveSensor cs1 = CapacitiveSensor(0, 1); // 10M resistor between pins 0 & 1

// Start Strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(5, 4, NEO_GRB + NEO_KHZ800);

void setup() {
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void loop() {
    long total1 = cs1.capacitiveSensor(30);    
    if (total1 > 300 && (millis() - mark) > 800) {
        j = 0;
        mark = millis();
        mode++;
    }
    // if mode greater than numMode reset
    if (mode > numMode) { mode = 1; }
    // main function
    doSomething(mode);


}


void doSomething(int var) {
    if (j > 254) {
        direction = 0;
    }
    if (j < 1) {
        direction = 1;
    }
    switch (var) {
    case 1:
        rainbow(20);
        break;
    case 2:
        colorWipe(strip.Color(random(255), random(255), random(255)), 140); 
        break;
    case 3:
        twinkle(3, random(0, 4));
        colorFast(strip.Color(180, 0, 180), 0);
        break;
    case 4:
        rainbowCycle(10);
        break;
    case 5:
        sineFirefly(70);
        counter++;
        break;
    case 6:
        colorFast(strip.Color(255, 255, 255), 0); // white
        break;
    case 7:
        colorFast(strip.Color(255, 147, 41), 0); // white
        break;
    case 8:
        rainbow(1);
        break;
    case 9:
        flame();
        break;
    case 10:
        chaseLights(100);
        break;     
    case 11:
        colorFirefly(100);
        counter++;
        break;  
    case 12:
        totallyRandom();
    default:
        rainbow(2);
        // if nothing else matches, do the default
        // default is optional
    }
    if (direction == 1) {
        j++;
    } else {
        j--;
    }
}

// PATTERN FUNCTIONS
// Created, or adapted from Adafruit and Funkboxing!

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

// fast version 

void colorFast(uint32_t c, uint8_t wait) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
    }
     strip.show();
    delay(wait);
}


void rainbow(uint8_t wait) {
    uint16_t i;

    //for(j=0; j<256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
    // }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
    uint16_t i;

    //  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
    // }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
    if (WheelPos < 85) {
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    } else if (WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else {
        WheelPos -= 170;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}

void chaseLights(int wait) { //-POLICE LIGHTS (TWO COLOR SINGLE LED)
    idex++;
    if (idex >= strip.numPixels()) {
        idex = 0;
    }
    int idexR = idex;
    int idexB = antipodal_index(idexR);
    for (int i = 0; i < strip.numPixels(); i++) {
        if (i == idexR) {
            strip.setPixelColor(i, 00, 255, 0);
        } else if (i == idexB) {
            strip.setPixelColor(i, 0, 0, 255);
        } else {
            strip.setPixelColor(i, 0, 0, 0);
        }
    }
    strip.show();
    delay(wait);
}

void twinkle(int q, int num) {
    for (int x = 1; x < num; x++) {
        strip.setPixelColor(random(strip.numPixels()), 255, 255, 255);
    }
    strip.show();
    delay(50);
}


void flame() {
    int colors[3];
    int idelay = random(30, 80);

    for (int i = 0; i < strip.numPixels(); i++) {
        HSVtoRGB(random(11,34), 255, random(200,255), colors);

        strip.setPixelColor(i, colors[0], colors[1], colors[2]);
    }    
        strip.show();
        delay(idelay);
   
}

int lastPix=0; int myPix=0;
void sineFirefly(int wait) {
        if(myPix != lastPix) {
          if(counter<16) {
            float colorV = sin((6.28/30)*(float)(counter)) *255;
            strip.setPixelColor(myPix,colorV,colorV,colorV);

           strip.show();
           delay(wait);
          } else {
            lastPix=myPix;
            counter=0;
            colorFast(0,0);
          }
        } else {
          myPix=random(0,strip.numPixels());
        }
	
}

void totallyRandom() {
        int colors[3];
        for (int i = 0; i < strip.numPixels(); i++) {
           HSVtoRGB(random(0,359), 255,255, colors);
            strip.setPixelColor(i, colors[0], colors[1], colors[2]);

        }
        strip.show();
        delay(50);
}

void colorFirefly(int wait) {
        if(myPix != lastPix) {
          if(counter<16) {
            int colors[3];
            float colorV = sin((6.28/30)*(float)(counter)) *255;
            HSVtoRGB((359/16)*counter, 255, colorV, colors);
            strip.setPixelColor(myPix, colors[0], colors[1], colors[2]);

           strip.show();
           delay(wait);
          } else {
            lastPix=myPix;
            counter=0;
            colorFast(0,0);
          }
        } else {
          myPix=random(0,strip.numPixels());
        }
	
}



// HELPER FUNCTIONS, DERIVED FROM FUNKBOXING FAST_SPI!
// YAYAYAYAYAYYAYA

// antipodal point
int antipodal_index(int i) {
    //int N2 = int(NUM_LEDS/2);
    int iN = i + strip.numPixels() / 2;
    if (i >= (strip.numPixels() / 2)) {
        iN = (i + (strip.numPixels() / 2)) % strip.numPixels();
    }
    return iN;
}

// horizontal index
int horizontal_index(int i) {
    //-ONLY WORKS WITH INDEX < TOPINDEX
    if (i == BOTTOM_INDEX) {
        return BOTTOM_INDEX;
    }
    if (i == TOP_INDEX && EVENODD == 1) {
        return TOP_INDEX + 1;
    }
    if (i == TOP_INDEX && EVENODD == 0) {
        return TOP_INDEX;
    }
    return 6 - i;
}

// HSV to RGB colors
void HSVtoRGB(int hue, int sat, int val, int * colors) {
    int r, g, b, base;

    // hue: 0-359, sat: 0-255, val (lightness): 0-255


    if (sat == 0) { // Achromatic color (gray).
        colors[0] = val;
        colors[1] = val;
        colors[2] = val;
    } else {
        base = ((255 - sat) * val) >> 8;
        switch (hue / 60) {
        case 0:
            colors[0] = val;
            colors[1] = (((val - base) * hue) / 60) + base;
            colors[2] = base;
            break;
        case 1:
            colors[0] = (((val - base) * (60 - (hue % 60))) / 60) + base;
            colors[1] = val;
            colors[2] = base;
            break;
        case 2:
            colors[0] = base;
            colors[1] = val;
            colors[2] = (((val - base) * (hue % 60)) / 60) + base;
            break;
        case 3:
            colors[0] = base;
            colors[1] = (((val - base) * (60 - (hue % 60))) / 60) + base;
            colors[2] = val;
            break;
        case 4:
            colors[0] = (((val - base) * (hue % 60)) / 60) + base;
            colors[1] = base;
            colors[2] = val;
            break;
        case 5:
            colors[0] = val;
            colors[1] = base;
            colors[2] = (((val - base) * (60 - (hue % 60))) / 60) + base;
            break;
        }

    }
}

