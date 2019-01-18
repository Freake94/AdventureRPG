#pragma once

#ifndef __ENTITY_H__
#define __ENTITY_H__

#define PLAYER_MAX 4
#define PLAYER_SIZE v2(0.3, 0.3)
#define ENEMY_MAX 16

// ========================================================= PLAYER ================================================ //

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

// ========================================================= SLIME ================================================ //

typedef struct Slime {
    int count;
    //
    Box     box     [ENEMY_MAX];
    Health  health  [ENEMY_MAX];
} Slime;

static void slime_create(Slime* slime, const v2 pos) {
    int i = slime->count++;

    slime->box[i]    = box_create(pos, v2(0, 0), v2(0.3f, 0.3f));
    slime->health[i] = health_create(i);
}

static void slime_destroy(Slime* slime, int i) {
    --slime->count;
    slime->box[i]    = slime->box[slime->count];
    slime->health[i] = slime->health[slime->count];
}


#endif
