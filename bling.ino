#include <avr/pgmspace.h>
#include "Adafruit_WS2801.h"
#include <limits.h>

#if defined(__AVR_ATtiny85__)
#define BPIN ( (uint8_t)3 ) // Bass pin - ATTiny85 pin 2
#define TPIN ( (uint8_t)4 ) // Treble pin - ATTiny85 pin 3
#define CPIN ( (uint8_t)2 ) // LED clock pin - ATTiny85 pin 7
#define DPIN ( (uint8_t)1 ) // Data pin - ATTiny85 pin 6
#elif defined(__AVR_ATmega2560__)
#define BPIN ( (uint8_t)A0 ) // Bass pin
#define TPIN ( (uint8_t)A1 ) // Treble pin
#define CPIN ( (uint8_t)52 ) // LED clock pin
#define DPIN ( (uint8_t)53 ) // Data pin
#else
#error 'Chip not supported'
#endif

#define MAX( a, b )( ((a) > (b)) ? (a) : (b) )
#define MIN( a, b )( ((a) > (b)) ? (b) : (a) )
#define IN
#define OUT

// table of 256 sine values / one sine period / stored in flash memory
// table from http://interface.khm.de/index.php/lab/experiments/arduino-dds-sinewave-generator/
PROGMEM  prog_uchar sine256[]  = {
    127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,176,178,181,184,187,190,192,195,198,200,203,205,208,210,212,215,217,219,221,223,225,227,229,231,233,234,236,238,239,240,
    242,243,244,245,247,248,249,249,250,251,252,252,253,253,253,254,254,254,254,254,254,254,253,253,253,252,252,251,250,249,249,248,247,245,244,243,242,240,239,238,236,234,233,231,229,227,225,223,
    221,219,217,215,212,210,208,205,203,200,198,195,192,190,187,184,181,178,176,173,170,167,164,161,158,155,152,149,146,143,139,136,133,130,127,124,121,118,115,111,108,105,102,99,96,93,90,87,84,81,78,
    76,73,70,67,64,62,59,56,54,51,49,46,44,42,39,37,35,33,31,29,27,25,23,21,20,18,16,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,16,18,20,21,23,25,27,29,31,
    33,35,37,39,42,44,46,49,51,54,56,59,62,64,67,70,73,76,78,81,84,87,90,93,96,99,102,105,108,111,115,118,121,124
};

struct rgb_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

#if 0
    void simplePulse
(
 IN uint16_t       bassAmpl,
 IN uint16_t       trebAmpl,
 IN OUT struct rgb_t      *color,
 IN OUT uint16_t   *delayTime
 )
{
    (void) bassAmpl;
    (void) trebAmpl;

    uint8_t const MAX_COLOR = 0xFF;
    uint8_t const MIN_COLOR = 0;

    static boolean bIncreasing;

    if ( color->r == MIN_COLOR )
    {
        bIncreasing = true;
    }
    else if ( color->r == MAX_COLOR )
    {
        bIncreasing = false;
    }

    color->r = ( bIncreasing ) ? (color->r + 1) : (color->r - 1);

    *delayTime = 5;
}
#endif


#if 0
void colorCycle
(
 IN uint32_t       counter,
 IN uint16_t       bassAmpl,
 IN uint16_t       trebAmpl,
 IN uint16_t       bassMax,
 IN uint16_t       trebMax,
 IN OUT struct rgb_t      *color,
 IN OUT uint16_t   *delayTime
 )
{
    // cycling algorithm from http://krazydad.com/tutorials/makecolors.php
    uint32_t const FREQUENCY = 27;
    uint16_t const PHASE_R = 81;
    uint16_t const PHASE_G = 0;
    uint16_t const PHASE_B = 162;
    uint32_t const MAX_COLOR = bassAmpl*255 / bassMax;

    uint8_t rSineIndex = ((FREQUENCY*counter/10) + PHASE_R) & 255;
    uint8_t gSineIndex = ((FREQUENCY*counter/10) + PHASE_G) & 255;
    uint8_t bSineIndex = ((FREQUENCY*counter/10) + PHASE_B) & 255;

    color->r = pgm_read_byte_near(sine256 + rSineIndex);
    color->g = pgm_read_byte_near(sine256 + gSineIndex);
    color->b = pgm_read_byte_near(sine256 + bSineIndex);

    color->r = color->r*MAX_COLOR / 255;
    color->g = color->g*MAX_COLOR / 255;
    color->b = color->b*MAX_COLOR / 255;

    *delayTime = 100;
}
#endif


