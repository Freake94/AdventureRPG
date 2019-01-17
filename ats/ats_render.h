#pragma once

#include "ats_platform_glfw.h"
#include "ats_math.h"

// @NOTE: layout - vertex : normal
static const float render_shapes[] = {
    // rectangle:
    -1.0f, -1.0f, 0.0f,     0.0f, 0.0f, 1.0f,
    +1.0f, -1.0f, 0.0f,     0.0f, 0.0f, 1.0f,
    +1.0f, +1.0f, 0.0f,     0.0f, 0.0f, 1.0f,
    -1.0f, +1.0f, 0.0f,     0.0f, 0.0f, 1.0f,
    // cube:
    -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, -1.0f,
    +1.0f, -1.0f, -1.0f,    0.0f, 0.0f, -1.0f,
    +1.0f, +1.0f, -1.0f,    0.0f, 0.0f, -1.0f,
    -1.0f, +1.0f, -1.0f,    0.0f, 0.0f, -1.0f,

    -1.0f, -1.0f, +1.0f,    0.0f, 0.0f, 1.0f,
    +1.0f, -1.0f, +1.0f,    0.0f, 0.0f, 1.0f,
    +1.0f, +1.0f, +1.0f,    0.0f, 0.0f, 1.0f,
    -1.0f, +1.0f, +1.0f,    0.0f, 0.0f, 1.0f,

    -1.0f, -1.0f, -1.0f,    0.0f, -1.0f, 0.0f,
    +1.0f, -1.0f, -1.0f,    0.0f, -1.0f, 0.0f,
    +1.0f, -1.0f, +1.0f,    0.0f, -1.0f, 0.0f,
    -1.0f, -1.0f, +1.0f,    0.0f, -1.0f, 0.0f,

    -1.0f, +1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
    +1.0f, +1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
    +1.0f, +1.0f, +1.0f,    0.0f, 1.0f, 0.0f,
    -1.0f, +1.0f, +1.0f,    0.0f, 1.0f, 0.0f,

    -1.0f, -1.0f, -1.0f,    -1.0f, 0.0f, 0.0f,
    -1.0f, +1.0f, -1.0f,    -1.0f, 0.0f, 0.0f,
    -1.0f, +1.0f, +1.0f,    -1.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, +1.0f,    -1.0f, 0.0f, 0.0f,

    +1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 0.0f,
    +1.0f, +1.0f, -1.0f,    1.0f, 0.0f, 0.0f,
    +1.0f, +1.0f, +1.0f,    1.0f, 0.0f, 0.0f,
    +1.0f, -1.0f, +1.0f,    1.0f, 0.0f, 0.0f,
};

static void render_begin_shapes() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 6 * sizeof (float), render_shapes);
    glNormalPointer(GL_FLOAT, 6 * sizeof (float), &render_shapes[3]);
}

static void render_init() {
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

    glEnable(GL_NORMALIZE);

    render_begin_shapes();
}

static void render_clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void render_set_matrix(const float* matrix) {
    glLoadMatrixf(matrix);
}

static void render_set_projection_view(float* projection_view) {
    glMatrixMode(GL_PROJECTION);
    render_set_matrix(projection_view);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void render_set_model(float px, float py, float pz, float sx, float sy, float sz) {
    float T[16];
    mat4_translate_scale(T, px, py, pz, sx, sy, sz);
    render_set_matrix(T);
}

static void render_set_translate_scale_rotate_z(
        float px, float py, float pz,
        float sx, float sy, float sz,
        float rot) {
    float T[16];
    
    mat4_translate_scale_rotate_z(T, px, py, pz, sx, sy, sz, rot);
    render_set_matrix(T);
}

static void render_set_camera(
        float px, float py, float pz,
        float tx, float ty, float tz,
        float ux, float uy, float uz,
        float fov, float n, float f) {
    float P[16];
    float V[16];
    float PV[16];

    mat4_perspective(P, to_rad(fov), platform.aspect_ratio, n, f);
    mat4_look_at(V, float3(px, py, pz), float3(tx, ty, tz), float3(ux, uy, uz));
    mat4_mul(PV, P, V);

    render_set_projection_view(PV);
}

static void render_set_color(float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
}

static void render_quad() {
    glDrawArrays(GL_QUADS, 0, 4);
}

static void render_cube() {
    glDrawArrays(GL_QUADS, 4, 24);
}

static void render_square(float x, float y, float z, float rad, float r, float g, float b, float a) {
    render_set_color(r, g, b, a);
    render_set_model(x, y, z, rad, rad, rad);
    render_quad();
}

static void render_line(float x0, float y0, float z0, float x1, float y1, float z1, float r, float g, float b, float a) {
    render_set_color(r, g, b, a);
    glLoadIdentity();
    glBegin(GL_LINES);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x0, y0, z0);
    glVertex3f(x1, y1, z1);
    glEnd();
}

