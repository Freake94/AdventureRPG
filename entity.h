#pragma once

#ifndef __ENTITY_H__
#define __ENTITY_H__

#define PLAYER_MAX 4
#define PLAYER_SIZE v2(0.3, 0.3)
#define ENEMY_MAX 16

// ========================================================= ENTITY ================================================ //

enum {
    ENTITY_NONE,
    ENTITY_PLAYER,
    ENTITY_SLIME,
    ENTITY_COUNT
};

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
    Brain   brain   [ENEMY_MAX];
} Slime;

static void slime_create(Slime* slime, const v2 pos) {
    int i = slime->count++;
    slime->box[i]    = box_create(pos, v2(0, 0), v2(0.3f, 0.3f));
    slime->health[i] = health_create(i);
    slime->brain[i]  = brain_create();
}

static void slime_destroy(Slime* slime, int i) {
    --slime->count;
    slime->box[i]    = slime->box[slime->count];
    slime->health[i] = slime->health[slime->count];
    slime->brain[i]  = slime->brain[slime->count];
}

static void slime_update(Slime* slime, float t, Player* player) {
    Box* player_box = &player->box[0];
    for(int i = 0; i < slime->count; i++) {
        Box* slime_box = &slime->box[i];
        
        float x = player_box->pos[0] - slime_box->pos[0];
        float y = player_box->pos[1] - slime_box->pos[1];
        slime_box->vel[0] = x;
        slime_box->vel[1] = y;
        ns2v(slime_box->vel, slime_box->vel, t * 400);
    }
}


#endif
