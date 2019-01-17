#pragma once

#define OPENGL_MODERN
#include "../ats/ats_platform_glfw.h"
#include "../ats/ats_shaders.h"
#include "../ats/ats_texture.h"

static const char* se_vertex_shader =
"#version 330 core\n"
"layout (location = 0) in vec4 pos;"
"layout (location = 1) in vec4 col;"
"layout (location = 2) in vec2 tex;"
"out vec2 tex_coord;"
"out vec4 color;"
"uniform mat4 PVM;"
"void main() {"
"   gl_Position = PVM * pos;"
"   tex_coord = tex;"
"   color = col;"
"}";

static const char* se_fragment_shader =
"#version 330 core\n"
"out vec4 frag_color;"
"in vec2 tex_coord;"
"in vec4 color;"
"uniform sampler2D texture0;"
"void main() {"
"   frag_color = texture(texture0, tex_coord) * color;"
"}";

typedef struct Vertex {
    float       position[4];
    float       color[4];
    float       tex_coord[2];
} Vertex;

#define vertex(x, y, z, r, g, b, a, tx, ty) ((Vertex) { (x), (y), (z), 1.0f, (r), (g), (b), (a), (tx), (ty) })

static Vertex* se_vertex_buffer = NULL;

static int se_shader_program;
static int se_vao;
static int se_vbo;

void se_init() {
    se_shader_program = shader_load_from_memory(se_vertex_shader, se_fragment_shader);

    glGenVertexArrays(1, &se_vao);
    glGenBuffers(1, &se_vbo);

    glBindVertexArray(se_vao);

    glBindBuffer(GL_ARRAY_BUFFER, se_vbo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)(0 * sizeof (float)));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)(4 * sizeof (float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)(8 * sizeof (float)));

    float PVM[16];
    mat4_identity(PVM);

    shader_set_mat4(se_shader_program, "PVM", PVM);
}

static void se_render_buffer() {
    glBindBuffer(GL_ARRAY_BUFFER, se_vbo);
    glBufferData(GL_ARRAY_BUFFER, buf_len(se_vertex_buffer) * sizeof (Vertex), se_vertex_buffer, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, buf_len(se_vertex_buffer));

    buf_clear(se_vertex_buffer);
}

static void se_push_square(float x, float y, float z, float rad, float sx, float sy, float sw, float sh) {
    float xsr = x - rad;
    float xar = x + rad;
    float ysr = y - rad;
    float yar = y + rad;

    buf_push(se_vertex_buffer, vertex(xsr, ysr, z, 1.0f, 1.0f, 1.0f, 1.0f, sx, sy));
    buf_push(se_vertex_buffer, vertex(xar, ysr, z, 1.0f, 1.0f, 1.0f, 1.0f, sx + sw, sy));
    buf_push(se_vertex_buffer, vertex(xsr, yar, z, 1.0f, 1.0f, 1.0f, 1.0f, sx, sy + sh));
    buf_push(se_vertex_buffer, vertex(xar, yar, z, 1.0f, 1.0f, 1.0f, 1.0f, sx + sw, sy + sh));
    buf_push(se_vertex_buffer, vertex(xsr, yar, z, 1.0f, 1.0f, 1.0f, 1.0f, sx, sy + sh));
    buf_push(se_vertex_buffer, vertex(xar, ysr, z, 1.0f, 1.0f, 1.0f, 1.0f, sx + sw, sy));
}

static void se_push_rect(float x, float y, float z, float rx, float ry, float sx, float sy, float sw, float sh) {
    float xsr = x - rx;
    float xar = x + rx;
    float ysr = y - ry;
    float yar = y + ry;

    buf_push(se_vertex_buffer, vertex(xsr, ysr, z, 1.0f, 1.0f, 1.0f, 1.0f, sx,      1.0f - (sy + sh)));
    buf_push(se_vertex_buffer, vertex(xar, ysr, z, 1.0f, 1.0f, 1.0f, 1.0f, sx + sw, 1.0f - (sy + sh)));
    buf_push(se_vertex_buffer, vertex(xsr, yar, z, 1.0f, 1.0f, 1.0f, 1.0f, sx,      1.0f - sy));
    buf_push(se_vertex_buffer, vertex(xar, yar, z, 1.0f, 1.0f, 1.0f, 1.0f, sx + sw, 1.0f - sy));
    buf_push(se_vertex_buffer, vertex(xsr, yar, z, 1.0f, 1.0f, 1.0f, 1.0f, sx,      1.0f - sy));
    buf_push(se_vertex_buffer, vertex(xar, ysr, z, 1.0f, 1.0f, 1.0f, 1.0f, sx + sw, 1.0f - (sy + sh)));
}

typedef struct Sprite {
    // normalized sprite size:
    float       sprite_size[2];
    // normalized texture size;
    float       texture_size[2];
    Texture     texture;
} Sprite;

static Sprite se_sprite_load(const char* filename, int sx, int sy, bool smooth) {
    Sprite s = {0};

    s.texture = texture_load(filename, smooth);

    s.texture_size[0] = (float)s.texture.width  / (float)s.texture.height;
    s.texture_size[1] = (float)s.texture.height / (float)s.texture.width;

    s.sprite_size[0] = (float)sx / (float)s.texture.width;
    s.sprite_size[1] = (float)sy / (float)s.texture.height;

    return s;
}

static void se_sprite_bind(const Sprite* s) {
    texture_bind(&s->texture);
}

static void se_sprite_push(const Sprite* s, float x, float y, float z, float rx, float ry, int i, int j) {
    float sx = i * s->sprite_size[0];
    float sy = j * s->sprite_size[1];

    se_push_rect(x, y, z, rx, ry, sx, sy, s->sprite_size[0], s->sprite_size[1]);
}

