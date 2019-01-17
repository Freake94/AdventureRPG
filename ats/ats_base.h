#pragma once

#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

typedef     unsigned int uint;

typedef     uint8_t     uint8;
typedef     uint16_t    uint16;
typedef     uint32_t    uint32;
typedef     uint64_t    uint64;

typedef     int8_t      int8;
typedef     int16_t     int16;
typedef     int32_t     int32;
typedef     int64_t     int64;

typedef     uint8_t     bool8;
typedef     uint16_t    bool16;
typedef     uint32_t    bool32;
typedef     uint64_t    bool64;

typedef     uint8_t     u8;
typedef     uint16_t    u16;
typedef     uint32_t    u32;
typedef     uint64_t    u64;

typedef     int8_t      i8;
typedef     int16_t     i16;
typedef     int32_t     i32;
typedef     int64_t     i64;

typedef     float       r32;
typedef     double      r64;

typedef float float2[2];
typedef float float3[3];
typedef float float4[4];

typedef float v2[2];
typedef float v3[3];
typedef float v4[4];

typedef float Color[4];

#define float2(x, y)        (float[2]) { (float)(x), (float)(y) }
#define float3(x, y, z)     (float[3]) { (float)(x), (float)(y), (float)(z) }
#define float4(x, y, z, w)  (float[4]) { (float)(x), (float)(y), (float)(z), (float)(w) }

#define v2(x, y)            (float[2]) { (float)(x), (float)(y) }
#define v3(x, y, z)         (float[3]) { (float)(x), (float)(y), (float)(z) }
#define v4(x, y, z, w)      (float[4]) { (float)(x), (float)(y), (float)(z), (float)(w) }

#define Color(r, g, b, a)   (float[4]) { (float)(r), (float)(g), (float)(b), (float)(a) }

typedef float m2[4];
typedef float m3[9];
typedef float m4[16];

