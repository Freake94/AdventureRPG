#pragma once

#ifndef __ATS_COMPONENTS_H__
#define __ATS_COMPONENTS_H__

#include "ats_math.h"

// ====================================================== BOX 3D ================================================ //

typedef struct Box3Component {
    float   pos[3];
    float   vel[3];
    float   rad;
} Box3Component;

static Box3Component box3_component_create(const float pos[3], const float vel[3], float rad) {
    Box3Component box;
    //
    box.pos[0] = pos[0];
    box.pos[1] = pos[1];
    box.pos[2] = pos[2];
    //
    box.vel[0] = vel[0];
    box.vel[1] = vel[1];
    box.vel[2] = vel[2];
    //
    box.rad = rad;
    //
    return box;
}

static void box3_component_update(Box3Component* box, int count, float k, float t) {
    float fm = k * t;

    for (int i = 0; i < count; ++i) {
        Box3Component* b = &box[i];

        b->pos[0] += b->vel[0] * t;
        b->pos[1] += b->vel[1] * t;
        b->pos[2] += b->vel[2] * t;

        b->vel[0] -= b->vel[0] * fm;
        b->vel[1] -= b->vel[1] * fm;
        b->vel[2] -= b->vel[2] * fm;
    }
}

static void box3_component_move_to(Box3Component* box, int count, const v3 pos, float speed, float t) {
    float   mul     = speed * t;
    v3      move_to = {0};

    for (int i = 0; i < count; ++i) {
        Box3Component* b = &box[i];

        sns3v(move_to, pos, b->pos, mul);
        add3v(b->vel, b->vel, move_to);
    }
}

static void box3_component_apply_force(Box3Component* box, int count, const v3 force, float t) {
    for (int i = 0; i < count; ++i) {
        Box3Component* b = &box[i];

        b->vel[0] += force[0] * t;
        b->vel[1] += force[1] * t;
        b->vel[2] += force[2] * t;
    }
}

// ====================================================== BOX 2D ================================================ //

typedef struct BoxComponent {
    float       pos[2];
    float       vel[2];
    float       rad;
} BoxComponent;

static BoxComponent box_component_create(const v2 pos, float rad) {
    BoxComponent b;
    //
    b.pos[0] = pos[0];
    b.pos[1] = pos[1];
    b.vel[0] = 0.0f;
    b.vel[1] = 0.0f;
    b.rad = rad;
    //
    return b;
}

static bool box_component_intersect(const BoxComponent* e, const BoxComponent* other) {
    return dist_sq2v(e->pos, other->pos) < square(e->rad + other->rad);
}

static void box_component_apply(BoxComponent* e, const v2 force, float t) {
    adds2v(e->vel, e->vel, force, t);
}

static void box_component_friction(BoxComponent* e, float k, float t) {
    adds2v(e->vel, e->vel, e->vel, -k * t);
}

static void box_component_apply_push(BoxComponent* e, const v2 push) {
    add2v(e->vel, e->vel, push);
}

static void box_component_update(BoxComponent* box, int count, float k, float t) {
    float fm = k * t;

    for (int i = 0; i < count; ++i) {
        BoxComponent* b = &box[i];

        b->pos[0] += b->vel[0] * t;
        b->pos[1] += b->vel[1] * t;

        b->vel[0] -= b->vel[0] * fm;
        b->vel[1] -= b->vel[1] * fm;
    }
}

static void box_component_move_to(BoxComponent* box, int count, const v2 pos, float speed, float t) {
    float   mul     = speed * t;
    v2      move_to = {0};

    for (int i = 0; i < count; ++i) {
        BoxComponent* b = &box[i];

        sns2v(move_to, pos, b->pos, mul);
        add2v(b->vel, b->vel, move_to);
    }
}

static void box_component_move_to_if_in_range(BoxComponent* box, int count, const v2 pos, float speed, float range, float t) {
    float   mul         = speed * t;
    float   sq_range    = range * range;
    float   move_to[2]  = {0};

    for (int i = 0; i < count; ++i) {
        BoxComponent* b = &box[i];

        if (dist_sq2v(pos, b->pos) < sq_range) {
            sns2v(move_to, pos, b->pos, mul);
            add2v(b->vel, b->vel, move_to);
        }
    }
}

// ==================================================== ROTATE COMPONENT ============================================ //

typedef struct RotateComponent {
    float       pos;
    float       vel;
} RotateComponent;

static RotateComponent rotate_component_create(float pos) {
    RotateComponent r = {};
    r.pos  = pos;
    return r;
}

static void rotate_component_update(RotateComponent* rotate, int count, float k, float t) {
    for (int i = 0; i < count; ++i) {
        RotateComponent* r = &rotate[i];

        r->pos += r->vel * t;
        r->vel -= r->vel * t * k;
    }
}

// ==================================================== LINE COMPONENT ============================================ //

typedef struct LineComponent {
    float       a[2];
    float       b[2];
} LineComponent;

static LineComponent line_component_create(const float a[2], const float b[2]) {
    LineComponent line;
    copy2v(line.a, a);
    copy2v(line.b, b);
    return line;
}

static bool line_component_intersect_box(const LineComponent* line, const BoxComponent* box) {
    return segment_is_intersecting_circle2v(
            line->a,  line->b,
            box->pos, box->rad);
}

