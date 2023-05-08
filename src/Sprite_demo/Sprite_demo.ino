// Uncomment 1 and only 1 option
// #define USE_INDEXED_SPRITE
#define USE_INDEXED_SPRITE_AND_CANVAS
// #define USE_SPRITE

/*******************************************************************************
 * Start of Arduino_GFX setting
 *
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * Arduino Nano, Micro and more: CS:  9, DC:  8, RST:  7, BL:  6, SCK: 13, MOSI: 11, MISO: 12
 * ESP32 various dev board     : CS:  5, DC: 27, RST: 33, BL: 22, SCK: 18, MOSI: 23, MISO: nil
 * ESP32-C3 various dev board  : CS:  7, DC:  2, RST:  1, BL:  3, SCK:  4, MOSI:  6, MISO: nil
 * ESP32-S2 various dev board  : CS: 34, DC: 38, RST: 33, BL: 21, SCK: 36, MOSI: 35, MISO: nil
 * ESP32-S3 various dev board  : CS: 40, DC: 41, RST: 42, BL: 48, SCK: 36, MOSI: 35, MISO: nil
 * ESP8266 various dev board   : CS: 15, DC:  4, RST:  2, BL:  5, SCK: 14, MOSI: 13, MISO: 12
 * Raspberry Pi Pico dev board : CS: 17, DC: 27, RST: 26, BL: 28, SCK: 18, MOSI: 19, MISO: 16
 * RTL8720 BW16 old patch core : CS: 18, DC: 17, RST:  2, BL: 23, SCK: 19, MOSI: 21, MISO: 20
 * RTL8720_BW16 Official core  : CS:  9, DC:  8, RST:  6, BL:  3, SCK: 10, MOSI: 12, MISO: 11
 * RTL8722 dev board           : CS: 18, DC: 17, RST: 22, BL: 23, SCK: 13, MOSI: 11, MISO: 12
 * RTL8722_mini dev board      : CS: 12, DC: 14, RST: 15, BL: 13, SCK: 11, MOSI:  9, MISO: 10
 * Seeeduino XIAO dev board    : CS:  3, DC:  2, RST:  1, BL:  0, SCK:  8, MOSI: 10, MISO:  9
 * Teensy 4.1 dev board        : CS: 39, DC: 41, RST: 40, BL: 22, SCK: 13, MOSI: 11, MISO: 12
 ******************************************************************************/
#include <Arduino_GFX_Library.h>

#define GFX_DEV_DEVICE LILYGO_T_DISPLAY_S3
#define GFX_EXTRA_PRE_INIT()          \
  {                                   \
    pinMode(15 /* PWD */, OUTPUT);    \
    digitalWrite(15 /* PWD */, HIGH); \
  }
#define GFX_BL 38
Arduino_DataBus *bus = new Arduino_ESP32PAR8Q(
    7 /* DC */, 6 /* CS */, 8 /* WR */, 9 /* RD */,
    39 /* D0 */, 40 /* D1 */, 41 /* D2 */, 42 /* D3 */, 45 /* D4 */, 46 /* D5 */, 47 /* D6 */, 48 /* D7 */);
Arduino_G *output_display = new Arduino_ST7789(bus, 5 /* RST */, 3 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */, 35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);

// #define GFX_DEV_DEVICE ESP32_1732S019
// #define GFX_BL 14
// Arduino_DataBus *bus = new Arduino_ESP32SPI(11 /* DC */, 10 /* CS */, 12 /* SCK */, 13 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
// Arduino_G *output_display = new Arduino_ST7789(bus, 1 /* RST */, 3 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */, 35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);

#if defined(USE_INDEXED_SPRITE_AND_CANVAS)
Arduino_Canvas_Indexed *gfx = new Arduino_Canvas_Indexed(320 /* width */, 170 /* height */, output_display);
#else
Arduino_GFX *gfx = new Arduino_Canvas(320 /* width */, 170 /* height */, output_display);
#endif
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

