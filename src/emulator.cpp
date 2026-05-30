#include "emulator.h"

#include <Arduino.h>
#include <peanut.h>

#include "screen.h"
#include "controls.h"
#include "memory.h"
#include "audio.h"

#define NATIVE_WIDTH 160
#define NATIVE_HEIGHT 144

// Tipos rapidos utilizados por PeanutGB
typedef uint_fast8_t uf8;
typedef uint_fast16_t uf16;
typedef uint_fast32_t uf32;
typedef uint_fast64_t uf64;

// Contexto global del emulador

static gb_s emulator;

static u8* rom = nullptr;
static usize rom_size = 0;

static u8* cart_ram = nullptr;
static usize cart_ram_size = 0;

// Configuracion de la paleta de colores

static constexpr u16 PALETTE[4] =
{
    0xFFFF,
    0xC618,
    0x7BEF,
    0x0000
};

// Capa de traduccion interna del emulator

// ROM
u8 _emulator_rom_read(gb_s* gb, const uf32 address);
u8 _emulator_boot_rom_read(gb_s* gb, const uf16 address);

// RAM
u8 _emulator_cart_ram_read(gb_s *gb, const uf32 address);
void _emulator_cart_ram_write(gb_s *gb, const uf32 address, const u8 value);

// Audio
u8 _emulator_audio_read(u16 address);
void _emulator_audio_write(u16 address, u8 value);
void _emulator_audio_callback(void* ptr, u8* data, u32 length);

// Archivos
void _emulator_cart_file_read(const char* file_name, u8** buffer, const usize length);
void _emulator_cart_file_write(const char* file_name, u8** buffer, const usize length);

// Errores
void _emulator_handle_error(gb_s* gb, const gb_error_e error, u16 address);

// Rendering
void _emulator_screen_draw_line(gb_s* gb, const u8* pixels, uf8 line);

// Configuracion del emulador

bool emulator_init() {
    gb_init_error_e result = gb_init(
        &emulator,
        &_emulator_rom_read,
        &_emulator_cart_ram_read,
        &_emulator_cart_ram_write,
        &_emulator_handle_error,
        NULL
    );

	switch(result) {
	case GB_INIT_NO_ERROR:
		break;

	case GB_INIT_CARTRIDGE_UNSUPPORTED:
        Serial.println("The selected cartridge is unsupported.");
        return false;

	case GB_INIT_INVALID_CHECKSUM:
        Serial.println("The selected rom is corrupted (checksum fail).");
        return false;

	default:
        Serial.printf("Unexpected error found when initializing emulator (code %d).\n", result);
        return false;
	}

    gb_init_lcd(&emulator, &_emulator_screen_draw_line);
    gb_reset(&emulator);

    return true;
}

void emulator_reset() {
    gb_reset(&emulator);
}

void emulator_next_frame() {
    emulator.direct.joypad = 0xFF;

    controls_t controls;
    controls_poll(&controls);

    if (controls.a) {
        emulator.direct.joypad &= ~JOYPAD_A;
        
    }
    
    if (controls.b) {
        emulator.direct.joypad &= ~JOYPAD_B;
    }

    if (controls.select) {
        emulator.direct.joypad &= ~JOYPAD_START;
    }

    if (controls.left) {
        emulator.direct.joypad &= ~JOYPAD_LEFT;
    }

    if (controls.right) {
        emulator.direct.joypad &= ~JOYPAD_RIGHT;
    }
    
    if (controls.up) {
        emulator.direct.joypad &= ~JOYPAD_UP;
    }

    if (controls.down) {
        emulator.direct.joypad &= ~JOYPAD_DOWN;
    }

    gb_run_frame(&emulator);
}

bool emulator_load_rom(const char* rom_name) {
    sd_file_t rom_file;

    if (!sd_open(&rom_file, rom_name)) {
        Serial.println("Failed to find rom file");
        return false;
    }

    if (rom) {
        free(rom);
        rom = nullptr;
        rom_size = 0;
    }

    rom_size = sd_size(&rom_file);
    rom = (u8*) malloc(rom_size);
    
    if (!rom) {
        sd_close(&rom_file);
        Serial.println("Failed to allocate ROM");
        return false;
    }

    sd_read(&rom_file, rom, rom_size);
    sd_close(&rom_file);

    return true;
}

// ROM
u8 _emulator_rom_read(gb_s* gb, const uf32 address) {
    if (address >= rom_size)
        return 0xFF;

    return rom[address];
}

u8 _emulator_boot_rom_read(gb_s* gb, const uf16 address) {
    return 0xFF; // TO-DO: Implement boot room
}

// RAM
u8 _emulator_cart_ram_read(gb_s *gb, const uf32 address) {
    if (!cart_ram)
        return 0xFF;

    if (address >= cart_ram_size)
        return 0xFF;

    return cart_ram[address];
}

void _emulator_cart_ram_write(gb_s *gb, const uf32 address, const u8 value) {
    if (!cart_ram)
        return;

    if (address >= cart_ram_size)
        return;

    cart_ram[address] = value;
}

// Audio
u8 _emulator_audio_read(u16 address) {
    // TO-DO: Implement audio read
    return 0;
}

void _emulator_audio_write(u16 address, u8 value) {
    // TO-DO: Implement audio write
}

void _emulator_audio_callback(void* ptr, u8* data, u32 length) {
    audio_play_samples(data, length);
}

// Archivos
void _emulator_cart_file_read(const char* file_name, u8** buffer, const usize length) {
    // Se crea el buffer de destino
    *buffer = (u8*) malloc(length * sizeof(u8));
    if (!*buffer) {
        Serial.println("Failed to allocate memory for cart file.");
        return;
    }

    // Se asegura que la memoria inicie en 0
    memset(*buffer, 0, length);

    // TO-DO: Leer de la SD
}

void _emulator_cart_file_write(const char* file_name, u8** buffer, const usize length) {
    // TO-DO: Escribir a la SD
}

// Errores
void _emulator_handle_error(gb_s* gb, const gb_error_e error, u16 address) {
    Serial.printf("Un error inesperado ha ocurrido en la ejecucion (codigo: %d, direccion: 0x%X)\n", error, address);
    while (true) delay(50); // La ejecucion se detiene para siempre
}

// Rendering
void _emulator_screen_draw_line(gb_s* gb, const u8* pixels, uf8 line) {
    static u16 transformed_pixels[NATIVE_WIDTH];
    for (u8 x = 0; x < NATIVE_WIDTH; x++) {
        transformed_pixels[x] = PALETTE[pixels[x] & 0x03];
    }

    screen_draw_line(transformed_pixels, line);
}
