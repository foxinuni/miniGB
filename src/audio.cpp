#include "audio.h"
#include <Arduino.h>
#include <esp32-hal-ledc.h>

#define BUZZER_PIN 33
#define BUZZER_CHANNEL 0

void audio_init() {
    ledcSetup(BUZZER_CHANNEL, 440, 8);
    ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
}

void audio_play_samples(const u8* data, u32 length) {
    i32 avg = 0;

    for (i32 i = 0; i < length; i++) {
        avg += ((int)data[i] - 128);
    }

    avg /= length;

    if (abs(avg) < 2) {
        ledcWriteTone(BUZZER_CHANNEL, 0);
        return;
    }

    u32 freq = 200 + abs(avg) * 8;
    ledcWriteTone(BUZZER_CHANNEL, freq);
}