#pragma once

#ifndef __ENTITY_H__
#define __ENTITY_H__

#define PLAYER_MAX 4
#define ENEMY_MAX 16

typedef struct Player {
    int count;
    Box box[PLAYER_MAX];
} Player;

typedef struct Slime {
    int count;
    Box box[ENEMY_MAX];
    int dead;
} Slime;

#endif