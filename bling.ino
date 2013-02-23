#include <SPI.h>

#include <Adafruit_WS2801.h>

#define CPIN ( (uint8_t)52 )
#define DPIN ( (uint8_t)53 )

#define IN
#define OUT

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


// construct LED driver object
Adafruit_WS2801 LedDriver ( 
  (uint16_t) 1,
  DPIN, 
  CPIN, 
  (uint8_t)WS2801_RGB 
 );

uint16_t delayTime;
struct rgb_t color;

void setup() {
  // put your setup code here, to run once:
  LedDriver.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  simplePulse( 0, 0, &color, &delayTime );
  
  LedDriver.setPixelColor( 
    (uint16_t) 0,
    (uint8_t) color.r,
    (uint8_t) color.g,
    (uint8_t) color.b
  );
  LedDriver.show();
  delay( delayTime );
}
