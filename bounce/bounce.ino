// A very simple bouncing pixel algorithm.
// Intended as a low power demo for hub75 RGB matrices.
// Requires 'gfx' & 'rgb matrix panel' libraries from Adafruit.
// KB 2019
// BSD license, all text above must be included in any redistribution.

#include <RGBmatrixPanel.h>

// Hardware
#define CLK  8   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3
#define RANDSRC A4

// Constants
#define HUEMAX 1536
#define MAX_BOUNCES 100
#define MIN_DELAY_MS 5
// maximum x/y delta of 3 gives some interesting variations
// and ensures that the pleasing (IMO) slower bouces occur often
#define MAX_DELTA 3

// Macros
#define RNDHUE random(HUEMAX)

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

void setup() {
  randomSeed(analogRead(RANDSRC));
  matrix.begin();
}

int speedup(int val) {
  if ( val > MIN_DELAY_MS) {
    return --val;
  }
}

void loop() {
  int dx = random(-MAX_DELTA, MAX_DELTA+1), dy = random(-MAX_DELTA, MAX_DELTA+1);
  int x = random(matrix.width()), y = random(matrix.height());
  // Re-roll the dice until we are not stationary!
  while (dx == 0 and dy == 0) {
    dx = random(-MAX_DELTA, MAX_DELTA+1);
    dy = random(-MAX_DELTA, MAX_DELTA+1);
  }
  long hue = RNDHUE;
  int delay_ms = 20, bounces = 0;

  while (bounces < MAX_BOUNCES) {
    matrix.drawPixel(x, y, matrix.ColorHSV(hue, 255, 255, true));

    delay(delay_ms);
    matrix.drawPixel(x, y, matrix.Color888(0, 0, 0, true));
    if (x + dx >= matrix.width() or x + dx < 0) {
      dx = 0 - dx;
      hue = RNDHUE;
      bounces++;
      delay_ms = speedup(delay_ms);
    }
    if (y + dy >= matrix.height() or y + dy < 0) {
      dy = 0 - dy;
      hue = RNDHUE;
      bounces++;
      delay_ms = speedup(delay_ms);
    }
    x += dx;
    y += dy;
  }
}
