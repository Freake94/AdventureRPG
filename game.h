#pragma once

#ifndef __GAME_H__
#define __GAME_H__

#include "camera.h"

typedef struct GameState {
    Camera      camera;
    Tilemap     map;
    Player player;
} GameState;

static void game_init(GameState* gs) {
    memset(gs, 0, sizeof *gs);

    tilemap_generate_random(&gs->map);
    player_create(&gs->player, v2(5, 5));
}

static void game_update(GameState* gs, float t) {
    camera_update(&gs->camera, v2(10.0f, 10.0f), t);
}

static void game_render(GameState* gs) {
    ce_set_view(
        8.0f, 8.0f, 16.0f,
        8.0f, 8.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
        60.0f, 0.1f, 64.0f);

    ce_set_light(0.0f, 0.0f, 16.0f, 1.0f, 1.0f, 1.0f);

    tilemap_render(&gs->map);

    entity_render(gs->player.box, gs->player.count);

    ce_render_squares();
}

#endif