// ==================================================== HEALTH COMPONENT ============================================ //

typedef struct LifeComponent {
    float       val;
    float       max;
} LifeComponent;

static LifeComponent life_component_create(float max) {
    LifeComponent h;
    h.val = max;
    h.max = max;
    return h;
}

static void life_component_apply(LifeComponent* health, int count, float change, float t) {
    float mul = change * t;

    for (int i = 0; i < count; ++i) {
        LifeComponent* h = &health[i];

        h->val += mul;
        h->val = clampf(h->val, 0.0f, h->max);
    }
}

static void life_component_set(LifeComponent* health, int count, float v) {
    for (int i = 0; i < count; ++i) {
        LifeComponent* h = &health[i];
        h->val = clampf(v, 0.0f, h->max);
    }
}

// ================================================== FADE COMPONENT ============================================ //

typedef struct FadeComponent {
    float       color[4];
    float       start[4];
    float       end[4];
} FadeComponent; 

static FadeComponent fade_component_create(const float start[4], const float end[4]) {
    FadeComponent fade;
    //
    fade.color[0] = start[0];
    fade.color[1] = start[1];
    fade.color[2] = start[2];
    fade.color[3] = start[3];
    //
    fade.start[0] = start[0];
    fade.start[1] = start[1];
    fade.start[2] = start[2];
    fade.start[3] = start[3];
    //
    fade.end[0] = end[0];
    fade.end[1] = end[1];
    fade.end[2] = end[2];
    fade.end[3] = end[3];
    //
    return fade;
}

static void fade_component_apply_life(FadeComponent* fade, const LifeComponent* health, int count) {
    for (int i = 0; i < count; ++i) {
        const LifeComponent* h = &health[i];
        FadeComponent*       f = &fade[i];

        lerp4v(f->color, f->start, f->end, 1.0f - (h->val / h->max));
    }
}

// --------------------------------------------- POSITION ------------------------------------ //

typedef struct PosComponent {
    v2      current;
    v2      target;
} PosComponent;

static PosComponent pos_component_create(float x, float y) {
    PosComponent p;
    p.current[0] = x;
    p.current[1] = y;
    p.target[0]  = x;
    p.target[1]  = y;
    return p;
}

static void pos_component_update(PosComponent* pos, int count, float t) {
    for (int i = 0; i < count; ++i) {
        PosComponent* p = &pos[i];

        p->current[0] = lerpf(p->current[0], p->target[0], t);
        p->current[1] = lerpf(p->current[1], p->target[1], t);
    }
}

// ============================================ RENDER FUNCS ===================================================== //

#ifdef __ATS_CUBE_ENGINE_H__

static void pos_component_render(const PosComponent* pos, int count, float z, float rad, const Color color) {
    ce_set_color4v(color);

    for (int i = 0; i < count; ++i) {
        const PosComponent* p = &pos[i];

        ce_push_cube(p->current[0], p->current[1], z, rad);
    }
}

static void box_component_render(const BoxComponent* box_array, int count, float z, const v4 color) {
    ce_set_color4v(color);

    for (int i = 0; i < count; ++i) {
        const BoxComponent*     box     = &box_array[i];

        ce_push_cube(box->pos[0], box->pos[1], z, box->rad);
    }
}

static void box_component_rotate_render(const BoxComponent* box_array, const RotateComponent* rotate_array, int count, float z, const v4 color) {
    ce_set_color4v(color);

    for (int i = 0; i < count; ++i) {
        const BoxComponent*     box     = &box_array[i];
        const RotateComponent*  rotate  = &rotate_array[i];

        ce_push_cube_rot(box->pos[0], box->pos[1], z, box->rad, rotate->pos);
    }
}

static void box3_component_render(const Box3Component* box_array, int count, const v4 color) {
    ce_set_color4v(color);

    for (int i = 0; i < count; ++i) {
        const Box3Component* box = &box_array[i];

        ce_push_cube(box->pos[0], box->pos[1], box->pos[2], box->rad);
    }
}

static void box3_component_render_fade(const Box3Component* box_array, const FadeComponent* fade_array, int count) {
    for (int i = 0; i < count; ++i) {
        const Box3Component* box  = &box_array[i];
        const FadeComponent* fade = &fade_array[i];

        ce_set_color4v(fade->color);
        ce_push_cube(box->pos[0], box->pos[1], box->pos[2], box->rad);
    }
}

static LineComponent line_component_render(const LineComponent* line_array, int count, float z, float rad, const v4 color) {
    ce_set_color4v(color);

    for (int i = 0; i < count; ++i) {
        const LineComponent* line = &line_array[i];

        ce_push_line(
                line->a[0], line->a[1],
                line->b[0], line->b[1],
                z, rad);
    }
}

static LineComponent line_component_render_fade_life(
        const LineComponent*    line_array,
        const FadeComponent*    fade_array,
        const LifeComponent*    life_array,
        int count, float z, float rad) {
    for (int i = 0; i < count; ++i) {
        const LineComponent* line = &line_array[i];
        const FadeComponent* fade = &fade_array[i];
        const LifeComponent* life = &life_array[i];

        ce_set_color4v(fade->color);

        ce_push_line(
                line->a[0], line->a[1],
                line->b[0], line->b[1],
                z, life->val * rad);
    }
}


#endif

#endif

