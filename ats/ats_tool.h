#pragma once

#include "ats_base.h"

#ifndef alloca
#define alloca __builtin_alloca
#endif

#define setbit(n, b)    ((n) |=  (1 << (b)))
#define clrbit(n, b)    ((n) &= ~(1 << (b)))
#define chkbit(n, b)    (((n) & (1 << (b))) == (1 << (b)))

#define count_of(arr)   (sizeof (arr) / sizeof (arr[0]))

#define swap(T, a, b)   { T t = (a); (a) = (b); (b) = t; }

#define CLAMP_MAX(x, max) MIN(x, max)
#define CLAMP_MIN(x, min) MAX(x, min)
#define IS_POW2(x) (((x) != 0) && ((x) & ((x)-1)) == 0)
#define ALIGN_DOWN(n, a) ((n) & ~((a) - 1))
#define ALIGN_UP(n, a) ALIGN_DOWN((n) + (a) - 1, (a))
#define ALIGN_DOWN_PTR(p, a) ((void *)ALIGN_DOWN((uintptr_t)(p), (a)))
#define ALIGN_UP_PTR(p, a) ((void *)ALIGN_UP((uintptr_t)(p), (a)))

#include "ats_math.h"

// ==================================================== BUFFER ============================================= //

typedef struct BufHdr {
    size_t len;
    size_t cap;
    char buf[];
} BufHdr;

#define buf__hdr(b) ((BufHdr *)((char *)(b) - offsetof(BufHdr, buf)))

#define buf_len(b) ((b) ? buf__hdr(b)->len : 0)
#define buf_cap(b) ((b) ? buf__hdr(b)->cap : 0)
#define buf_end(b) ((b) + buf_len(b))
#define buf_sizeof(b) ((b) ? buf_len(b)*sizeof(*b) : 0)

#define buf_free(b) ((b) ? (free(buf__hdr(b)), (b) = NULL) : 0)
#define buf_fit(b, n) ((n) <= buf_cap(b) ? 0 : ((b) = buf__grow((b), (n), sizeof(*(b)))))
#define buf_push(b, ...) (buf_fit((b), 1 + buf_len(b)), (b)[buf__hdr(b)->len++] = (__VA_ARGS__))
#define buf_printf(b, ...) ((b) = buf__printf((b), __VA_ARGS__))
#define buf_clear(b) ((b) ? buf__hdr(b)->len = 0 : 0)

static void *buf__grow(const void *buf, size_t new_len, size_t elem_size) {
    assert(buf_cap(buf) <= (SIZE_MAX - 1) / 2);
    size_t new_cap = CLAMP_MIN(2*buf_cap(buf), MAX(new_len, 16));
    assert(new_len <= new_cap);
    assert(new_cap <= (SIZE_MAX - offsetof(BufHdr, buf))/elem_size);
    size_t new_size = offsetof(BufHdr, buf) + new_cap*elem_size;
    BufHdr *new_hdr;
    if (buf) {
        new_hdr = realloc(buf__hdr(buf), new_size);
    } else {
        new_hdr = malloc(new_size);
        new_hdr->len = 0;
    }
    new_hdr->cap = new_cap;
    return new_hdr->buf;
}

static char *buf__printf(char *buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    size_t cap = buf_cap(buf) - buf_len(buf);
    size_t n = 1 + vsnprintf(buf_end(buf), cap, fmt, args);
    va_end(args);
    if (n > cap) {
        buf_fit(buf, n + buf_len(buf));
        va_start(args, fmt);
        size_t new_cap = buf_cap(buf) - buf_len(buf);
        n = 1 + vsnprintf(buf_end(buf), new_cap, fmt, args);
        assert(n <= new_cap);
        va_end(args);
    }
    buf__hdr(buf)->len += n - 1;
    return buf;
}

// =========================================== ARENA ALLOCATOR ================================================= //

#define MEMORY_KB   (1000)
#define MEMORY_MB   ((1000) * (MEMORY_KB))
#define MEMORY_GB   ((1000) * (MEMORY_MB))

typedef struct ArenaAllocator {
    uint64      capacity;
    uint64      counter;
    //
    uint8*      memory;
} ArenaAllocator;

static void arena_allocator_create(ArenaAllocator* arena, uint64 capacity, void* memory) {
    memset(arena, 0, sizeof *arena);

    arena->capacity = capacity;

    if (memory) {
        arena->memory = (uint8*)memory;
    } else {
        arena->memory = (uint8*)malloc(capacity);
    }
}

