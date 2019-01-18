#pragma once

#ifndef __ENTITY_H__
#define __ENTITY_H__

#define PLAYER_MAX 4
#define PLAYER_SIZE v2(0.3, 0.6)
#define ENEMY_MAX 16

typedef struct Player {
    int count;
    //
    Box     box     [PLAYER_MAX];
    Health  health  [PLAYER_MAX];
} Player;

static void player_create(Player* player, const v2 pos) {
    int i = player->count++;
    
    player->box[i]    = box_create(pos, v2(0, 0), PLAYER_SIZE);
    player->health[i] = health_create(i);
}

static void player_destroy(Player* player, int i) {
    --player->count;
    player->box[i]    = player->box[player->count];
    player->health[i] = player->health[player->count];
}

typedef struct Slime {
    int count;
    //
    Box     box     [ENEMY_MAX];
    Health  health  [ENEMY_MAX];
} Slime;

#endif
