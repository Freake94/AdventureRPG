
#ifndef ATS_TEXTURE
#define ATS_TEXTURE

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb/stb_image.h" 

typedef struct Texture {
    uint32         id;
    int32         width;
    int32         height;
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

#ifdef OPENGL_MODERN
    glGenerateMipmap(GL_TEXTURE_2D);
#endif

    stbi_image_free(pixels);

    return texture;
}

static void texture_bind(const Texture* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

#endif

