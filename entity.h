#pragma once

#ifndef __ENTITY_H__
#define __ENTITY_H__

#define PLAYER_MAX 4
#define PLAYER_SIZE v2(0.3, 0.6)
#define ENEMY_MAX 16

typedef struct Player {
    int count;
    Box box[PLAYER_MAX];
} Player;

void player_create(Player *player, const v2 pos) {
    player->box[player->count] = box_create(pos, v2(0, 0), PLAYER_SIZE);
    player->count++;
}

typedef struct Slime {
    int count;
    Box box[ENEMY_MAX];
    int dead;
} Slime;

static void entity_render(Box *box, int count) {
    ce_set_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    for(int i = 0; i < count; ++i) {
        ce_push_box(box[i].pos[0], box[i].pos[1], 0, box[i].rad[0], box[i].rad[1], .3);
    }
}

#endif