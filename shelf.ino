#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMPIXELS 71

enum Pattern { BREATH, OFF, SCANNER };

class NeoStrip : public Adafruit_NeoPixel {
    public:
    Pattern currentPattern = OFF;

    unsigned long interval = 500;
    unsigned long lastUpdate = 0;

    uint32_t pinkColor = Color(253, 41, 188);
    uint32_t purpleColor = Color(255, 56, 255);
    uint32_t redColor = Color(196, 2, 48);
    uint32_t colors[3] = {pinkColor, purpleColor, redColor};
    uint8_t colorSize = 3;

    int totalSteps = 10;
    int index = 0;

    NeoStrip() :Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800) {}

    void update() {
        if (millis() - lastUpdate > interval) {
            lastUpdate = millis();
            switch(currentPattern) {
                case OFF:
                    offUpdate();
                    break;
                case BREATH:
                    breathUpdate();
                    break;
                case SCANNER:
                    scannerUpdate();
                    break;
                default:
                    break;
            }
        }
    }

    void increment() {
        index++;
        if (index >= totalSteps) {
            index = 0;
        }
    }

    void off() {
        currentPattern = OFF;
        totalSteps = 10;
    }

    void offUpdate() {
        setAll(Color(0, 0, 0));
        show();
        increment();
    }

    void breath() {
        currentPattern = BREATH;
        totalSteps = (20 * colorSize) - 1;
        interval = 500;
        index = 0;
    }

    void breathUpdate() {
        uint16_t tempIndex = index;
        uint8_t colorIndex = 0;
        while (tempIndex >= 20) {
            colorIndex++;
            tempIndex = tempIndex - 20;
        }
        if (tempIndex < 15) {
            setAll(gamma32(colors[colorIndex]));
        } else {
            uint32_t cFrom = colors[colorIndex];
            uint32_t cTo;
            if (colorIndex == colorSize) {
                cTo = colors[0];
            } else {
                cTo = colors[colorIndex + 1];
            }
            setAll(gamma32(getTransitionColor(cFrom, cTo, tempIndex - 15, 5)));
        }
        show();
        increment();
    }

    void scanner() {
    }

    void scannerUpdate() {
    }

    uint32_t getTransitionColor(uint32_t from, uint32_t to, int step, int stepCount) {
        if (step == stepCount) {
            return to;
        }
        uint8_t nextRed = computeTransitionColor(red(from), red(to), step, stepCount);
        uint8_t nextGreen = computeTransitionColor(green(from), green(to), step, stepCount);
        uint8_t nextBlue = computeTransitionColor(blue(from), blue(to), step, stepCount);

        return Color(nextRed, nextGreen, nextBlue);
    }

    uint8_t computeTransitionColor(uint8_t from, uint8_t to, int step, int stepCount) {
        int temp = (from - to) * 10;
        temp = (temp / stepCount) * (step + 1);
        temp += 5;
        temp = from - (temp / 10);
        return temp;
    }

    uint8_t red(uint32_t c) {
        return (c >> 16) & 0xFF;
    }

    uint8_t green(uint32_t c) {
        return (c >> 8) & 0xFF;
    }

    uint8_t blue(uint32_t c) {
        return c & 0xFF;
    }

    void setAll(uint32_t color) {
        for (int i = 0; i < NUMPIXELS; i++) {
            setPixelColor(i, color);
        }
    }
};

NeoStrip strip;

// int color_index = 0;
// uint32_t current_color = colors[0];

void setup() {
    Serial.begin(9600);
    strip.begin();
    strip.breath();
}

void loop() {
    strip.update();
  // for (int i = 0; i < NUMPIXELS; i++) {
    // strip.setPixelColor(i, current_color);
  // }
  // strip.show();
  // delay(5000);
  // uint32_t old_color = current_color;
  // color_index = (color_index + 1) % 3;
  // current_color = colors[color_index];
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
