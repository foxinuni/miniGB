#include "screen.h"

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "spi_coms.h"

#define TFT_WIDTH 240
#define TFT_HEIGHT 320

#define TFT_CS   5
#define TFT_DC   27
#define TFT_RST  4
#define TFT_BCK  22
   
#define NATIVE_WIDTH 160
#define NATIVE_HEIGHT 144

static Adafruit_ST7789 screen(spi_get_class(), TFT_CS, TFT_DC, TFT_RST);

static u16 frame[NATIVE_WIDTH * NATIVE_HEIGHT];

void screen_init() {
    // Se configura la pantalla
    screen.init(TFT_WIDTH, TFT_HEIGHT);
    screen.setRotation(1);
    screen.fillScreen(ST77XX_WHITE);
    screen.setTextColor(ST77XX_BLACK);
    screen.setTextSize(2);
    screen.setCursor(0, 0);
    screen.println("Hello ESP32!");

    // Configuracion del pin de brillo
    pinMode(TFT_BCK, OUTPUT);
    digitalWrite(TFT_BCK, HIGH);
}

void screen_brightness(f32 percent) {
    constexpr f32 MAX_BITS = 255.0f;
    analogWrite(TFT_BCK, (int)(MAX_BITS * percent));
}

void screen_draw_line(const u16* pixels, u8 line) {
    if (line >= LINE_MAX) {
        return;
    }

    u16* row = &frame[line * NATIVE_WIDTH];
    for (int x = 0; x < NATIVE_WIDTH; x++) {
        row[x] = pixels[x];
    }
}

void screen_next_frame() {
    screen.drawRGBBitmap(0, 0, frame, NATIVE_WIDTH, NATIVE_HEIGHT);
}
