#include "screen.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "spi_coms.h"

#define TFT_WIDTH  240
#define TFT_HEIGHT 320
#define TFT_CS   5
#define TFT_DC   27
#define TFT_RST  4
#define TFT_BCK  22

#define NATIVE_WIDTH  160
#define NATIVE_HEIGHT 144

#define SCALE_NUM  3
#define SCALE_DEN  2
#define SCALED_WIDTH  ((NATIVE_WIDTH  * SCALE_NUM) / SCALE_DEN)   // 240
#define SCALED_HEIGHT ((NATIVE_HEIGHT * SCALE_NUM) / SCALE_DEN)   // 216

// After setRotation(1) the logical display is 320w x 240h
#define DISPLAY_W  TFT_HEIGHT   // 320
#define DISPLAY_H  TFT_WIDTH    // 240

#define OFFSET_X  ((DISPLAY_W - SCALED_WIDTH)  / 2)   // 40px
#define OFFSET_Y  ((DISPLAY_H - SCALED_HEIGHT) / 2)   // 12px

static Adafruit_ST7789 screen(spi_get_class(), TFT_CS, TFT_DC, TFT_RST);
static u16 scaled_row[SCALED_WIDTH];

void screen_init() {
    screen.init(TFT_WIDTH, TFT_HEIGHT);
    screen.setRotation(1);
    screen.fillScreen(ST77XX_WHITE);
    screen.setTextColor(ST77XX_BLACK);
    screen.setTextSize(2);
    screen.setCursor(0, 0);

    pinMode(TFT_BCK, OUTPUT);
    digitalWrite(TFT_BCK, HIGH);
}

void screen_brightness(f32 percent) {
    constexpr f32 MAX_BITS = 255.0f;
    analogWrite(TFT_BCK, (int)(MAX_BITS * percent));
}

void screen_draw_line(const u16* pixels, u8 line) {
    if (line >= NATIVE_HEIGHT) return;

    for (int sx = 0; sx < SCALED_WIDTH; sx++) {
        int nx = (sx * SCALE_DEN) / SCALE_NUM;
        scaled_row[sx] = pixels[nx];
    }

    const int scaled_y_start = (line * SCALE_NUM) / SCALE_DEN;
    const int scaled_y_end   = ((line + 1) * SCALE_NUM) / SCALE_DEN;

    screen.startWrite();
    for (int sy = scaled_y_start; sy < scaled_y_end; sy++) {
        screen.setAddrWindow(OFFSET_X, OFFSET_Y + sy, SCALED_WIDTH, 1);
        screen.writePixels(scaled_row, SCALED_WIDTH);
    }
    screen.endWrite();
}

void screen_next_frame() { }
