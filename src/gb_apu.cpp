#include "gb_apu.h"

#include <minigb_apu.h>

#include "audio.h"

static struct minigb_apu_ctx apu_ctx;

#define TONE_MIN_HZ  80
#define TONE_MAX_HZ  3500

extern "C" uint8_t audio_read(const uint16_t addr)
{
	return minigb_apu_audio_read(&apu_ctx, addr);
}

extern "C" void audio_write(const uint16_t addr, const uint8_t val)
{
	minigb_apu_audio_write(&apu_ctx, addr, val);
}

void gb_apu_init()
{
	minigb_apu_audio_init(&apu_ctx);
}

/* DMG square/wave period: 1048576 / (2048 - freq_reg) Hz */
static u32 square_hz(const struct chan* c)
{
	if (!c->enabled || !c->powered || c->volume == 0)
		return 0;

	const u16 f = c->freq;
	if (f < 8 || f >= 2048)
		return 0;

	return 1048576u / (2048u - f);
}

static u32 noise_hz(const struct chan* c)
{
	if (!c->enabled || !c->powered || c->volume == 0)
		return 0;

	static const u32 lfsr_div[] = { 8, 16, 32, 48, 64, 80, 96, 112 };
	const u32 div = lfsr_div[c->noise.lfsr_div & 7];
	const u32 shift = c->freq & 0x0F;
	if (shift > 13)
		return 0;

	const u32 hz = 4194304u / (div << shift);
	if (hz < TONE_MIN_HZ || hz > TONE_MAX_HZ)
		return 200; /* low rattle when noise is active */

	return hz;
}

static u32 apu_pick_tone_hz()
{
	if ((apu_ctx.audio_mem[0xFF26 - AUDIO_ADDR_COMPENSATION] & 0x80) == 0)
		return 0;

	u32 best_hz = 0;
	u32 best_score = 0;

	for (int i = 0; i < 2; i++) {
		const u32 hz = square_hz(&apu_ctx.chans[i]);
		if (!hz)
			continue;

		const u32 score = apu_ctx.chans[i].volume;
		if (score >= best_score) {
			best_score = score;
			best_hz = hz;
		}
	}

	if (!best_hz) {
		const u32 hz = square_hz(&apu_ctx.chans[2]);
		if (hz)
			best_hz = hz;
	}

	if (!best_hz)
		best_hz = noise_hz(&apu_ctx.chans[3]);

	return best_hz;
}

void gb_apu_render_frame()
{
	static audio_sample_t samples[AUDIO_SAMPLES_TOTAL];
	minigb_apu_audio_callback(&apu_ctx, samples);
	audio_play_tone(apu_pick_tone_hz(), samples, AUDIO_SAMPLES_TOTAL);
}
