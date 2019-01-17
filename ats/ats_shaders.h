#pragma once

#include "ats_tool.h"

static const char* shader_vertex_simple =
"#version 430 core\n"
"layout (location = 0) in vec3 vp;\n"
"uniform mat4 PV;\n"
"uniform mat4 M;\n"
"void main() {\n"
"   gl_Position = PV * M * vec4(vp, 1.0);\n"
"}\n";

static const char* shader_fragment_simple =
"#version 430 core\n"
"out vec4 frag_color;\n"
"uniform vec4 color;\n"
"void main() {\n"
"   frag_color = color;\n"
"}\n";

static uint shader_compile(const char* source, uint type) {
    uint success;

    char info_log[512];

    uint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        puts(info_log);

        exit(EXIT_FAILURE);
    }

    return shader;
}

static uint shader_link_program(uint vertex_shader, uint fragment_shader) {
    uint success;
    char info_log[512];

    uint shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        puts(info_log);

        exit(EXIT_FAILURE);
    }

    return shader_program;
}

static uint shader_load_from_memory(const char* vs, const char* fs) {
    uint success;
    char info_log[512];

    uint vertex     = shader_compile(vs, GL_VERTEX_SHADER);
    uint fragment   = shader_compile(fs, GL_FRAGMENT_SHADER);

    uint shader_program = shader_link_program(vertex, fragment);

    glUseProgram(shader_program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shader_program;
}

static uint shader_load_from_files(const char* vertex_path, const char* fragment_path) {
    char* vs = file_read_str(vertex_path);
    char* fs = file_read_str(fragment_path);

    uint program = shader_load_from_memory(vs, fs);

    free(vs);
    free(fs);

    return program;
}

static void shader_use(uint shader) {
    glUseProgram(shader);
}

static uint shader_get_location(uint shader, const char* var_name) {
    return glGetUniformLocation(shader, var_name);
}

static void shader_setf(uint shader, const char* loc, float n) {
    glUniform1f(glGetUniformLocation(shader, loc), n);
}

static void shader_set2f(uint shader, const char* loc, float a, float b) {
    glUniform2f(glGetUniformLocation(shader, loc), a, b);
}

static void shader_set3f(uint shader, const char* loc, float a, float b, float c) {
    glUniform3f(glGetUniformLocation(shader, loc), a, b, c);
}

static void shader_set4f(uint shader, const char* loc, float a, float b, float c, float d) {
    glUniform4f(glGetUniformLocation(shader, loc), a, b, c, d);
}

static void shader_set2v(uint shader, const char* loc, const float* u) {
    glUniform2fv(glGetUniformLocation(shader, loc), 1, u);
}

static void shader_set3v(uint shader, const char* loc, const float* u) {
    glUniform3fv(glGetUniformLocation(shader, loc), 1, u);
}

static void shader_set4v(uint shader, const char* loc, const float* u) {
    glUniform4fv(glGetUniformLocation(shader, loc), 1, u);
}

static void shader_set_mat4(uint shader, const char* loc, const float* m) {
    glUniformMatrix4fv(glGetUniformLocation(shader, loc), 1, GL_FALSE, m);
}

