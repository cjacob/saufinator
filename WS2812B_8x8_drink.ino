#include "FastLED.h"

//Dateineingang des WS2812B am Arduino Port
//DATA_PIN 12 entsprich D6 am NodeMCU
#define DATA_PIN 12

#define NUM_STRIPS 1
#define NUM_LEDS_PER_STRIP 64
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS
#define COLOR_ORDER RGB
#define CHIPSET     WS2812B
#define BRIGHTNESS 80

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

// Einrichten der Matrix,
// siehe Funktion truePixel
const bool myMatrixSerpentineLayout = true;
const bool myMatrixLayoutStartsRight = true;

// Pixel außen definieren
int pixelaussen[] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 15, 16, 23, 24, 31, 32, 39, 40, 47, 48, 55, 56, 57, 58, 59, 60, 61, 62, 63
};

// Pfeile (6 x 6) definieren
int pfeile[][8][8] = {
  {
    //links oben
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0},
    {0, 1, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  //oben
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  //rechts oben
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 0, 1, 0},
    {0, 1, 1, 1, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  //rechts
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  //rechts unten
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 1, 0},
    {0, 0, 0, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  // unten
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  //links unten
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 1, 0},
    {0, 1, 0, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  //links
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  }
};

// Pfeile definieren (große Pfeile, werden nicht verwendet)
int pfeile_gross[][8][8] = {
  {
    //links oben
    {1, 1, 1, 1, 1, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 0, 0, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  //oben
  {
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 0},
    {1, 0, 0, 1, 0, 0, 1, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  //rechts oben
  {
    {0, 0, 0, 0, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 1, 1},
    {0, 0, 0, 0, 0, 1, 0, 1},
    {0, 0, 0, 0, 1, 0, 0, 1},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  //rechts
  {
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  //rechts unten
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 1},
    {0, 0, 0, 0, 0, 1, 0, 1},
    {0, 0, 0, 0, 0, 0, 1, 1},
    {0, 0, 0, 0, 1, 1, 1, 1}
  },
  // unten
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0}
  },
  //links unten
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 0, 0},
    {1, 0, 1, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 0, 0, 0, 0}
  },
  //links
  {
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  }
};

// Zahlen definieren. Werden für Counter verwendet
int numbers[][8][8] = {
  {
    // 0
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // 1
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // 2
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // 3
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // 4
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // 5
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // 6
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // 7
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // 8
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    // 9
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  }
};

/* Funktion um Serpentinenlayout abzubilden
     1  2  3  4  5  6  7  8
 *                        *
    16 15 14 13 12 11 10 9
 *  *
    17 18 19 20 ...

    statt
    1  2  3  4  5  6  7  8
 *                       *
 *  **********************
 *  *
    9 10 11 12 13 14 15 16
 *                      *
 *  *********************
 *  *
    17 18 19 20 ...

    abzubilenden.

    Und die Matrix links/rechts zu vertauschen

    Steuerung über
    const bool myMatrixSerpentineLayout = true;
    const bool myMatrixLayoutStartsRight = true;
    (siehe oben)
*/
int truePixel(int i) {
  int truei = i;
  if (myMatrixSerpentineLayout) {
    int spalte = i / 8;
    if ( (i / 8) & 0x01 ) {
      truei =  7 - (i % 8) + 8 * spalte;
    } else {
      truei = i;
    }
  }
  if (myMatrixLayoutStartsRight) {
    truei = NUM_LEDS - truei - 1 ;
  }
  return truei;
}

//Start
/*
  uint16_t XY( uint8_t x, uint8_t y)
  {
  uint16_t i;

  if( myMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if( myMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i;
  }


  uint16_t XYsafe( uint8_t x, uint8_t y)
  {
  if( x >= kMatrixWidth) return -1;
  if( y >= kMatrixHeight) return -1;
  return XY(x,y);
  }


  void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
  {
  byte lineStartHue = startHue8;
  for( byte y = 0; y < kMatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;
    for( byte x = 0; x < kMatrixWidth; x++) {
      pixelHue += xHueDelta8;
      leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
    }
  }
  }
*/
//END


void setup() {
  // setup code to run once:

  // tell FastLED there's 60 NEOPIXEL leds on DATA_PIN, starting at index 0 in the led array
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS_PER_STRIP);

  // Helligkeit setzen
  LEDS.setBrightness(BRIGHTNESS);

  // Random initialisieren mit Rauschen von A0
  randomSeed(analogRead(0));

  //Serielle Schnittstelle öffnen (zum Testen)
  //Serial.begin(9600);
}

