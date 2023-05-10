/*******************************************************************************
 * GIF Sprite Demo
 * This is a simple GIF Sprite Demo example
 * Image Source:
 * https://www.freepik.com/free-vector/urban-life-drawing_727890.htm#query=city%20road
 * https://giphy.com/gifs/car-carro-nQaMsylXcTIRNorQLJ
 *
 * Setup steps:
 * 1. Change your LCD parameters in Arduino_GFX setting
 * 2. Upload Animated GIF file
 *   FFat (ESP32):
 *     upload FFat (FatFS) data with ESP32 Sketch Data Upload:
 *     ESP32: https://github.com/lorol/arduino-esp32fs-plugin
 *   LittleFS (ESP32 / ESP8266 / Pico):
 *     upload LittleFS data with ESP8266 LittleFS Data Upload:
 *     ESP32: https://github.com/lorol/arduino-esp32fs-plugin
 *     ESP8266: https://github.com/earlephilhower/arduino-esp8266littlefs-plugin
 *     Pico: https://github.com/earlephilhower/arduino-pico-littlefs-plugin.git
 *   SPIFFS (ESP32):
 *     upload SPIFFS data with ESP32 Sketch Data Upload:
 *     ESP32: https://github.com/lorol/arduino-esp32fs-plugin
 *   SD:
 *     Most Arduino system built-in support SD file system.
 *     Wio Terminal require extra dependant Libraries:
 *     - Seeed_Arduino_FS: https://github.com/Seeed-Studio/Seeed_Arduino_FS.git
 *     - Seeed_Arduino_SFUD: https://github.com/Seeed-Studio/Seeed_Arduino_SFUD.git
 ******************************************************************************/
#define GIF_FILENAME "/city17_320.gif"

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
Arduino_G *output_display = new Arduino_ST7789(bus, 5 /* RST */, 0 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */, 35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);

// #define GFX_DEV_DEVICE ESP32_1732S019
// #define GFX_BL 14
// Arduino_DataBus *bus = new Arduino_ESP32SPI(11 /* DC */, 10 /* CS */, 12 /* SCK */, 13 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
// Arduino_G *output_display = new Arduino_ST7789(bus, 1 /* RST */, 0 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */, 35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);

Arduino_Canvas_Indexed *gfx = new Arduino_Canvas_Indexed(170 /* width */, 320 /* height */, output_display);
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
#include <Seeed_FS.h>
#include <SD/Seeed_SD.h>
#elif defined(TARGET_RP2040)
#include <LittleFS.h>
#include <SD.h>
#elif defined(ESP32)
#include <FFat.h>
#include <LittleFS.h>
#include <SPIFFS.h>
#include <SD.h>
#elif defined(ESP8266)
#include <LittleFS.h>
#include <SD.h>
#else
#include <SD.h>
#endif

#include "GifClass.h"
static GifClass gifClass;

uint8_t *spriteMaster;
bool spriteInitiated = false;

#include "Indexed_Sprite.h"
Indexed_Sprite *background;
Indexed_Sprite *mountains;
Indexed_Sprite *road;
Indexed_Sprite *buildings;
Indexed_Sprite *grasses2;
Indexed_Sprite *cars;
Indexed_Sprite *grasses1;
Indexed_Sprite *birds;
Indexed_Sprite *clouds;
Indexed_Sprite *mpv;

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
  Serial.println("GIF Sprite Demo");

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  gfx->begin(80000000);
  gfx->fillScreen(BLACK);
  gfx->flush();
  gfx->setDirectUseColorIndex(true);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
  if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 4000000UL))
#elif defined(TARGET_RP2040)
  if (!LittleFS.begin())
  // if (!SD.begin(SS))
#elif defined(ESP32)
  // if (!FFat.begin())
  if (!LittleFS.begin())
  // if (!SPIFFS.begin())
  // if (!SD.begin(SS))
#elif defined(ESP8266)
  if (!LittleFS.begin())
  // if (!SD.begin(SS))
#else
  if (!SD.begin())
#endif
  {
    Serial.println(F("ERROR: File System Mount Failed!"));
    gfx->println(F("ERROR: File System Mount Failed!"));
    exit(0);
  }

/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
  File gifFile = SD.open(GIF_FILENAME, "r");
#elif defined(TARGET_RP2040)
  File gifFile = LittleFS.open(GIF_FILENAME, "r");
  // File gifFile = SD.open(GIF_FILENAME, "r");
