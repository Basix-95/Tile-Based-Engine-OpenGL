#include "tileset.h"
#include "stb_image.h"
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  // or keep this in a separate stb_image.c

GLuint tileset_load(const char *filename, int *out_w, int *out_h) {
    int n;
    unsigned char *data = stbi_load(filename, out_w, out_h, &n, 4);
    if (!data) {
        fprintf(stderr, "Failed to load texture %s\n", filename);
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *out_w, *out_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(data);
    return tex;
}

