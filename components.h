#pragma once

#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

// ======================================================  BOX ==================================================== //

typedef struct Box {
    v2      pos;
    v2      vel;
    v2      rad;
} Box;

static Box box_create(const v2 pos, const v2 vel, const v2 rad) {
    Box box;
    copy2v(box.pos, pos);
    copy2v(box.vel, vel);
    copy2v(box.rad, rad);
    return box;
}

static void box_render(Box* box, int count, const v4 color) {
    ce_set_color4v(color);

    for(int i = 0; i < count; ++i)
        ce_push_box(box[i].pos[0], box[i].pos[1], 0, box[i].rad[0], box[i].rad[1], 0.3f);
}

static void box_update(Box* box, int count, float friction, float t) {
    float k = -friction * t;

    for (int i = 0; i < count; ++i) {
        Box* b = &box[i];

        b->vel[0] += b->vel[0] * k;
        b->vel[1] += b->vel[1] * k;
        b->pos[0] += b->vel[0] * t;
        b->pos[1] += b->vel[1] * t;
    }
}

// ======================================================  HEALTH ==================================================== //

typedef struct Health {
    float   val;
    float   max;
    int     dead;
} Health;

static Health health_create(float val) {
    Health health;
    health.val = val;
    health.max = val;
    return health;
}

static void health_mark_dead(Health *health, int count) {
    for(int i = 0; i < count; ++i) {
        if(health[i].val <= 0) health[i].dead = 1;
    }
}

// ======================================================  EQUIPMENT ==================================================== //

typedef struct Equipment {
    // @TODO: do equipment shit:
} Equipment;

// ======================================================= INVENTORY ==================================================== //

typedef struct Inventory {
    // @TODO: do inventory shit:
} Inventory;

// ======================================================= BRAIN ==================================================== //

enum {
    BRAIN_IDLE,
    BRAIN_GO,
    BRAIN_ATTACK,
    BRAIN_COUNT
};

typedef struct Brain {
    int     state;
    v2      pos;
} Brain;

static Brain brain_create() {
    Brain brain = {0};
    brain.state = BRAIN_IDLE;
    return brain;
}

static void brain_update_state(Brain* brain, const Box* box, int count) {
    for (int i = 0; i < count; ++i) {
        //
    }
}

#endif
