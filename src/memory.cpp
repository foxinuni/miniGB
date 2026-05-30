#include "memory.h"

#include <Arduino.h>
#include "spi_coms.h"

#define SD_CS 14

void sd_init() {
    if (!SD.begin(SD_CS, *spi_get_class())) {
        Serial.println("Failed to initialize SD card!");
    }
}

bool sd_open(sd_file_t *f, const char *path) {
    f->file = SD.open(path, FILE_READ);

    if (!f->file)
        return false;

    f->size = f->file.size();
    return true;
}

void sd_close(sd_file_t *f) {
    if (f->file) {
        f->file.close();
    }
}

usize sd_read(sd_file_t *f, void *buffer, usize len) {
    return f->file.read((uint8_t *)buffer, len);
}

bool sd_seek(sd_file_t *f, usize offset) {
    return f->file.seek(offset);
}

usize sd_size(sd_file_t *f) {
    return f->size;
}