static void arena_allocator_destroy(ArenaAllocator* arena) {
    free(arena->memory);
    memset(arena, 0,  sizeof *arena);
}

static void* arena_allocator_alloc(ArenaAllocator* arena, uint64 size) {
    uint8* memory = arena->memory + arena->counter;

    arena->counter += size;
    memset(memory, 0, size);

    return (void*)memory;
}

static void arena_allocator_clear(ArenaAllocator* arena) {
    arena->counter = 0;
}

// ============================================== POOL ALLOCATOR ================================================= //

typedef struct PoolAllocator {
    uint32          block_count;
    uint32          block_size;
    uint32          block_free_count;
    uint32          block_init_count;
    uint8*          memory;
    uint8*          next;
} PoolAllocator;

static void pool_allocator_create(PoolAllocator* pool, uint block_count, uint block_size, ArenaAllocator* arena) {
    memset(pool, 0, sizeof *pool);

    pool->block_count       = block_count;
    pool->block_size        = block_size;
    pool->block_free_count  = block_count;

    if (arena) {
        pool->memory = (uint8*)arena_allocator_alloc(arena, pool->block_count * pool->block_size);
    } else {
        pool->memory = (uint8*)malloc(pool->block_count * pool->block_size);
    }

    pool->next   = (uint8*)pool->memory;

    assert(pool->memory != NULL);
}

// @NOTE: only call this if the pool owns the used memory
static void pool_allocator_destroy(PoolAllocator* pool) {
    free(pool->memory);
    memset(pool, 0, sizeof *pool);
}

static void* pool_allocator_index_to_address(const PoolAllocator* pool, uint index) {
    return pool->memory + (pool->block_size * index);
}

static uint pool_allocator_address_to_index(const PoolAllocator* pool, uint8* address) {
    return (((uint)(address - pool->memory)) / pool->block_size);
}

static void* pool_allocator_alloc(PoolAllocator* pool) {
    if (pool->block_init_count < pool->block_count) {
        uint* p = (uint*)pool_allocator_index_to_address(pool, pool->block_init_count);
        *p = ++pool->block_init_count;
    }

    void* ret = NULL;

    if ((--pool->block_free_count) > 0) {
        ret = pool->next;

        if (pool->block_free_count != 0) {
            pool->next = (uint8*)pool_allocator_index_to_address(pool, *((uint*)pool->next));
        } else {
            pool->next = NULL;
        }

        memset(ret, 0, pool->block_size);
    }

    return ret;
}

static void pool_allocator_free(PoolAllocator* pool, void* ptr) {
    if (pool->next != NULL) {
        (*(uint*)ptr) = pool_allocator_address_to_index(pool, pool->next);
    } else {
        (*(uint*)ptr) = pool->block_count;
    }
    pool->next = (uint8*)ptr;
    ++pool->block_free_count;
}

// ================================================== FILE IO ============================================= //

static size_t file_get_size(FILE* fp) {
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}

static char* file_read_str(const char* file_name) {
    FILE*   fp      = NULL;
    char*   buffer  = NULL;
    
    if (fp = fopen(file_name, "rb")) {
        size_t size = file_get_size(fp);

        buffer = (char*)malloc(size + 1);

        if (buffer) {
            buffer[size] = 0;

            if (fread(buffer, 1, size, fp) == 0) {
                free(buffer);
                buffer = 0;
            }
        }

        fclose(fp);
    }

    return buffer;
}

static bool file_write_str(const char* file_name, const char* buffer) {
    FILE* fp = NULL;

    if (fp = fopen(file_name, "w")) {
        size_t size = strlen(buffer);
        size_t n    = fwrite(buffer, 1, size, fp);
        fclose(fp);
        return n == size;
    }

    return false;
}

static bool file_append_str(const char* file_name, const char* buffer) {
    FILE* fp = NULL;

    if (fp = fopen(file_name, "a")) {
        size_t size = strlen(buffer);
        size_t n    = fwrite(buffer, 1, size, fp);

        fclose(fp);

        return n == size;
    }

    return false;
}

static bool file_read_bin(const char* file_name, void* data, size_t size) {
    FILE* fp = NULL;

    if (fp = fopen(file_name, "rb")) {
        fread(data, size, 1, fp);
        fclose(fp);
        return true;
    }
    return false;
} 

