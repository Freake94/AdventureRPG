#pragma once

#ifndef __TILEMAP_H__
#define __TILEMAP_H__

#define TILEMAP_WIDTH   (32)
#define TILEMAP_HEIGHT  (32)

#define TILEMAP_RENDER_SIZE (8)
#define TILEMAP_SIM_SIZE    (16)

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

#define COLLISION_OBJECT_MAX (16)

typedef struct CollisionObject {
    int         count;
    //
    int         type    [COLLISION_OBJECT_MAX];
    int         id      [COLLISION_OBJECT_MAX];
    Box*        box     [COLLISION_OBJECT_MAX];
    Health*     health  [COLLISION_OBJECT_MAX];
} CollisionObject;

static void collision_object_add(CollisionObject* collision_object, int type, int id, Box* box, Health* health) {
    int i = collision_object->count++;
    //
    collision_object->type[i]   = type;
    collision_object->id[i]     = id;
    collision_object->box[i]    = box;
    collision_object->health[i] = health;
}

typedef struct Tilemap {
    Tile    tiles[TILEMAP_HEIGHT][TILEMAP_WIDTH];
    //
    CollisionObject collision_object[TILEMAP_HEIGHT][TILEMAP_WIDTH];
} Tilemap;

static void tilemap_generate_random(Tilemap* map) {
    for (int y = 0; y < TILEMAP_HEIGHT; ++y) {
        for (int x = 0; x < TILEMAP_WIDTH; ++x)
            map->tiles[y][x].type = randi(TILE_WATER, TILE_COUNT);
    }
}

static void tilemap_render(const Tilemap* map, const v2 center) {
    int sx = clampi(center[0] - TILEMAP_RENDER_SIZE, 0, TILEMAP_WIDTH);
    int sy = clampi(center[1] - TILEMAP_RENDER_SIZE, 0, TILEMAP_HEIGHT);
    int ex = clampi(center[0] + TILEMAP_RENDER_SIZE, 0, TILEMAP_WIDTH);
    int ey = clampi(center[1] + TILEMAP_RENDER_SIZE, 0, TILEMAP_HEIGHT);

    for (int y = sy; y < ey; ++y) {
        for (int x = sx; x < ex; ++x) {
            const Tile* tile = &map->tiles[y][x];
            
            switch (tile->type) {
                case TILE_WATER:  ce_set_color4f(0.2f, 0.2f, 0.2f, 1.0f); break;
                case TILE_GROUND: ce_set_color4f(0.3f, 0.3f, 0.3f, 1.0f); break;
                case TILE_TREE:   ce_set_color4f(0.4f, 0.4f, 0.4f, 1.0f); break;
            }
            
            ce_push_cube(x + 0.5f, y + 0.5f, -0.01f, 0.5f);
        }
    }
}

static void tilemap_clear_simulation_area(Tilemap* map, const v2 center) {
    int sx = clampi(center[0] - TILEMAP_SIM_SIZE, 0, TILEMAP_WIDTH);
    int sy = clampi(center[1] - TILEMAP_SIM_SIZE, 0, TILEMAP_HEIGHT);
    int ex = clampi(center[0] + TILEMAP_SIM_SIZE, 0, TILEMAP_WIDTH);
    int ey = clampi(center[1] + TILEMAP_SIM_SIZE, 0, TILEMAP_HEIGHT);

    for (int y = sy; y < ey; ++y) {
        for (int x = sx; x < ex; ++x) {
            CollisionObject* co = &map->collision_object[y][x];
            co->count = 0;
        }
    }
}

#endif

