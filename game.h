#pragma once

#ifndef __GAME_H__
#define __GAME_H__

#include "camera.h"

typedef struct GameState {
    Camera      camera;
    // entities:
    Player      player;
    Slime       slime;
    // map:
    Tilemap     map;
} GameState;

static void game_init(GameState* gs) {
    memset(gs, 0, sizeof *gs);

    tilemap_generate_random(&gs->map);

    player_create(&gs->player, v2(5, 5));
    //
    slime_create(&gs->slime, v2(7, 7));
    slime_create(&gs->slime, v2(5, 7));
    slime_create(&gs->slime, v2(4, 4));
}

static void game_update(GameState* gs, float t) {
    camera_update(&gs->camera, gs->player.box[0].pos, t);
}

static void game_render(GameState* gs) {
    tilemap_render(&gs->map);

    box_render(gs->player.box, gs->player.count, v4(1.0f, 0.5f, 0.0f, 1.0));
    box_render(gs->slime.box, gs->slime.count, v4(0.0f, 1.0f, 1.0f, 1.0));

    ce_render_squares();
}

#endif
