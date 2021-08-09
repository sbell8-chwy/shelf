#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMPIXELS 71

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t pink = strip.gamma32(strip.Color(253, 41, 188));
uint32_t purple = strip.gamma32(strip.Color(255, 56, 255));
uint32_t red = strip.gamma32(strip.Color(196, 2, 48));

uint32_t colors[] = {pink, purple, red};
int color_index = 0;
uint32_t current_color = colors[0];

void setup() {
  strip.begin();
  strip.show();
}

void loop() {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, current_color);
  }
  strip.show();
  delay(5000);
  uint32_t old_color = current_color;
  color_index = (color_index + 1) % 3;
  current_color = colors[color_index];
//  transition(old_color, current_color);
}

void breath() {
}

void transition(uint32_t old, uint32_t next) {
  int step = 1;
  if (old > next) {
    step = -1;
  }
  for (uint32_t c = old; c != next; c += step) {
    for (int i = 0; i < NUMPIXELS; i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(10);
  }
}
