#pragma once

#ifndef __TILEMAP_H__
#define __TILEMAP_H__

#define TILEMAP_WIDTH   (512)
#define TILEMAP_HEIGHT  (512)

typedef struct Tile {
    int     type;
} Tile;

typedef struct Tilemap {
    Tile    tiles[TILEMAP_HEIGHT][TILEMAP_WIDTH];
} Tilemap;

#endif

