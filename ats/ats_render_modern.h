#pragma once

#include "ats_shaders.h"

// =================================================== SHADERS =============================================== //

static const char* vertex_shader =
"#version 330 core\n"
"layout (location = 0) in vec3 ps;"
"layout (location = 1) in vec3 ns;"
"layout (location = 2) in vec4 cs;"
"out vec4 color;"
"out vec3 normal;"
"out vec3 frag_pos;"
"uniform mat4 PVM;"
"uniform mat4 M;"
"void main() {"
    "gl_Position = PVM * vec4(ps, 1);"
    "color    = cs;"
    "normal   = normalize(ns);"
    "frag_pos = vec3(M * vec4(ps, 1));"
"}";

static const char* fragment_shader =
"#version 330\n"
"out vec4 frag_color;"
"in vec4 color;"
"in vec3 normal;"
"in vec3 frag_pos;"
"uniform vec3 light_pos;"
"uniform vec3 light_color;"
"void main() {"
    "vec3  light_delta = light_pos - frag_pos;"
    //"float light_dist  = 0.5f * length(light_delta);"
    "vec3 light_dir    = normalize(light_delta);"
    "vec3 diffuse      = max(dot(normal, light_dir), 0.0) * light_color;"
    "vec3 result       = (0.2 * light_color + diffuse) * color.xyz;"
    "frag_color        = vec4(result, color.w);"
    //"frag_color        = vec4((1.0f - light_dist / 16.0f) * result, color.w);"
"}";

// =================================================== RENDER ================================================ //

typedef struct Vertex {
    float   pos[3];
    float   normal[3];
    float   color[4];
} Vertex;

#define VERTEX_ARRAY_MAX (2 << 21)

static struct {
    uint    program;
    uint    vao;
    uint    vbo;
    //
    union {
        struct {
            uint32 matrix   : 1;
            uint32 light    : 1;
            uint32 array    : 1;
            uint32 : 0;
        };
        //
        uint32 all;
    } update;
    //
    uint32  type;
    //
    float   normal[3];
    float   color[4];
    //
    float   PV[16];
    float   M[16];
    float   PVM[16];
    //
    float   light_pos[3];
    float   light_color[3];
    //
    uint32  count;
    Vertex  array[VERTEX_ARRAY_MAX];
} vertex_array;

static void render_init() {
    vertex_array.program = shader_load_from_memory(vertex_shader, fragment_shader);

    shader_use(vertex_array.program);

    vertex_array.normal[0] = 0;
    vertex_array.normal[1] = 0;
    vertex_array.normal[2] = 1;

    vertex_array.color[0] = 1;
    vertex_array.color[1] = 1;
    vertex_array.color[2] = 1;
    vertex_array.color[3] = 1;

    vertex_array.light_color[0] = 1;
    vertex_array.light_color[1] = 1;
    vertex_array.light_color[2] = 1;

    mat4_identity(vertex_array.PV);
    mat4_identity(vertex_array.M);
    mat4_identity(vertex_array.PVM);

    shader_set_mat4(vertex_array.program, "M",   vertex_array.M);
    shader_set_mat4(vertex_array.program, "PVM", vertex_array.PVM);

    glGenVertexArrays(1, &vertex_array.vao);
    glGenBuffers(1, &vertex_array.vbo);

    glBindVertexArray(vertex_array.vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_array.vbo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)(3 * sizeof (float)));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)(6 * sizeof (float)));
}

static inline void render_begin(uint type) {
    vertex_array.update.array = true;

    vertex_array.type  = type;
    vertex_array.count = 0;
}

static inline void render_end() {
    if (vertex_array.update.matrix) {
        mat4_mul(vertex_array.PVM, vertex_array.PV, vertex_array.M);

        shader_set_mat4(vertex_array.program, "PVM", vertex_array.PVM);
        shader_set_mat4(vertex_array.program, "M", vertex_array.M);
    }

    if (vertex_array.update.light) {
        shader_set_vec3(vertex_array.program, "light_pos", vertex_array.light_pos);
        shader_set_vec3(vertex_array.program, "light_color", vertex_array.light_color);
    }

    if (vertex_array.update.array) {
        glBufferData(GL_ARRAY_BUFFER, vertex_array.count * sizeof (Vertex), vertex_array.array, GL_DYNAMIC_DRAW);
    }

    glDrawArrays(vertex_array.type, 0, vertex_array.count);

    vertex_array.update.all = 0;
}

static inline void render_color(float r, float g, float b, float a) {
    vertex_array.color[0] = r;
    vertex_array.color[1] = g;
    vertex_array.color[2] = b;
    vertex_array.color[3] = a;
}

static inline void render_normal(float x, float y, float z) {
    vertex_array.normal[0] = x;
    vertex_array.normal[1] = y;
    vertex_array.normal[2] = z;
}