#elif defined(ESP32)
  // File gifFile = FFat.open(GIF_FILENAME, "r");
  File gifFile = LittleFS.open(GIF_FILENAME, "r");
  // File gifFile = SPIFFS.open(GIF_FILENAME, "r");
  // File gifFile = SD.open(GIF_FILENAME, "r");
#elif defined(ESP8266)
  File gifFile = LittleFS.open(GIF_FILENAME, "r");
  // File gifFile = SD.open(GIF_FILENAME, "r");
#else
  File gifFile = SD.open(GIF_FILENAME, FILE_READ);
#endif
  if (!gifFile || gifFile.isDirectory())
  {
    Serial.println(F("ERROR: open gifFile Failed!"));
    gfx->println(F("ERROR: open gifFile Failed!"));
  }
  else
  {
    // read GIF file header
    gd_GIF *gif = gifClass.gd_open_gif(&gifFile);
    if (!gif)
    {
      Serial.println(F("gd_open_gif() failed!"));
    }
    else
    {
      spriteMaster = (uint8_t *)malloc(gif->width * gif->height);
      if (!spriteMaster)
      {
        Serial.println(F("spriteMaster malloc failed!"));
      }
      else
      {
        int32_t res = gifClass.gd_get_frame(gif, spriteMaster);

        if (res > 0)
        {
          // inital palette
          uint16_t *palette = gfx->getColorIndex();
          memcpy(palette, gif->palette->colors, gif->palette->len * 2);

          background = new Indexed_Sprite(0, 0, spriteMaster, palette, 540, 205, 0, false, 1, 0);
          mountains = new Indexed_Sprite(0, 81, spriteMaster + (205 * 540), palette, 540, 124, 0, true, 1, 3, gif->gce.tindex);
          road = new Indexed_Sprite(0, 205, spriteMaster + (478 * 540), palette, 540, 108, 0, true, 1, 1);
          buildings = new Indexed_Sprite(0, 100, spriteMaster + (329 * 540), palette, 540, 128, 0, true, 1, 1, gif->gce.tindex);
          grasses2 = new Indexed_Sprite(0, 220, spriteMaster + (457 * 540), palette, 540, 21, 0, true, 1, 1, gif->gce.tindex);
          cars = new Indexed_Sprite(0, 245, spriteMaster + (586 * 540), palette, 540, 14, 0, true, 1, 1, gif->gce.tindex);
          grasses1 = new Indexed_Sprite(0, 302, spriteMaster + (600 * 540), palette, 540, 20, 0, true, 1, 1, gif->gce.tindex);
          birds = new Indexed_Sprite(0, 101, spriteMaster + (620 * 540), palette, 51, 32, (540 - 51), false, 4, 4, gif->gce.tindex);
          clouds = new Indexed_Sprite(0, 2, spriteMaster + (652 * 540), palette, 540, 124, 0, true, 1, 2, gif->gce.tindex);
          mpv = new Indexed_Sprite((gfx->width() - 80) / 2, 242, spriteMaster + (776 * 540), palette, 67, 40, (540 - 67), false, 8, 2, gif->gce.tindex);

          spriteInitiated = true;
        }

        gifClass.gd_close_gif(gif);
      }
    }
  }

  gfx->setTextColor(0xfd, 0x00);
  gfx->setTextSize(1);

  // Second Ticker thread, pinned to code 0, while Core 1 runs the sprite code
  xTaskCreatePinnedToCore(SecondTickerCode, "Seconds", 1000, NULL, -1, &SecondTicker, 0);
}

bool otherFrame = false;
void testingLoop(void)
{
  if (spriteInitiated)
  {
    background->draw(gfx);

    mountains->h_scroll(-1);
    mountains->draw(gfx);

    road->h_scroll(-3);
    road->draw(gfx);

    buildings->h_scroll(-1);
    buildings->draw(gfx);

    grasses2->h_scroll(-2);
    grasses2->draw(gfx);

    cars->h_scroll(-6);
    cars->draw(gfx);

    grasses1->h_scroll(-5);
    grasses1->draw(gfx);

    birds->h_scroll(1, 480);
    birds->next_frame();
    birds->draw(gfx);

    clouds->h_scroll(1);
    clouds->draw(gfx);

    mpv->next_frame();
    mpv->draw(gfx);
  }
}

void loop()
{
  testingLoop();

  gfx->setCursor(2, 2);
  gfx->print(fSnapShot);

  gfx->flush();
  frame++;
}
