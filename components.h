#pragma once

#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

// TODO(AIDSTON): entity components:

/* BOX */

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

static void box_render(Box *box, int count, const v4 color) {
    ce_set_color4v(color);
    
    for(int i = 0; i < count; ++i)
        ce_push_box(box[i].pos[0], box[i].pos[1], 0, box[i].rad[0], box[i].rad[1], 0.3f);
}

/* Health */

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

// TODO(Blöch): Equipment System

// TODO(Blöch): Inventory System

#endif
