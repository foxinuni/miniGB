#include "screen.h"

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_WIDTH 240
#define TFT_HEIGHT 320

#define TFT_CS   5
#define TFT_DC   27
#define TFT_RST  4
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_MISO 19
#define TFT_LED  22
static SPIClass hspi(HSPI);
static Adafruit_ST7789 screen(&hspi, TFT_CS, TFT_DC, TFT_RST);

void screen_init() {
    pinMode(TFT_LED, OUTPUT);
    digitalWrite(TFT_LED, HIGH);

    hspi.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);

    screen.init(TFT_WIDTH, TFT_HEIGHT);
    screen.setRotation(0);
    screen.fillScreen(ST77XX_BLACK);
    screen.setTextColor(ST77XX_WHITE);
    screen.setTextSize(2);
    screen.setCursor(20, 140);
    screen.println("Hello ESP32!");
}
