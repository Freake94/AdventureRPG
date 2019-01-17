#pragma once

#ifndef __ATS_CUBE_ENGINE_H__
#define __ATS_CUBE_ENGINE_H__

#define OPENGL_MODERN
#include "ats_platform_glfw.h"

#include "ats_shaders.h"
#include <string.h>

typedef struct Vertex {
    float   position[4];
    float   normal[4];
    float   color[4];
} Vertex;

static Vertex vertex_create(float px, float py, float pz, float nx, float ny, float nz, float r, float g, float b, float a) {
    Vertex vertex = {
        px, py, pz, 1.0f,
        nx, ny, nz, 0.0f,
        r, g, b, a
    };
    return vertex;
}

// Layout:  vertex : normal : color
static const Vertex vertex_array_cube[] = {
    // box:
    -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 0.0f, -1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 0.0f, -1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, +1.0f, -1.0f, 1.0f,    0.0f, 0.0f, -1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, +1.0f, -1.0f, 1.0f,    0.0f, 0.0f, -1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, +1.0f, -1.0f, 1.0f,    0.0f, 0.0f, -1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 0.0f, -1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,

    -1.0f, -1.0f, +1.0f, 1.0f,    0.0f, 0.0f, +1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f, +1.0f, 1.0f,    0.0f, 0.0f, +1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, +1.0f, +1.0f, 1.0f,    0.0f, 0.0f, +1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, +1.0f, +1.0f, 1.0f,    0.0f, 0.0f, +1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, +1.0f, +1.0f, 1.0f,    0.0f, 0.0f, +1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f, +1.0f, 1.0f,    0.0f, 0.0f, +1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,

    -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f, -1.0f, 1.0f,    0.0f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, +1.0f, 1.0f,    0.0f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f, +1.0f, 1.0f,    0.0f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, +1.0f, 1.0f,    0.0f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f, -1.0f, 1.0f,    0.0f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,

    -1.0f, +1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, +1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, +1.0f, +1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, +1.0f, +1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, +1.0f, +1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, +1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,

    -1.0f, -1.0f, -1.0f, 1.0f,    -1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, +1.0f, -1.0f, 1.0f,    -1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, +1.0f, 1.0f,    -1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, +1.0f, +1.0f, 1.0f,    -1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, +1.0f, 1.0f,    -1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, +1.0f, -1.0f, 1.0f,    -1.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,

    +1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, +1.0f, -1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f, +1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, +1.0f, +1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f, +1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, +1.0f, -1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
};

// Layout:  vertex : normal
static const Vertex vertex_array_square[] = {
    // box:
    -1.0f, -1.0f, +0.0f, 1.0f,    0.0f, 0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f, +0.0f, 1.0f,    0.0f, 0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, +1.0f, +0.0f, 1.0f,    0.0f, 0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, +1.0f, +0.0f, 1.0f,    0.0f, 0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, +1.0f, +0.0f, 1.0f,    0.0f, 0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f, +0.0f, 1.0f,    0.0f, 0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
};

static const char* ce_shader_vertex_light =
"#version 330 core\n"
"layout (location = 0) in vec4 vp;"
"layout (location = 1) in vec4 np;"
"layout (location = 2) in vec4 vc;"
"layout (location = 4) in vec4 C;"
"layout (location = 5) in mat4 M;"
"out vec3 frag_pos;"
"out vec3 normal;"
"out vec4 color;"
"uniform mat4 PV;"
"void main() {"
    "vec4 mpos      = M * vp;"
    "gl_Position    = PV * mpos;"
    "frag_pos       = mpos.xyz;"
    "normal         = normalize((M * np).xyz);"
    "color          = C * vc;"
"}";