void loop() {
  // main code to run repeatedly:
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[truePixel(i)] = CRGB::Green;
    FastLED.show();
    leds[truePixel(i)] = CRGB::Black;
    delay(10);
  }
  FastLED.show();
  delay(150);
  int pixel = 0;
  long randNumber;
  randNumber = random(50, 100);
  //Serial.println(".");
  //Serial.print("Random: ");
  //Serial.println(randNumber);
  for (int zaehler = 0; zaehler < randNumber; zaehler++) {
    int pfeil = (zaehler % 8);
    for (int zeile = 0; zeile < 8; zeile++) {
      for (int spalte = 0; spalte < 8; spalte++) {
        pixel = spalte * 8 + zeile;
        if (pfeile[pfeil][zeile][spalte] == 1) {
          leds[truePixel(pixel)] = CRGB::Red;
        } else {
          leds[truePixel(pixel)] = CRGB::Black;
        }
      }
    }
    FastLED.show();
    /*
      Delay testen:
      Serial.print(zaehler + 1);
      Serial.print(", ");
      Serial.print((zaehler + 1) / (randNumber / 4));
      Serial.print(": ");
      Serial.print(100+(100*(zaehler + 1) / (randNumber / 4)));
      Serial.print("; ");*/
    // delay wird in vier Schritten größer
    delay(50 + (120 * (zaehler + 1) / (randNumber / 4)));
  }
  for (int aussen = 0; aussen < 28; aussen++)  {
    leds[truePixel(pixelaussen[aussen])] = CRGB::Blue;
  }
  FastLED.show();
  delay(300);
  for (int aussen = 0; aussen < 28; aussen++)  {
    leds[truePixel(pixelaussen[aussen])] = CRGB::Black;
  }
  FastLED.show();
  delay(300);
  for (int aussen = 0; aussen < 28; aussen++)  {
    leds[truePixel(pixelaussen[aussen])] = CRGB::HotPink;
  }
  FastLED.show();
  delay(300);
  for (int aussen = 0; aussen < 28; aussen++)  {
    leds[truePixel(pixelaussen[aussen])] = CRGB::Black;
  }
  FastLED.show();
  delay(300);
  for (int aussen = 0; aussen < 28; aussen++)  {
    leds[truePixel(pixelaussen[aussen])] = CRGB::Green;
  }
  FastLED.show();
  delay(1000);
  for (int aussen = 0; aussen < 28; aussen++)  {
    leds[truePixel(pixelaussen[aussen])] = CRGB::Yellow;
  }
  FastLED.show();
  delay(1000);
  for (int aussen = 0; aussen < 28; aussen++)  {
    leds[truePixel(pixelaussen[aussen])] = CRGB::Green;
  }
  FastLED.show();
  delay(1000);

  // START
  /*
    for (int i = 0; i < 100; i++) {
    uint32_t ms = millis();
    int32_t yHueDelta32 = ((int32_t)cos16( ms * (27 / 1) ) * (350 / kMatrixWidth));
    int32_t xHueDelta32 = ((int32_t)cos16( ms * (39 / 1) ) * (310 / kMatrixHeight));
    DrawOneFrame( ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
    if ( ms < 5000 ) {
      FastLED.setBrightness( scale8( BRIGHTNESS, (ms * 256) / 5000));
    } else {
      FastLED.setBrightness(BRIGHTNESS);
    }
    FastLED.show();
    }

  */
  //END

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[truePixel(i)] = CRGB::HotPink;
    FastLED.show();
    leds[truePixel(i)] = CRGB::Black;
    delay(10);
  }
  FastLED.show();
  delay(500);
  for (int nummer = 9; nummer >= 0; nummer = nummer - 1) {
    for (int zeile = 0; zeile < 8; zeile++) {
      for (int spalte = 0; spalte < 8; spalte++) {
        pixel = spalte * 8 + zeile;
        if (numbers[nummer][zeile][spalte] == 1) {
          leds[truePixel(pixel)] = CRGB::Red;
        } else {
          leds[truePixel(pixel)] = CRGB::Black;
        }
      }
    }
    FastLED.show();
    delay(1000);
  }
}
