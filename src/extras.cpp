#include "extras.h"

#include <Arduino.h>
#include "common.h"
#include "screen.h"

#define PIN_LIGHT 32 
#define BRIGHTNESS_MIN 0.10f
#define BRIGHTNESS_MAX 1.0f

f32 map_value_to_log(f32 t, f32 k) {
    return (expf(k * t) - 1.0f) / (expf(k) - 1.0f);
}

void update_brightess() {
    f32 light_level = analogRead(PIN_LIGHT) / 4095.0f;
    f32 brightness = min(max(map_value_to_log(light_level, 5), BRIGHTNESS_MIN), BRIGHTNESS_MAX);
    screen_brightness(brightness);
}

void extras_init() {
    pinMode(PIN_LIGHT, INPUT);
}

void extras_update() {
    update_brightess();
}