static bool file_write_bin(const char* file_name, const void* data, size_t size) {
    FILE* fp = NULL;

    if (fp = fopen(file_name, "wb")) {
        fwrite(data, size, 1, fp);
        fclose(fp);
        return true;
    }
    return false;
}

// ================================================ FILE SYSTEM ====================================== //

#include "dirent.h"

// ================================================== TILEMAP ========================================= //

#ifdef ATS_TILEMAP

#ifndef ATS_TILEMAP_MAX_SIZE
#define ATS_TILEMAP_MAX_SIZE (1024 * 1024)
#endif

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h" 

enum Tile_Attribute {
    Tile_None       = 0,
    Tile_Exit_N     = (1 << 0),
    Tile_Exit_S     = (1 << 1),
    Tile_Exit_W     = (1 << 2),
    Tile_Exit_E     = (1 << 3),
    Tile_Room       = (1 << 4),
};

typedef struct Tilemap {
    int32     width;
    int32     height;
    int32     tiles[ATS_TILEMAP_MAX_SIZE];
} Tilemap;

static void tilemap_set_tile(Tilemap* tiles, int x, int y, int tile) {
    if (x < 0 || x >= tiles->width)  { return; }
    if (y < 0 || y >= tiles->height) { return; }

    tiles->tiles[y * tiles->width + x] = tile;
}

static int  tilemap_get_tile(const Tilemap* tiles, int x, int y) {
    if (x < 0 || x >= tiles->width)  { return 0; }
    if (y < 0 || y >= tiles->height) { return 0; }

    return tiles->tiles[y * tiles->width + x];
}

static void add_bit(Tilemap* tiles, int x, int y, int bit) {
    tiles->tiles[y * tiles->width + x] |= bit;
}

static void tilemap_init(Tilemap* tiles, int width, int height) {
    assert((width * height) < ATS_TILEMAP_MAX_SIZE);
    memset(tiles->tiles, 0, width * height * sizeof *tiles->tiles);
    tiles->width  = width;
    tiles->height = height;
}

static void tilemap_add_tile(Tilemap* tiles, int x, int y, int type, int tile_size) {
    int ht = tile_size / 2;
    if ((type & Tile_Exit_N) == Tile_Exit_N) {
        for (int i = 0; i <= tile_size; i++) {
            tilemap_set_tile(tiles, x + ht, y - i,  1);
        }
    }
    if ((type & Tile_Exit_S) == Tile_Exit_S) {
        for (int i = 0; i < tile_size; i++) {
            tilemap_set_tile(tiles, x + ht, y + i,  1);
        }
    }
    if ((type & Tile_Exit_W) == Tile_Exit_W) {
        for (int i = 0; i <= tile_size; i++) {
            tilemap_set_tile(tiles, x - i, y + ht, 1);
        }
    }
    if ((type & Tile_Exit_E) == Tile_Exit_E) {
        for (int i = 0; i < tile_size; i++) {
            tilemap_set_tile(tiles, x + i, y + ht, 1);
        }
    }
}

