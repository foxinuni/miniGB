#pragma once

#include "common.h"

// Configuracion de la pantalla
void screen_init();
void screen_brightness(f32 percent);

// Renderizado
void screen_draw_line(const u16* pixels, u8 line);
void screen_next_frame();