static inline void render_vertex(float x, float y, float z) {
    Vertex vertex = {
        // position:
        x,
        y,
        z,
        // normal:
        vertex_array.normal[0],
        vertex_array.normal[1],
        vertex_array.normal[2],
        // color:
        vertex_array.color[0],
        vertex_array.color[1],
        vertex_array.color[2],
        vertex_array.color[3]
    };

    vertex_array.array[vertex_array.count++] = vertex;
}

static void render_view(const float* pos, const float* look, const float* up, float fov, float n, float f) {
    vertex_array.update.matrix = true;
    
    float P[16];
    float V[16];

    mat4_perspective(P, to_rad(fov), platform.aspect_ratio, n, f);
    mat4_look_at(V, pos, look, up);
    mat4_mul(vertex_array.PV, P, V);
}

static void render_model(const float* M) {
    vertex_array.update.matrix = true;
    mat4_copy(vertex_array.M, M);
}

static void render_load_identity() {
    vertex_array.update.matrix = true;
    mat4_identity(vertex_array.M);
}

static void render_light(float x, float y, float z, float r, float g, float b) {
    vertex_array.update.light = true;

    vertex_array.light_pos[0] = x;
    vertex_array.light_pos[1] = y;
    vertex_array.light_pos[2] = z;

    vertex_array.light_color[0] = r;
    vertex_array.light_color[1] = g;
    vertex_array.light_color[2] = b;
}

// ---------------------------------- helpers funcs ------------------------------------- //

static inline void render_square(float x, float y, float z, float rad) {
    float xsr = x - rad, xar = x + rad;
    float ysr = y - rad, yar = y + rad;

    render_normal(0.0f, 0.0f, 1.0f);
    render_vertex(xsr, ysr, z);
    render_vertex(xsr, yar, z);
    render_vertex(xar, ysr, z);
    render_vertex(xsr, yar, z);
    render_vertex(xar, yar, z);
    render_vertex(xar, ysr, z);
}

static inline void render_cube(float x, float y, float z, float rad) {
    float xsr = x - rad, xar = x + rad;
    float ysr = y - rad, yar = y + rad;
    float zsr = z - rad, zar = z + rad;

    render_normal(0.0f, 0.0f, -1.0f);
    render_vertex(xsr, ysr, zsr);
    render_vertex(xsr, yar, zsr);
    render_vertex(xar, ysr, zsr);
    render_vertex(xsr, yar, zsr);
    render_vertex(xar, yar, zsr);
    render_vertex(xar, ysr, zsr);

    render_normal(0.0f, 0.0f, +1.0f);
    render_vertex(xsr, ysr, zar);
    render_vertex(xar, ysr, zar);
    render_vertex(xsr, yar, zar);
    render_vertex(xar, ysr, zar);
    render_vertex(xar, yar, zar);
    render_vertex(xsr, yar, zar);

    render_normal(0.0f, -1.0f, 0.0f);
    render_vertex(xsr, ysr, zsr);
    render_vertex(xar, ysr, zsr);
    render_vertex(xsr, ysr, zar);
    render_vertex(xar, ysr, zsr);
    render_vertex(xar, ysr, zar);
    render_vertex(xsr, ysr, zar);

    render_normal(0.0f, 1.0f, 0.0f);
    render_vertex(xsr, yar, zsr);
    render_vertex(xsr, yar, zar);
    render_vertex(xar, yar, zsr);
    render_vertex(xsr, yar, zar);
    render_vertex(xar, yar, zar);
    render_vertex(xar, yar, zsr);

    render_normal(-1.0f, 0.0f, 0.0f);
    render_vertex(xsr, ysr, zsr);
    render_vertex(xsr, ysr, zar);
    render_vertex(xsr, yar, zsr);
    render_vertex(xsr, ysr, zar);
    render_vertex(xsr, yar, zar);
    render_vertex(xsr, yar, zsr);

    render_normal(1.0f, 0.0f, 0.0f);
    render_vertex(xar, ysr, zsr);
    render_vertex(xar, yar, zsr);
    render_vertex(xar, ysr, zar);
    render_vertex(xar, yar, zsr);
    render_vertex(xar, yar, zar);
    render_vertex(xar, ysr, zar);
}

static void render_box_rot(float x, float y, float z, float sx, float sy, float sz, float rot, const float* color) {
    float M[16];
    mat4_translate_scale_rotate_z(M, x, y, z, sx, sy, sz, rot);
    render_color(color[0], color[1], color[2], color[3]);
    render_model(M);
    
    render_begin(GL_TRIANGLES);
    render_cube(0.0f, 0.0f, 0.0f, 1.0f);
    render_end();
}

static void render_box(float x, float y, float z, float sx, float sy, float sz, const float* color) {
    float M[16];
    mat4_translate_scale(M, x, y, z, sx, sy, sz);
    render_color(color[0], color[1], color[2], color[3]);
    render_model(M);
    
    render_begin(GL_TRIANGLES);
    render_cube(0.0f, 0.0f, 0.0f, 1.0f);
    render_end();
}

