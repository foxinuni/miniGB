#pragma once

#include "common.h"

void audio_init();
void audio_play_tone(u32 freq_hz, const i16* samples, u32 count);
