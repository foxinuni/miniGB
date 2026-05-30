#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t audio_read(uint16_t addr);
void audio_write(uint16_t addr, uint8_t val);

#ifdef __cplusplus
}
#endif

void gb_apu_init();
void gb_apu_render_frame();