static const char* ce_shader_fragment_light =
"#version 330 core\n"
"out vec4 frag_color;"
"in  vec3 normal;"
"in  vec3 frag_pos;"
"in  vec4 color;"
"in  vec2 tex_coord;"
"uniform vec3 light_pos;"
"uniform vec3 light_color;"
"uniform vec3 view_pos;"
"void main() {"
    "float   ambient_str   = 0.2;"
    "vec3    ambient       = ambient_str * light_color;"
    "vec3    light_dir     = normalize(light_pos - frag_pos);"
    "float   diff          = max(dot(normal, light_dir), 0.0);"
    "vec3    diffuse       = diff * light_color;"
    "float   specular_str  = 0.1;"
    "vec3    view_dir      = normalize(view_pos - frag_pos);"
    "vec3    reflect_dir   = reflect(-light_dir, normal);"
    "float   spec          = pow(max(dot(view_dir, reflect_dir), 0.0), 2);"
    "vec3    specular      = specular_str * spec * light_color;"
    "vec3    result        = (ambient + diffuse + specular) * vec3(color);"
    "frag_color            = vec4(result, color.w);"
"}";

static int ce_shader;

static int ce_vao_cube;
static int ce_vbo_cube;
static int ce_vbo_cube_data;

static int ce_location_PV;
static int ce_location_light_pos;
static int ce_location_light_color;
static int ce_location_view_pos;

typedef struct RObject {
    float      color[4];
    float      model[16];
} RObject;

#define robject(c, m) (RObject) { (c), (m) }

#define RQUEUE_MAX_SIZE (1000000)

typedef struct RQueue {
    int     count;
    RObject queue[RQUEUE_MAX_SIZE];
} RQueue;

static float ce_current_color[4];
static RQueue render_queue;

static void ce_set_color4f(float r, float g, float b, float a) {
    ce_current_color[0] = r;
    ce_current_color[1] = g;
    ce_current_color[2] = b;
    ce_current_color[3] = a;
}

static void ce_set_color4v(const float* color) {
    ce_current_color[0] = color[0];
    ce_current_color[1] = color[1];
    ce_current_color[2] = color[2];
    ce_current_color[3] = color[3];
}

static void ce_set_color_lerp4v(const float* start, const float* end, float t) {
    ce_current_color[0] = lerpf(start[0], end[0], t);
    ce_current_color[1] = lerpf(start[1], end[1], t);
    ce_current_color[2] = lerpf(start[2], end[2], t);
    ce_current_color[3] = lerpf(start[3], end[3], t);
}

static RObject* ce_rqueue_new_robject() {
    RObject* robj = &render_queue.queue[render_queue.count++];
    return robj;
}

static void ce_rqueue_push(const RObject* obj) {
    render_queue.queue[render_queue.count++] = *obj;
}

static void ce_set_light(float x, float y, float z, float r, float g, float b) {
    glUniform3f(ce_location_light_color, r, g, b);
    glUniform3f(ce_location_light_pos, x, y, z);
}

static void ce_set_PV(const float* PV) {
    glUniformMatrix4fv(ce_location_PV, 1, GL_FALSE, PV);
}

static void ce_set_view_pos(const float* pos) {
    glUniform3fv(ce_location_view_pos, 1, (float*)pos);
}

static void ce_set_view(
        float px,   float py,   float pz, 
        float lx,   float ly,   float lz, 
        float ux,   float uy,   float uz, 
        float fov, float n, float f) {
    float pos[3]  = { px, py, pz };
    float look[3] = { lx, ly, lz };
    float up[3]   = { ux, uy, uz };
    //
    float PV[16];
    mat4_perspective_view(PV, pos, look, up, fov, platform.aspect_ratio, n, f);
    ce_set_PV(PV);
    ce_set_view_pos(pos);
}

static void ce_set_view_2d(
        float px,   float py,   float pz, 
        float lx,   float ly,   float lz, 
        float ux,   float uy,   float uz,
        float rx,   float ry,
        float n, float f) {
    float pos[3]  = { px, py, pz };
    float look[3] = { lx, ly, lz };
    float up[3]   = { ux, uy, uz };
    float rad[2]  = { rx, ry };
    float PV[16];

    mat4_ortho_view(PV, pos, look, up, rad, n, f);
    ce_set_PV(PV);
    ce_set_view_pos(pos);
}

static void ce_set_vertex_array(const Vertex* vertex_array, size_t vertex_count) {
    glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof *vertex_array, vertex_array, GL_STATIC_DRAW);
}

