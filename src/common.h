#pragma once

/*
 * Este archivo contine definiciones basicas que suelo
 * usar en distintos proyectos en C/C++.
 *
 * En general me gusta definir tipos customizados
 * similares a los de Rust por facilidad- C++ es
 * raro y no esta estandarizado los tama;os.
 */

#include <cstdint>
#include <cstddef>

// Configuracion Comun

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t usize;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef float f64;

#ifndef NULL
#define NULL 0
#endif
