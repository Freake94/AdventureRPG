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

static void tilemap_render(const Tilemap* map) {
    for (int y = 0; y < TILEMAP_HEIGHT; ++y) {
        for (int x = 0; x < TILEMAP_WIDTH; ++x) {
            const Tile* tile = &map->tiles[y][x];

            switch (tile->type) {
                case TILE_WATER:  ce_set_color4f(0.0f, 0.1f, 1.0f, 1.0f); break;
                case TILE_GROUND: ce_set_color4f(0.0f, 1.0f, 0.0f, 1.0f); break;
                case TILE_TREE:   ce_set_color4f(0.0f, 0.5f, 0.0f, 1.0f); break;
            }

            ce_push_cube(x + 0.5f, y + 0.5f, 0.0f, 0.5f);
        }
    }
}

#endif

