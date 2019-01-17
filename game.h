#pragma once

#ifndef __GAME_H__
#define __GAME_H__

typedef struct GameState {
    Tilemap     map;
} GameState;

static void game_init(GameState* gs) {
    tilemap_generate_random(&gs->map);
}

static void game_update(GameState* gs, float t) {
    // @TODO: update shit!
}

static void game_render(GameState* gs) {
    ce_set_view(
            8.0f, 8.0f, 16.0f,
            8.0f, 8.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
            60.0f, 0.1f, 64.0f);

    ce_set_light(0.0f, 0.0f, 16.0f, 1.0f, 1.0f, 1.0f);

    tilemap_render(&gs->map);

    ce_render_squares();
}

#endif