#if defined(USE_INDEXED_SPRITE) or defined(USE_INDEXED_SPRITE_AND_CANVAS)
#include "Indexed_Sprite.h"
#elif defined(USE_SPRITE)
#include "Sprite.h"
#endif

#include "image/imageA.h"
#include "image/imageB.h"
#include "image/imageC0.h"
#include "image/imageC1.h"
#include "image/imageC2.h"
#include "image/imageC3.h"
#include "image/imageC4.h"
#include "image/imageG0.h"
#include "image/imageG1.h"
#include "image/imageG2.h"
#include "image/imageG3.h"
#include "image/imageG4.h"
#include "image/imageKaramoon2.h"
#include "image/imageL.h"
#include "image/imageM.h"

#if defined(USE_INDEXED_SPRITE) or defined(USE_INDEXED_SPRITE_AND_CANVAS)
Indexed_Sprite *a;
Indexed_Sprite *b;
Indexed_Sprite *c0;
Indexed_Sprite *c1;
Indexed_Sprite *c2;
Indexed_Sprite *c3;
Indexed_Sprite *c4;
Indexed_Sprite *g0;
Indexed_Sprite *g1;
Indexed_Sprite *g2;
Indexed_Sprite *g3;
Indexed_Sprite *g4;
Indexed_Sprite *karamoon;
Indexed_Sprite *l;
Indexed_Sprite *m;
#elif defined(USE_SPRITE)
Sprite *a;
Sprite *b;
Sprite *c0;
Sprite *c1;
Sprite *c2;
Sprite *c3;
Sprite *c4;
Sprite *g0;
Sprite *g1;
Sprite *g2;
Sprite *g3;
Sprite *g4;
Sprite *karamoon;
Sprite *l;
Sprite *m;
#endif

// Set Up Basic Loop with FPS counter
int frame = 0;
volatile int fSnapShot = 0;
TaskHandle_t SecondTicker;
void SecondTickerCode(void *p)
{
  // Infinite Loop
  for (;;)
  {
    // Sleep for 1 second
    sleep(1);
    fSnapShot = frame;
    frame = 0;
  }
}

