#pragma once

#ifndef __GAME_H__
#define __GAME_H__

#include "camera.h"
#include "controls.h"

typedef struct GameState {
    Camera      camera;
    // entities:
    Player      player;
    Slime       slime;
    // map:
    Tilemap     map;
    int input;
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
    Box* player_box = &gs->player.box[0];

    gs->input = chk_button_inputs();

    camera_update(&gs->camera, player_box->pos, t);

    // handle player controlls:
    {
        if (chkbit(gs->input, BUTTON_MOVE_N)) player_box->vel[1] += 4.0f * t;
        if (chkbit(gs->input, BUTTON_MOVE_S)) player_box->vel[1] -= 4.0f * t;
        if (chkbit(gs->input, BUTTON_MOVE_W)) player_box->vel[0] -= 4.0f * t;
        if (chkbit(gs->input, BUTTON_MOVE_E)) player_box->vel[0] += 4.0f * t;
    }

    box_update(gs->player.box, gs->player.count, 4.0f, t);
    box_update(gs->slime.box, gs->player.count, 4.0f, t);
}

static void game_render(GameState* gs) {
    tilemap_render(&gs->map, gs->player.box[0].pos);

    box_render(gs->player.box, gs->player.count, v4(1.0f, 0.5f, 0.0f, 1.0));
    box_render(gs->slime.box, gs->slime.count, v4(0.0f, 1.0f, 1.0f, 1.0));

    ce_render_squares();
}

#endif
