#include "sd.h"

#include <SD.h>
#include "spi.h"

#define SD_CS 13

void sd_init() {
    if (!SD.begin(SD_CS)) {
        Serial.println("Failed to initialize SD card!");
    }
}
