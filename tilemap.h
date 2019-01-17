#pragma once

#ifndef __TILEMAP_H__
#define __TILEMAP_H__

#define TILEMAP_WIDTH   (512)
#define TILEMAP_HEIGHT  (512)

enum {
    TILE_NONE,
    TILE_WATER,
    TILE_GROUND,
    TILE_TREE,
    TILE_COUNT,
};

typedef struct Tile {
    int     type;
} Tile;

typedef struct Tilemap {
    Tile    tiles[TILEMAP_HEIGHT][TILEMAP_WIDTH];
} Tilemap;

static void tilemap_generate_random(Tilemap* map) {
    for (int y = 0; y < TILEMAP_HEIGHT; ++y) {
        for (int x = 0; x < TILEMAP_WIDTH; ++x)
            map->tiles[y][x].type = randi(TILE_WATER, TILE_COUNT);
    }
}

#endif

