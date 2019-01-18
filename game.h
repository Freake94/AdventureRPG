#pragma once

#ifndef __GAME_H__
#define __GAME_H__

#include "camera.h"
#include "controls.h"

typedef struct GameState {
    Camera      camera;
    Tilemap     map;
    Player player;
    int input;
} GameState;

static void game_init(GameState* gs) {
    memset(gs, 0, sizeof *gs);
    
    tilemap_generate_random(&gs->map);
    
    player_create(&gs->player, v2(5, 5));
}

static void game_update(GameState* gs, float t) {
    camera_update(&gs->camera, gs->player.box[0].pos, t);
    gs->input = chk_button_inputs();
}

static void game_render(GameState* gs) {
    tilemap_render(&gs->map);
    
    box_render(gs->player.box, gs->player.count, v4(1.0f, 0.5f, 0.0f, 1.0));
    
    ce_render_squares();
}

#endif
