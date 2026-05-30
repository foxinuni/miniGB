#pragma once

#include <SD.h>
#include <stddef.h>
#include "common.h"

typedef struct {
    File file;
    size_t size;
} sd_file_t;

void sd_init();

bool sd_open(sd_file_t *f, const char *path);
void sd_close(sd_file_t *f);

usize sd_read(sd_file_t *f, void *buffer, usize len);
bool sd_seek(sd_file_t *f, usize offset);

usize sd_size(sd_file_t *f);