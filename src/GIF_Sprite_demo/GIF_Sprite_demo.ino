/*******************************************************************************
 * GIF Sprite Demo
 * This is a simple GIF Sprite Demo example
 * Image Source: https://www.pexels.com/video/earth-rotating-video-856356/
 * cropped: x: 598 y: 178 width: 720 height: 720 resized: 240x240
 * optimized with ezgif.com
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
#define GFX_BL 14
Arduino_DataBus *bus = new Arduino_ESP32SPI(11 /* DC */, 10 /* CS */, 12 /* SCK */, 13 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_G *output_display = new Arduino_ST7789(bus, 1 /* RST */, 0 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */, 35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);
Arduino_Canvas_Indexed *gfx = new Arduino_Canvas_Indexed(170 /* width */, 320 /* height */, output_display);
// Arduino_GFX *gfx = new Arduino_Canvas(170 /* width */, 320 /* height */, output_display);
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
Indexed_Sprite *mountains1;
Indexed_Sprite *mountains2;
Indexed_Sprite *road;
Indexed_Sprite *buildings;
Indexed_Sprite *grasses1;
Indexed_Sprite *grasses2;
Indexed_Sprite *cars;
Indexed_Sprite *clouds;
Indexed_Sprite *sun;

void setup()
{
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Animated GIF Image Viewer");

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
          // uint16_t *palette = (uint16_t *)malloc(gif->palette->len * 2);
          memcpy(palette, gif->palette->colors, gif->palette->len * 2);

          background = new Indexed_Sprite(0, 0, spriteMaster, palette, 540, 320, false, 1);
          mountains2 = new Indexed_Sprite(0, 81, spriteMaster + (320 * 540), palette, 540, 124, true, 1, gif->gce.tindex);
          mountains1 = new Indexed_Sprite(0, 179, spriteMaster + (444 * 540), palette, 540, 32, true, 1, gif->gce.tindex);
          road = new Indexed_Sprite(0, 210, spriteMaster + (476 * 540), palette, 540, 108, true, 1);
          buildings = new Indexed_Sprite(0, 112, spriteMaster + (584 * 540), palette, 540, 128, true, 1, gif->gce.tindex);
          grasses2 = new Indexed_Sprite(0, 224, spriteMaster + (712 * 540), palette, 540, 21, true, 1, gif->gce.tindex);
          grasses1 = new Indexed_Sprite(0, 302, spriteMaster + (733 * 540), palette, 540, 20, true, 1, gif->gce.tindex);
          cars = new Indexed_Sprite(0, 260, spriteMaster + (753 * 540), palette, 540, 14, true, 1, gif->gce.tindex);
          sun = new Indexed_Sprite(0, 12, spriteMaster + (767 * 540), palette, 540, 40, true, 1, gif->gce.tindex);
          clouds = new Indexed_Sprite(0, 2, spriteMaster + (807 * 540), palette, 540, 124, true, 1, gif->gce.tindex);

          spriteInitiated = true;
        }

        gifClass.gd_close_gif(gif);
      }
    }
  }
}

bool otherFrame = false;
void testingLoop(void)
{
  if (spriteInitiated)
  {
    background->draw(gfx);

    mountains2->h_scroll(-1);
    mountains2->draw(gfx);

    mountains1->h_scroll(-2);
    mountains1->draw(gfx);

    road->h_scroll(-3);
    road->draw(gfx);

    buildings->h_scroll(-4);
    buildings->draw(gfx);

    grasses2->h_scroll(-5);
    grasses2->draw(gfx);

    grasses1->h_scroll(-6);
    grasses1->draw(gfx);

    cars->h_scroll(-7);
    cars->draw(gfx);

    sun->h_scroll(-8);
    sun->draw(gfx);

    clouds->h_scroll(-9);
    clouds->draw(gfx);
  }
}

void loop()
{
  unsigned long start = millis();
  testingLoop();
  // Serial.print(millis() - start);
  gfx->flush();
  // Serial.printf(", %d\n", millis() - start);
}
