#include "controls.h"

#include <Arduino.h>

#define JOYSTICK_THRESHOLD 0.75f
#define JOYSTICK_CORRECTION 128 

#define PIN_A 13
#define PIN_B 12
#define PIN_SELECT 26
#define PIN_HORIZONTAL 35
#define PIN_VERTICAL 34

void controls_init() {
    pinMode(PIN_A, INPUT);
    pinMode(PIN_A, INPUT);
    pinMode(PIN_SELECT, INPUT);
    pinMode(PIN_HORIZONTAL, INPUT);
    pinMode(PIN_VERTICAL, INPUT);
}

float read_joystick(u8 pin) {
    int raw = analogRead(pin);
    float normalized = (raw / 2047.5f) - 1.0f;

    if (normalized > -JOYSTICK_THRESHOLD && normalized < JOYSTICK_THRESHOLD) {
        return 0.0f;
    }
    
    return normalized;
}

void controls_poll(controls_t* controls) {
    if (!controls) return;

    controls->a      = digitalRead(PIN_A);
    controls->b      = digitalRead(PIN_B);
    controls->select = digitalRead(PIN_SELECT);

    float h = read_joystick(PIN_HORIZONTAL);
    float v = read_joystick(PIN_VERTICAL);

    controls->left  = h < -JOYSTICK_THRESHOLD;
    controls->right = h >  JOYSTICK_THRESHOLD;
    controls->up    = v < -JOYSTICK_THRESHOLD;
    controls->down  = v >  JOYSTICK_THRESHOLD;
}