void setup()
{
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Sprite Demo");

  // size_t len = sizeof(imageA);
  // size_t i = 0;
  // for (int f = 0; f < 6; f++)
  // {
  //   for (int y = 0; y < 50; y++)
  //   {
  //     delay(100);
  //     for (int x = 0; x < 40; x++)
  //     {
  //       Serial.printf("0x%02x,", imageA[(y * 40 * 6) + (f * 40) + x]);
  //     }
  //     Serial.println();
  //   }
  // }

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  gfx->begin(80000000);
  gfx->fillScreen(BLACK);
  gfx->flush();
#if defined(USE_INDEXED_SPRITE_AND_CANVAS)
  gfx->setDirectUseColorIndex(true);
  // inital palette
  memcpy(gfx->getColorIndex(), paletteA, sizeof(paletteA));
#endif

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

#if defined(USE_INDEXED_SPRITE) or defined(USE_INDEXED_SPRITE_AND_CANVAS)
  karamoon = new Indexed_Sprite(0, 0, imageKaramoon2, paletteKaramoon, 336, 200, true, 1);
  a = new Indexed_Sprite(130, 94, imageA, paletteA, 40, 50, false, 6, 0x0);
  b = new Indexed_Sprite(320, 149, imageB, paletteB, 320, 21, true, 1, 0x0);
  c0 = new Indexed_Sprite(320, 0, imageC0, paletteC0, 320, 21, true, 1, 0x0);
  c1 = new Indexed_Sprite(320, 22, imageC1, paletteC1, 320, 40, true, 1, 0x0);
  c2 = new Indexed_Sprite(320, 62, imageC2, paletteC2, 320, 19, true, 1, 0x0);
  c3 = new Indexed_Sprite(320, 81, imageC3, paletteC3, 320, 9, true, 1, 0x0);
  c4 = new Indexed_Sprite(320, 90, imageC4, paletteC4, 320, 6, true, 1, 0x0);
  g0 = new Indexed_Sprite(320, 140, imageG0, paletteG0, 320, 2, true, 1);
  g1 = new Indexed_Sprite(320, 142, imageG1, paletteG1, 320, 3, true, 1);
  g2 = new Indexed_Sprite(320, 145, imageG2, paletteG2, 320, 7, true, 1);
  g3 = new Indexed_Sprite(320, 152, imageG3, paletteG3, 320, 7, true, 1);
  g4 = new Indexed_Sprite(320, 159, imageG4, paletteG4, 320, 11, true, 1);
  l = new Indexed_Sprite(320, 20, imageL, paletteL, 53, 53, false, 1, 0x0);
  m = new Indexed_Sprite(320, 67, imageM, paletteM, 320, 73, true, 1, 0x0);
#elif defined(USE_SPRITE)
  karamoon = new Sprite(0, 0, imageKaramoon2, paletteKaramoon, 336, 200, true, 1);
  a = new Sprite(130, 94, imageA, paletteA, 40, 50, false, 6, 0x0);
  b = new Sprite(320, 149, imageB, paletteB, 320, 21, true, 1, 0x0);
  c0 = new Sprite(320, 0, imageC0, paletteC0, 320, 21, true, 1, 0x0);
  c1 = new Sprite(320, 22, imageC1, paletteC1, 320, 40, true, 1, 0x0);
  c2 = new Sprite(320, 62, imageC2, paletteC2, 320, 19, true, 1, 0x0);
  c3 = new Sprite(320, 81, imageC3, paletteC3, 320, 9, true, 1, 0x0);
  c4 = new Sprite(320, 90, imageC4, paletteC4, 320, 6, true, 1, 0x0);
  g0 = new Sprite(320, 140, imageG0, paletteG0, 320, 2, true, 1);
  g1 = new Sprite(320, 142, imageG1, paletteG1, 320, 3, true, 1);
  g2 = new Sprite(320, 145, imageG2, paletteG2, 320, 7, true, 1);
  g3 = new Sprite(320, 152, imageG3, paletteG3, 320, 7, true, 1);
  g4 = new Sprite(320, 159, imageG4, paletteG4, 320, 11, true, 1);
  l = new Sprite(320, 20, imageL, paletteL, 53, 53, false, 1, 0x0);
  m = new Sprite(320, 67, imageM, paletteM, 320, 73, true, 1, 0x0);
#endif

  gfx->setTextColor(WHITE, BLACK);
  gfx->setTextSize(1);

  // Second Ticker thread, pinned to code 0, while Core 1 runs the sprite code
  xTaskCreatePinnedToCore(SecondTickerCode, "Seconds", 1000, NULL, -1, &SecondTicker, 0);
}

bool otherFrame = false;
void testingLoop(void)
{
  karamoon->h_scroll(-1);
  karamoon->draw(gfx);

  otherFrame = !otherFrame;
  if (otherFrame)
  {
    l->h_scroll(-1, 320);
    a->next_frame();
  }
  l->draw(gfx);

  c0->h_scroll(-7);
  c0->draw(gfx);

  c1->h_scroll(-6);
  c1->draw(gfx);

  c4->h_scroll(-2);
  c4->draw(gfx);

  m->h_scroll(-3);
  m->draw(gfx);

  c3->h_scroll(-4);
  c3->draw(gfx);

  c2->h_scroll(-5);
  c2->draw(gfx);

  g0->h_scroll(-3);
  g0->draw(gfx);

  g1->h_scroll(-4);
  g1->draw(gfx);

  g2->h_scroll(-5);
  g2->draw(gfx);

  g3->h_scroll(-6);
  g3->draw(gfx);

  g4->h_scroll(-7);
  g4->draw(gfx);

  b->h_scroll(-8);
  b->draw(gfx);

  a->draw(gfx);
}

void loop(void)
{
  testingLoop();

  gfx->setCursor(2, 2);
  gfx->print(fSnapShot);

  gfx->flush();
  frame++;
}
