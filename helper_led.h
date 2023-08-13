#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_GRBW    Pixels are wired for GRBW bitstream (RGB+W NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, D3,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);


//const uint16_t colors[] = {
//  matrix.Color(0, 255, 0), matrix.Color(128, 255, 0), matrix.Color(255, 255, 0), matrix.Color(255, 128, 0), matrix.Color(255, 0, 0) };

const uint16_t colors[] = {
  matrix.Color(0, 255, 0), matrix.Color(85, 170, 0), matrix.Color(105, 140, 0), matrix.Color(128, 128, 0), matrix.Color(255, 0, 0) };

bool firstBlink;

void matrixInitialize() {
  Serial.println("Initialize Matrix");
  matrix.begin();
  matrix.setBrightness(20);
  firstBlink = true;
}

int x    = matrix.width();
int pass = 0;

void matrixLine(int reihe, int hoehe, uint16_t farbe) {
  matrix.writeLine( reihe,  0,  reihe,  hoehe-1,  farbe);
  //matrix.writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
}

void matrixShow() {
  
  matrix.fillScreen(0);
  matrixLine(0, 1, colors[0]);
  matrixLine(1, 2, colors[1]);
  matrixLine(2, 3, colors[2]);
  matrixLine(3, 4, colors[3]);
  matrixLine(4, 5, colors[4]);
  matrixLine(5, 6, colors[3]);
  matrixLine(6, 7, colors[2]);
  matrixLine(7, 8, colors[1]);
  matrix.show();
}  



void matrixShowTibber() {

  matrix.fillScreen(0);
  firstBlink = !firstBlink;
  for (int i = 0; i<8; i++) {
    if (!PRICES.price[i].isNull) {
      int hoehe;
      if (PRICES.maximumPrice == PRICES.minimumPrice) {
        hoehe = 1;
      } else {
        hoehe = (int) 8 * (PRICES.price[i].price - PRICES.minimumPrice) / (PRICES.maximumPrice - PRICES.minimumPrice);
        hoehe = hoehe +1;
      }

      if (hoehe > 8) {
        hoehe = 8;
      }

      
      if (i!=0 || firstBlink) {
        matrixLine(i,hoehe, colors[PRICES.price[i].level]);
      }
    }
  }
  matrix.show();

}  