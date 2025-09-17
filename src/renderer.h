#ifndef RENDERER_H
#define RENDERER_H
#include <GLFW/glfw3.h>
typedef unsigned int GLuint;

extern int window_width;
extern int window_height;
int renderer_init(int w, int h);
void renderer_begin(int camx, int camy);
void renderer_end(void);
void renderer_draw_rect(float x, float y, float w, float h);
void renderer_draw_triangle(float x, float y, float w, float h);
void renderer_set_color(float r, float g, float b, float a);
void renderer_draw_textured(GLuint tex, float x, float y, float w, float h, float u0, float v0, float u1, float v1);
#endif

