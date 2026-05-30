#include "audio.h"
#include <Arduino.h>
#include <esp32-hal-ledc.h>

#define BUZZER_PIN 33
#define BUZZER_CHANNEL 0

#define TONE_MIN_HZ  80
#define TONE_MAX_HZ  3500
#define PEAK_SILENCE   400

void audio_init() {
    ledcSetup(BUZZER_CHANNEL, 440, 8);
    ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
}

static i32 sample_peak(const i16* data, u32 count) {
    i32 peak = 0;
    for (u32 i = 0; i < count; i++) {
        i32 a = abs((i32)data[i]);
        if (a > peak)
            peak = a;
    }
    return peak;
}

void audio_play_tone(u32 freq_hz, const i16* samples, u32 count) {
    if (freq_hz < TONE_MIN_HZ || freq_hz > TONE_MAX_HZ || sample_peak(samples, count) < PEAK_SILENCE) {
        ledcWriteTone(BUZZER_CHANNEL, 0);
        return;
    }

    ledcWriteTone(BUZZER_CHANNEL, freq_hz);
}