static void ce_set_robject_array(const RObject* robj_array, size_t robj_count) {
    glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube_data);
    glBufferData(GL_ARRAY_BUFFER, robj_count * sizeof *robj_array, robj_array, GL_STATIC_DRAW);
}

static void ce_render(uint type, const Vertex* vertex_array, size_t vertex_count, const RObject* robj_array, size_t robj_count) {
    ce_set_vertex_array(vertex_array, vertex_count);
    ce_set_robject_array(robj_array, robj_count);

    glDrawArraysInstanced(type, 0, vertex_count, robj_count);
}

static void ce_render_cubes() {
    ce_render(GL_TRIANGLES, vertex_array_cube, 36, render_queue.queue, render_queue.count);
    render_queue.count = 0;
}

static void ce_render_squares() {
    ce_render(GL_TRIANGLES, vertex_array_square, 6, render_queue.queue, render_queue.count);
    render_queue.count = 0;
}

static void ce_push_cube(float x, float y, float z, float scale) {
    RObject robj = {
        // model:
        .model = {
            scale, 0, 0, 0,
            0,  scale, 0, 0,
            0, 0, scale, 0,
            x, y, z, 1
        }
    };

    copy4v(robj.color, ce_current_color);
    render_queue.queue[render_queue.count++] = robj;
}

static void ce_push_cube_quat(float x, float y, float z, float scale, const float* quat) {
    RObject robj = {
        // model:
        .model = {
            scale, 0, 0, 0,
            0, scale, 0, 0,
            0, 0, scale, 0,
            x, y, z, 1
        }
    };

    copy4v(robj.color, ce_current_color);

    float R[16];
    mat4_from_quat(R, quat);
    mat4_mul(robj.model, robj.model, R);
    //
    render_queue.queue[render_queue.count++] = robj;
}

static void ce_push_box(float x, float y, float z, float sx, float sy, float sz) {
    RObject robj = {
        // model:
        .model = {
            sx, 0, 0, 0,
            0,  sy, 0, 0,
            0, 0, sz, 0,
            x, y, z, 1
        }
    };

    copy4v(robj.color, ce_current_color);
    render_queue.queue[render_queue.count++] = robj;
}

static void ce_push_cube_rot(float x, float y, float z, float scale, float rot) {
    float s = __builtin_sinf(rot);
    float c = __builtin_cosf(rot);

    RObject robj = {
        // model:
        .model = {
            c * scale, s * scale, 0, 0,
            -s * scale, c * scale, 0, 0,
            0, 0, scale, 0,
            x, y, z, 1
        }
    };

    copy4v(robj.color, ce_current_color);
    render_queue.queue[render_queue.count++] = robj;
}

static void ce_push_box_rot(float x, float y, float z, float sx, float sy, float sz, float rot) {
    float s = sinf(rot);
    float c = cosf(rot);

    RObject robj = {
        // model:
        .model = {
            c * sx, s * sx, 0, 0,
            -s * sy, c * sy, 0, 0,
            0, 0, sz, 0,
            x, y, z, 1
        }
    };

    copy4v(robj.color, ce_current_color);
    render_queue.queue[render_queue.count++] = robj;
}

static void ce_push_line(float x0, float y0, float x1, float y1, float z, float rad) {
    float p0[2] = { x0, y0 };
    float p1[2] = { x1, y1 };

    float line[2];
    sub2v(line, p1, p0);

    float line_length = len2v(line);

    float line_pos[2];
    adds2v(line_pos, p0, line, 0.5f);

    float axis[2] = { 0.0f, -1.0f };
    float rot = get_angle2v(axis, line);

    ce_push_box_rot(
            line_pos[0], line_pos[1], z,
            rad, 0.5f * line_length, rad,
            rot);
}

