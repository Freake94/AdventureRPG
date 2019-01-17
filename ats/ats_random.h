#pragma once

#include "ats_base.h"

// ============================================== RANDOM GENERATOR =========================================== //

typedef struct RndGen {
    uint32     x;
    uint32     y;
    uint32     z;
} RndGen;

static RndGen default_rnd_gen = { 123456789u, 362436069u, 521288629u };

static uint32 xorshf96(RndGen* g) {
    g->x  ^= g->x << 16;
    g->x  ^= g->x >> 5;
    g->x  ^= g->x << 1;

    uint32 t = g->x;

    g->x  = g->y;
    g->y  = g->z;
    g->z  = t ^ g->x ^ g->y;

    return g->z;
}

static int randi(int min, int max) {
    return min + xorshf96(&default_rnd_gen) % (max - min);
}

static float randf(float min, float max) {
    return min + ((float)xorshf96(&default_rnd_gen) / (float)0xFFFFFFFF) * (max - min); 
}


// ============================================== SSE RANDOM GENERATOR =========================================== //

/*
typedef union SSE_RndGen {
    struct {
        __m128i     x;
        __m128i     y;
        __m128i     z;
    };

    float xs[12];
} SSE_RndGen;

static SSE_RndGen sse_default_rnd_gen = {
    .xs = {
        123456789u, 362436069u, 521288629u, 232153213u,
        837458281u, 182474782u, 654324521u, 483810104u,
        850328273u, 148324273u, 543234534u, 582918338u
    }
};

static __m128i sse_xorshf96(SSE_RndGen* g) {
    g->x  = _mm_xor_si128(g->x, _mm_slli_si128(g->x, 16));
    g->x  = _mm_xor_si128(g->x, _mm_srli_si128(g->x, 5));
    g->x  = _mm_xor_si128(g->x, _mm_slli_si128(g->x, 1));

    __m128i t = g->x;

    g->x  = g->y;
    g->y  = g->z;
    g->z  = _mm_xor_si128(_mm_xor_si128(t, g->x), g->y);

    return g->z;
}

static __m128 randv(float min, float max) {
    __m128 l = _mm_set_ps1(min);
    __m128 h = _mm_set_ps1(max);
    __m128 m = _mm_set_ps1((float)0xFFFFFFFF);
    __m128 r = _mm_castsi128_ps(sse_xorshf96(&sse_default_rnd_gen));

    return _mm_add_ps(l, _mm_mul_ps(_mm_div_ps(r, m), _mm_sub_ps(h, l)));
}
*/