static void render_set_light_emiter(int index, float bright, float x, float y, float z) {
    float   pos[4]    = { x, y, z, 1 };
    float   zero[4]   = { 0, 0, 0, 0 };
    float   c[4]      = { bright, bright, bright, 0 };
    GLuint  light     = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, pos);
    glLightfv(light, GL_DIFFUSE,  c);
    glLightfv(light, GL_AMBIENT,  zero);
    glLightfv(light, GL_SPECULAR, zero);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

static void render_set_light_directed(int index, float bright, float x, float y, float z) {
    float d       = (float) (1.0f / sqrtf(x * x + y * y + z * z));
    float dir[4]  = { x * d, y * d, z * d, 0 };
    float zero[4] = { 0, 0, 0, 0 };
    float c[4]    = { bright, bright, bright, 0 };
    GLuint light  = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, dir);
    glLightfv(light, GL_DIFFUSE,  c);
    glLightfv(light, GL_AMBIENT,  zero);
    glLightfv(light, GL_SPECULAR, zero);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

static void render_set_light_global_ambient(float r, float g, float b) {
   float v[4] = { r, g, b, 0 };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, v);
}

// ===================================================== VERTEX ARRAY ============================================= //

#define RENDER_VERTEX_MAX (1 << 20)

static uint  render__vertex_count = 0;
static float render__vertex_array[3 * 4 * RENDER_VERTEX_MAX];

static void render_begin_vertex_array() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glVertexPointer(3, GL_FLOAT, 3 * 4 * sizeof (float), render__vertex_array);
    glColorPointer(4, GL_FLOAT, 3 * 4 * sizeof (float), render__vertex_array + 3);
}

static void render_push_vertex(float x, float y, float z, float r, float g, float b, float a) {
    render__vertex_array[render__vertex_count++] = x;
    render__vertex_array[render__vertex_count++] = y;
    render__vertex_array[render__vertex_count++] = z;
    //
    render__vertex_array[render__vertex_count++] = r;
    render__vertex_array[render__vertex_count++] = g;
    render__vertex_array[render__vertex_count++] = b;
    render__vertex_array[render__vertex_count++] = a;
}

static void render_vertex_array(uint gl_type) {
    glDrawArrays(gl_type, 0, render__vertex_count);
    render__vertex_count = 0;
}

// ======================================= TEXTURES ======================================== //

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h" 

typedef struct Texture {
    uint32      id;
    int32       width;
    int32       height;
} Texture;

static Texture texture_load(const char* texture_path, int32 is_smooth) {
    Texture texture     = {0};
    int32   channels    = 0;
    uint8*  pixels      = NULL;

    pixels = stbi_load(texture_path, &texture.width, &texture.height, &channels, 0);

    assert(pixels);

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);

    stbi_image_free(pixels);

    return texture;
}

static void texture_bind(const Texture* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

// ================================================= SLOW SIMPLE BITMAP FONT ======================================= //

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
    float T[16];
    mat4_translate_scale(T, x, y, 0.0f, 0.5f * sx, 0.5f * sy, 1.0f);
    render_set_matrix(T);
    glDrawArrays(GL_QUADS, 0, 4);
}

// @TODO: make less shit!!
static void render_ascii(char c, float px, float py, float pz, float x_scale, float y_scale, const float* col) {
    glColor4fv(col);

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
}

static void render_string(const char* str, float x, float y, float z, float scale_x, float scale_y, const float* color) {
    for (int32 i = 0; str[i] != '\0'; i++) {
        render_ascii(str[i], x + i * 8 * scale_x, y, z, scale_x, scale_y, color);
    }
}

static void render_int32(int32 n, float x, float y, float z, float scale_x, float scale_y, const float* color) {
    static char buffer[32];
    sprintf(buffer, "%d", n);
    render_string(buffer, x, y, z, scale_x, scale_y, color);
}

static void render_float(float n, float x, float y, float z, float scale_x, float scale_y, const float* color) {
    static char buffer[32];
    sprintf(buffer, "%.1f", n);
    render_string(buffer, x, y, z, scale_x, scale_y, color);
}

static void render_string_box(const char* str, float x, float y, float z, float w, float h, const float* color) {
    int32 slen = strlen(str);
    float x_scale = w / ((float)slen * 8.0f);
    float y_scale = h / 8.0f;
    
    for (int32 i = 0; i < slen; i++) {
        render_ascii(str[i], x + i * 8 * x_scale, y, z, x_scale, y_scale, color);
    }
}

static void render_number_box(int32 n, float x, float y, float z, float w, float h, const float* color) {
    static char buffer[32];
    sprintf(buffer, "%d", n);
    render_string_box(buffer, x, y, z, w, h, color);
}

