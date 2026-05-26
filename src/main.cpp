#include <Arduino.h>
#include "screen.h"

void setup() {
    Serial.begin(115200);
    screen_init();
}

void loop() {
    Serial.println("Hello World!");
    delay(500);
}
