#include <Arduino.h>

#include "spi_coms.h"
#include "memory.h"
#include "screen.h"
#include "emulator.h"

void setup() {
    // Se inicia la comunicacion serial
    Serial.begin(115200);
    Serial.println("Initializing miniGB...");

    // Se inicializa los diferentes componentes
    spi_init();
    sd_init();
    screen_init();

    // Se inicializa el emulador
    if (!emulator_init()) {
        Serial.println("Failed to initialize emulator! Halting...");
        while (true) delay(100);
    }

    Serial.println("Done!");
}

void loop() {
    emulator_next_frame();
    screen_next_frame();
}