static void ce_init(const char* vs, const char* fs) {
    ce_shader = shader_load_from_memory(
            vs? vs : ce_shader_vertex_light,
            fs? fs : ce_shader_fragment_light);

    // init opengl:
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
        glClearDepth(1.0f);
        glDepthFunc(GL_LESS);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        glAlphaFunc(GL_GREATER, 0.0);
        glEnable(GL_ALPHA_TEST);
    }

    shader_use(ce_shader);

    // setup vbo/vao:
    {
        glGenVertexArrays(1, &ce_vao_cube);
        glGenBuffers(1, &ce_vbo_cube);
        glGenBuffers(1, &ce_vbo_cube_data);

        glBindVertexArray(ce_vao_cube);

        // cube:
        {
            glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)(0));
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)(1 * sizeof (float[4])));
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)(2 * sizeof (float[4])));
        }

        // cube models:
        {
            glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube_data);

            // color:
            glEnableVertexAttribArray(4);

            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof (RObject), (void*)(0));
            glVertexAttribDivisor(4, 1);

            // model:
            glEnableVertexAttribArray(5);
            glEnableVertexAttribArray(6);
            glEnableVertexAttribArray(7);
            glEnableVertexAttribArray(8);

            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof (RObject), (void*)(1 * sizeof (float[4])));
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof (RObject), (void*)(2 * sizeof (float[4])));
            glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof (RObject), (void*)(3 * sizeof (float[4])));
            glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof (RObject), (void*)(4 * sizeof (float[4])));

            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);
            glVertexAttribDivisor(7, 1);
            glVertexAttribDivisor(8, 1);
        }
    }

    ce_location_PV          = shader_get_location(ce_shader, "PV");
    ce_location_light_pos   = shader_get_location(ce_shader, "light_pos");
    ce_location_light_color = shader_get_location(ce_shader, "light_color");
    ce_location_view_pos    = shader_get_location(ce_shader, "view_pos");
}

// =============================================== RENDER STATE ================================================= //

// TODO: nice idea! but needs a redesign!!
#if 0
typedef struct RState {
    Vertex*     vb;
    RObject*    rb;
} RState;

static RState* rs_create() {
    return calloc(1, sizeof (RState));
}

static void rs_destroy(RState* rs) {
    free(rs);
}

static void rs_push_vertex(RState* rs, float px, float py, float pz, float nx, float ny, float nz, float r, float g, float b, float a) {
    buf_push(rs->vb, vertex_create(px, py, pz, nx, ny, nz, r, g, b, a));
}

static void rs_push_model(RState* rs, const v4* color, const m4* model) {
    buf_push(rs->rb, robject(*color, *model));
}

static void rs_render(RState* rs, uint type) {
    ce_render(type, rs->vb, buf_len(rs->vb), rs->rb, buf_len(rs->rb));
}
#endif

// ===================================================== CE BITMAP ================================================== //

// ascii bitmaps from ' ' -> '~'
static const uint64 bitascii[128] = {
    0x000000000000000000,   // ' '
    0x000008000808080800,   // '!'
    0x000000000014140000,
    0x0000247e24247e2400,
    0x00083c42201c423c08,
    0x002152240810244a84,
    0x000c52225408141408,
    0x000000000000080800,
    0x000010080808081000,
    0x000008101010100800,
    0x000000000024182400,
    0x000008083e08080000,
    0x000004080000000000,
    0x000000007e00000000,
    0x000004000000000000,
    0x000002040810204000,
    0x000018244242241800,
    0x00001c0808080c0800,
    0x00007e041820423c00,
    0x00003c421820423c00,
    0x0000207e2428302000,
    0x00003c42403e027e00,
    0x00003c42423e023c00,
    0x000002040810207e00,
    0x00003c42423c423c00,
    0x00003c407c42423c00,
    0x000008000008000000,
    0x000004080000080000,
    0x000060180618600000,
    0x0000007e007e000000,
    0x000006186018060000,
    0x000004000c30423c00,
    0x003c02724a7a423c00,
    0x000042423c24181800,
    0x00003e42423e423e00,
    0x00003c420202423c00,
    0x00003e424242423e00,
    0x00007e02023e027e00,
    0x00000202023e027e00,
    0x00003c427a02423c00,
    0x00004242427e424200,
    0x00003e080808083e00,
    0x00003c424040407c00,
    0x000022120e0a122200,
    0x00007e020202020200,
    0x000041414955634100,
    0x00004262524a464200,
    0x00003c424242423c00,
    0x00000202023e423e00,
    0x00403c524242423c00,
    0x000042120a3a423e00,
    0x00003c423804423c00,
    0x000008080808087f00,
    0x00003c424242424200,
    0x000018242442424200,
    0x000041635549414100,
    0x000042241818244200,
    0x000008080814224100,
    0x00007e201008047e00,
    0x000038080808083800,
    0x000040201008040200,
    0x00001c101010101c00,
    0x000000000000140800,
    0x00007e000000000000,
    0x000000000000100800,
    0x00005c627c403c0000,
    0x00003a46423e020200,
    0x00003c02023c000000,
    0x00005c62427c404000,
    0x00007c027e423c0000,
    0x00000808081c081800,
    0x003840584464584000,
    0x000024241c04040400,
    0x000008080808000800,
    0x000e10101018001000,
    0x0000340c1424040400,
    0x000018080808080800,
    0x000042425a66000000,
    0x00002222263a000000,
    0x00003c42423c000000,
    0x0002023e42463a0000,
    0x0040407c42625c0000,
    0x00000202463a000000,
    0x00001c201804380000,
    0x00001808083c080000,
    0x00005c624242000000,
    0x000018244242000000,
    0x0000665a8181000000,
    0x000066182442000000,
    0x000608102844420000,
    0x00007e08107e000000,
    0x000030101818103000,
    0x000808080808080800,
    0x00000c081818080c00,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000
};

