#include <Adafruit_NeoPixel.h>


#define LED_PIN    A3
#define LED_COUNT 4
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {


  strip.begin();
  strip.show();
  strip.setBrightness(100);
  strip.setPixelColor(0, strip.Color(0, 0, 255));
  strip.setPixelColor(1, strip.Color(255, 0, 0));
  strip.setPixelColor(2, strip.Color(0, 255, 0));
  strip.setPixelColor(3, strip.Color(255, 255, 255));

  strip.show();
}

void loop() {
}
