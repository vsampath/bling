#include <SPI.h>
#include <avr/pgmspace.h>
#include <Adafruit_WS2801.h>
#include <limits.h>

#define CPIN ( (uint8_t)52 )
#define DPIN ( (uint8_t)53 )

#define IN
#define OUT

// table of 256 sine values / one sine period / stored in flash memory
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


void colorCycle
(
  IN uint16_t       bassAmpl,
  IN uint16_t       trebAmpl,
  IN OUT struct rgb_t      *color,
  IN OUT uint16_t   *delayTime
)
{
  (void) trebAmpl;

  uint32_t const FREQUENCY = 24;
  uint16_t const PHASE_R = 81;
  uint16_t const PHASE_G = 0;
  uint16_t const PHASE_B = 162;

  uint8_t rSineIndex = ((FREQUENCY*bassAmpl/10) + PHASE_R) & 255;
  uint8_t gSineIndex = ((FREQUENCY*bassAmpl/10) + PHASE_G) & 255;
  uint8_t bSineIndex = ((FREQUENCY*bassAmpl/10) + PHASE_B) & 255;

  color->r = pgm_read_byte_near(sine256 + rSineIndex);
  color->g = pgm_read_byte_near(sine256 + gSineIndex);
  color->b = pgm_read_byte_near(sine256 + bSineIndex);

  *delayTime = 10;
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

void setup() {
  // put your setup code here, to run once:
  LedDriver.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  colorCycle( counter, 0, &color, &delayTime );
  counter++;
  if ( counter == UINT_MAX ) counter = 0;
  
  LedDriver.setPixelColor( 
    (uint16_t) 0,
    (uint8_t) color.r,
    (uint8_t) color.g,
    (uint8_t) color.b
  );
  LedDriver.show();
  delay( delayTime );
}