void colorLevels
(
 IN uint32_t       counter,
 IN uint16_t       bassAmpl,
 IN uint16_t       trebAmpl,
 IN uint16_t       bassMax,
 IN uint16_t       trebMax,
 IN OUT struct rgb_t      *color,
 IN OUT uint16_t   *delayTime
 )
{
//  Serial.print("trebAmpl: ");
//  Serial.print(trebAmpl);
  Serial.print("\tbassMax:");
  Serial.println(bassMax);
  
  if ( bassAmpl > 950 ) {
    color->r = 255;
    color->g = 0;
    color->b = 0;  
  } else {
    color->r = 0;
    color->g = 0;
    color->b = 0;
  }
    /*
    if ( bassAmpl > bassMax * 1 / 10 )
    {
        // color->b = 0x10;
        if ( bassAmpl > bassMax * 2 / 10 )
        {
            // color->b = 0x40;
            if ( bassAmpl > bassMax * 3 / 10 )
            {
                // color->b = 0xA0;
                if ( bassAmpl > bassMax * 4 / 10 )
                {
                    // color->b = 0xFF;
                    if ( bassAmpl > bassMax * 5 / 10 )
                    {
                        color->g = 0x40;
                        if ( bassAmpl > bassMax * 6 / 10 )
                        {
                            color->g = 0xA0;
                            if ( bassAmpl > bassMax * 7 / 10 )
                            {
                                color->g = 0xFF;
                                if ( bassAmpl > bassMax * 8 / 10 )
                                {
                                    color->r = 0x40;
                                    if ( bassAmpl > bassMax * 9 / 10 )
                                    {
                                        color->r = 0xFF;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    */

    *delayTime = 25;
}


// construct LED driver object
Adafruit_WS2801 LedDriver ( 
        (uint16_t) 1,
        DPIN, 
        CPIN, 
        (uint8_t)WS2801_RGB 
        );

uint16_t delayTime;
struct rgb_t color;
uint32_t counter;
uint32_t Bass;
uint32_t Treb;
uint32_t Bdelay;
uint32_t Tdelay;
uint32_t Bmin;
uint32_t Tmin;
uint32_t Bmax;
uint32_t Tmax;
uint32_t B;
uint32_t T;
uint32_t Brange;
uint32_t Trange;
uint32_t Hmic;
uint32_t Lmic;

void setup() {
    // put your setup code here, to run once:
    LedDriver.begin();
    Serial.begin(9600);
}

void loop() {
    ++Bdelay;
    ++Tdelay;
    ++counter;

    if ( counter > 1023 )
    {
        ++Bmin;
        ++Tmin;
        --Bmax;
        --Tmax;
        counter = 0;
    }

    Bmin = MIN(Bmin, Bass);
    Tmin = MIN(Tmin, Treb);

    Bass = analogRead(BPIN) - Bmin;
    Treb = analogRead(TPIN) - Tmin;

    Bmax = MAX(Bmax, Bass);
    Tmax = MAX(Tmax, Treb);

    // puts back in range after a loud or anti-noise
    if ( Hmic > 10000 || Lmic > 10000 )
    {
        Bmax = ( analogRead(BPIN)/2 ) + 10;
        Bmin = ( analogRead(BPIN)/2 ) - 5;
        Tmax = ( analogRead(TPIN)/2 ) + 10;
        Tmin = ( analogRead(TPIN)/2 ) - 5;
        Hmic = 0;
        Lmic = 0;
    }

    // makes up for small changes in microphone output at low volumes
    Brange = Bmax - Bmin;
    B = Bass - Bmin;
    Trange = Tmax - Tmin;
    T = Treb - Tmin;

    if ( B > Brange*7/10 )
    {
        Lmic = 0;
    }
    if ( B > Brange*9/10 )
    {
        Hmic++;
    }
    if ( B < Brange*5/10 )
    {
        Hmic = 0;
        ++Lmic;
    }
    colorLevels( counter, B, T, Brange, Trange, &color, &delayTime );

    LedDriver.setPixelColor( 
            (uint16_t) 0,
            (uint8_t) color.r,
            (uint8_t) color.g,
            (uint8_t) color.b
            );

    LedDriver.show();
    delay( delayTime );
}
