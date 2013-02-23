#include <SPI.h>

#include <Adafruit_WS2801.h>

#define CPIN ( (uint8_t)52 )
#define DPIN ( (uint8_t)53 )

// construct LED driver object
Adafruit_WS2801 LedDriver ( 
  (uint16_t) 1,
  DPIN, 
  CPIN, 
  (uint8_t)WS2801_RGB 
 );

void setup() {
  // put your setup code here, to run once:
  LedDriver.begin();
  LedDriver.setPixelColor( 
    (uint16_t) 0,
    (uint8_t) 0xFF,
    (uint8_t) 0,
    (uint8_t) 0xFF
    );
  LedDriver.show();
}

void loop() {
  // put your main code here, to run repeatedly: 
  
}