static void tilemap_rec_map_gen(Tilemap* tiles, Tilemap* gen_tiles, int x, int y, int current, int depth, int max_depth, int tile_size) {
    if (depth == max_depth)                     { return; }
    if (x < 0 || x > tiles->width - 1)  { return; }
    if (y < 0 || y > tiles->height - 1) { return; }
    
    if (tilemap_get_tile(gen_tiles, x / tile_size, y / tile_size) != Tile_None)  { return; }

    tilemap_add_tile(tiles, x, y, current, tile_size);
    add_bit(gen_tiles, x / tile_size, y / tile_size, current);

    if (xorshf96(&default_rnd_gen) % 2) {
        tilemap_add_tile(tiles, x, y, Tile_Exit_N, tile_size);
        add_bit(gen_tiles, x / tile_size, y / tile_size, Tile_Exit_N);
        tilemap_rec_map_gen(tiles, gen_tiles, x, y - tile_size, (Tile_Exit_S), depth + 1, max_depth, tile_size);
    }
    
    if (xorshf96(&default_rnd_gen) % 2) {
        tilemap_add_tile(tiles, x, y, Tile_Exit_S, tile_size);
        add_bit(gen_tiles, x / tile_size, y / tile_size, Tile_Exit_S);
        tilemap_rec_map_gen(tiles, gen_tiles, x, y + tile_size, (Tile_Exit_N), depth + 1, max_depth, tile_size);
    }
    
    if (xorshf96(&default_rnd_gen) % 2) {
        tilemap_add_tile(tiles, x, y, Tile_Exit_W, tile_size);
        add_bit(gen_tiles, x / tile_size, y / tile_size, Tile_Exit_W);
        tilemap_rec_map_gen(tiles, gen_tiles, x - tile_size, y, (Tile_Exit_E), depth + 1, max_depth, tile_size);
    }

    if (xorshf96(&default_rnd_gen) % 2) {
        tilemap_add_tile(tiles, x, y, Tile_Exit_E, tile_size);
        add_bit(gen_tiles, x / tile_size, y / tile_size, Tile_Exit_E);
        tilemap_rec_map_gen(tiles, gen_tiles, x + tile_size, y, (Tile_Exit_W), depth + 1, max_depth, tile_size);
    }

    // make room
    if (xorshf96(&default_rnd_gen)  % 3 == 0) {
        add_bit(gen_tiles, x / tile_size, y / tile_size, Tile_Room);
        for (int ty = 1; ty < tile_size - 1; ty++) {
            for (int tx = 1; tx < tile_size - 1; tx++) {
                tilemap_set_tile(tiles, x + tx, y + ty, 1);
            }
        }
    }
}

static void tilemap_generate(Tilemap* tiles, int width, int height, int max_depth, int tile_size) {
    static Tilemap gen_tiles = {0};

    tilemap_init(&gen_tiles, width / tile_size, height / tile_size);

    tilemap_init(tiles, width, height);

    tilemap_rec_map_gen(
                tiles,
                &gen_tiles,
                tiles->width / 2,
                tiles->height / 2,
                Tile_Exit_N | Tile_Exit_S | Tile_Exit_W | Tile_Exit_E,
                0,
                max_depth,
                tile_size);

    // showing map in console for debugging!!
    for (int y = 0; y < gen_tiles.height; y++) {
        for (int x = 0; x < gen_tiles.width; x++) {
            int tile = tilemap_get_tile(&gen_tiles, x, y);
            int  p = 0;

            if  ((tile & Tile_Exit_N) == Tile_Exit_N) p++;
            if  ((tile & Tile_Exit_S) == Tile_Exit_S) p++;
            if  ((tile & Tile_Exit_W) == Tile_Exit_W) p++;
            if  ((tile & Tile_Exit_E) == Tile_Exit_E) p++;
        }
    }
}

static void tilemap_generate_noise(Tilemap* tiles, int width, int height, float scale_x, float scale_y, float tresh) {
    tilemap_init(tiles, width, height);

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i)
            tilemap_set_tile(tiles, i, j, stb_perlin_noise3(scale_x * i, scale_y * j, 0, 0, 0, 0) < tresh);
    }
}

// =============================================== MAP COLLISION SYSTEM =================================== //

typedef union Collision {
    //
    struct {
        uint8    top     : 1;
        uint8    bot     : 1;
        uint8    left    : 1;
        uint8    right   : 1;
    };

    uint8 has_collision;
} Collision;

static Collision tilemap_get_collision(const Tilemap* tiles, const float* pos, float rad) {
    const int32* ts = tiles->tiles;

    uint width  = tiles->width;
    uint height = tiles->height;

    Collision col = {0};

    float x = pos[0];
    float y = pos[1];

    if ((x - rad) < 0)       { col.left  = true; }
    if ((y - rad) < 0)       { col.top   = true; }
    if ((x + rad) >= width)  { col.right = true; }
    if ((y + rad) >= height) { col.bot   = true; }

#define GET_TILE(X, Y) ts[(int)(Y) * tiles->width + (int)(X)]
    if (GET_TILE(x + rad - 0.2f, y - rad) || GET_TILE(x - rad + 0.2f, y - rad)) col.top = true;
    if (GET_TILE(x + rad - 0.2f, y + rad) || GET_TILE(x - rad + 0.2f, y + rad)) col.bot = true;
    if (GET_TILE(x - rad, y + rad - 0.2f) || GET_TILE(x - rad, y - rad + 0.2f)) col.left = true;
    if (GET_TILE(x + rad, y + rad - 0.2f) || GET_TILE(x + rad, y - rad + 0.2f)) col.right = true;
#undef GET_TILE

    return col;
}

#endif