#define ce_getbit(N, X, Y) (((uint64)(N)) & ((uint64)1 << (((uint64)(Y)) * 8 + ((uint64)(X)))))

static void ce_bitmap_render_rect(float x, float y, float z, float sx, float sy) {
    ce_push_box(x, y, z, 0.5f * sx, 0.5f * sy, 0.01f);
}

// @TODO: make less shit!!
static void ce_push_ascii(char c, float px, float py, float pz, float x_scale, float y_scale) {
    if (c >= ' ' && c <= '~') {
        uint64 n = bitascii[c - ' '];
        for (uint32 j = 0; j < 8; ++j) {
            for (uint32 i = 0; i < 8; ++i) {
                if (ce_getbit(n, i, j) > 0)
                    ce_bitmap_render_rect(px + i * x_scale, py - j * y_scale, pz, x_scale, -y_scale);
            }
        }
    }
}

static void ce_push_string(const char* str, float x, float y, float z, float scale_x, float scale_y) {
    for (int32 i = 0; str[i] != '\0'; i++)
        ce_push_ascii(str[i], x + i * 8 * scale_x, y, z, scale_x, scale_y);
}

static void ce_push_string_format(float x, float y, float rad, const char* fmt, ...) {
    va_list list;
    char    buffer[64];

    va_start(list, fmt);
    vsnprintf(buffer, count_of(buffer), fmt, list);
    ce_push_string(buffer, x, y, 0.0f, rad, rad);
    va_end(list);
}


static void ce_push_int32(int32 n, float x, float y, float z, float scale_x, float scale_y) {
    static char buffer[32];
    sprintf(buffer, "%d", n);
    ce_push_string(buffer, x, y, z, scale_x, scale_y);
}

static void ce_push_float(float n, float x, float y, float z, float scale_x, float scale_y) {
    static char buffer[32];
    sprintf(buffer, "%.1f", n);
    ce_push_string(buffer, x, y, z, scale_x, scale_y);
}

static void ce_push_string_box(const char* str, float x, float y, float z, float w, float h) {
    int32 slen = strlen(str);
    float x_scale = w / ((float)slen * 8.0f);
    float y_scale = h / 8.0f;
    
    for (int32 i = 0; i < slen; i++) {
        ce_push_ascii(str[i], x + i * 8 * x_scale, y, z, x_scale, y_scale);
    }
}

static void ce_push_number_box(int32 n, float x, float y, float z, float w, float h, const float* color) {
    static char buffer[32];
    sprintf(buffer, "%d", n);
    ce_push_string_box(buffer, x, y, z, w, h);
}

#endif

