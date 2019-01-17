#pragma once

#ifndef __GAME_H__
#define __GAME_H__

#include "camera.h"

typedef struct GameState {
    Camera      camera;
    Tilemap     map;
} GameState;

static void game_init(GameState* gs) {
    memset(gs, 0, sizeof *gs);

    tilemap_generate_random(&gs->map);
}

static void game_update(GameState* gs, float t) {
    camera_update(&gs->camera, v2(10.0f, 10.0f), t);
}

static void game_render(GameState* gs) {
    tilemap_render(&gs->map);
    ce_render_squares();
}

#endif