static void render_cube_rot(float x, float y, float z, float rad, float rot, const float* color) {
    float M[16];
    mat4_translate_scale_rotate_z(M, x, y, z, rad, rad, rad, rot);
    render_color(color[0], color[1], color[2], color[3]);
    render_model(M);
    
    render_begin(GL_TRIANGLES);
    render_cube(0.0f, 0.0f, 0.0f, 1.0f);
    render_end();
}

static void render_cube2(float x, float y, float z, float rad, const float* color) {
    render_color(color[0], color[1], color[2], color[3]);
    render_load_identity();
    
    render_begin(GL_TRIANGLES);
    render_cube(x, y, z, rad);
    render_end();
}

static void render_cube_lerp(float x, float y, float z, float rad, const float* sc, const float* ec, float t) {
    float color[4];
    lerp4v(color, sc, ec, t);
    render_color(color[0], color[1], color[2], color[3]);

    render_cube(x, y, z, rad);
}

static void render_line2v(const float* a, const float* b, float z, float rad, const float* color) {
    float line[2];
    sub2v(line, b, a);

    float line_length = len2v(line);

    float line_pos[2];
    adds2v(line_pos, a, line, 0.5f);

    float rot = get_angle2v(float2(0.0f, -1.0f), line);

    render_box_rot(
            line_pos[0], line_pos[1], z,
            rad, 0.5f * line_length, rad,
            rot,
            color);
}

// ================================================== SIMPLE TEST STUFF ================================================= //

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

#define ASCII_SIZE ((int32)('~' - ' '))

#define bitmap_getbit(N, X, Y) (((uint64)(N)) & ((uint64)1 << (((uint64)(Y)) * 8 + ((uint64)(X)))))
#define bitmap_setbit(N, X, Y) (((uint64)(N)) | ((uint64)1 << (((uint64)(Y)) * 8 + ((uint64)(X)))))

static void bitmap_render_rect(float x, float y, float z, float sx, float sy) {
    float xsr = x - sx, xar = x + sx;
    float ysr = y - sy, yar = y + sy;

    render_vertex(xsr, ysr, z);
    render_vertex(xsr, yar, z);
    render_vertex(xar, ysr, z);
    render_vertex(xsr, yar, z);
    render_vertex(xar, yar, z);
    render_vertex(xar, ysr, z);
}

// @TODO: make less shit!!
static void render_ascii(char c, float px, float py, float pz, float x_scale, float y_scale, const float* col) {
    render_load_identity();
    render_normal(0.0f, 0.0f, 1.0f);
    render_color(col[0], col[1], col[2], col[3]);

    render_begin(GL_TRIANGLES);

    if (c >= ' ' && c <= '~') {
        uint64 n = bitascii[c - ' '];
        for (uint32 j = 0; j < 8; ++j) {
            for (uint32 i = 0; i < 8; ++i) {
                if (bitmap_getbit(n, i, j) > 0) {
                    bitmap_render_rect(
                            px + i * x_scale,
                            py + j * y_scale,
                            pz,
                            x_scale,
                            y_scale);
                }
            }
        }
    }

    render_end();
}

static void render_string(const char* str, float x, float y, float z, float scale_x, float scale_y, const float* c) {
    for (int32 i = 0; str[i] != '\0'; i++) {
        render_ascii(str[i], x + i * 8 * scale_x, y, z, scale_x, scale_y, c);
    }
}

static void render_string_format(float x, float y, float rad, const float* color, const char* fmt, ...) {
    va_list list;
    char    buffer[64];

    va_start(list, fmt);
    vsnprintf(buffer, count_of(buffer), fmt, list);
    render_string(buffer, x, y, 0.0f, rad, rad, color);
    va_end(list);
}


static void render_int32(int32 n, float x, float y, float z, float scale_x, float scale_y, const float* c) {
    static char buffer[32];
    sprintf(buffer, "%d", n);
    render_string(buffer, x, y, z, scale_x, scale_y, c);
}

static void render_float(float n, float x, float y, float z, float scale_x, float scale_y, const float* c) {
    static char buffer[32];
    sprintf(buffer, "%.1f", n);
    render_string(buffer, x, y, z, scale_x, scale_y, c);
}

static void render_string_box(const char* str, float x, float y, float z, float w, float h, const float* c) {
    int32 slen = strlen(str);
    float x_scale = w / ((float)slen * 8.0f);
    float y_scale = h / 8.0f;
    
    for (int32 i = 0; i < slen; i++) {
        render_ascii(str[i], x + i * 8 * x_scale, y, z, x_scale, y_scale, c);
    }
}

static void render_number_box(int32 n, float x, float y, float z, float w, float h, const float* c) {
    static char buffer[32];
    sprintf(buffer, "%d", n);
    render_string_box(buffer, x, y, z, w, h, c);
}

