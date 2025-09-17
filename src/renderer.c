#include "renderer.h"
#include <GL/gl.h>
#include <stdio.h>

int window_width = 960;
int window_height = 640;
static float cur_r=1,cur_g=1,cur_b=1,cur_a=1;
static int cam_x = 0, cam_y = 0;

int renderer_init(int w, int h){
    window_width = w; window_height = h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    return 0;
}

void renderer_begin(int camx, int camy){
    cam_x = camx; cam_y = camy;
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-cam_x, -cam_y, 0.0f);
}

void renderer_end(void){
    // nothing extra for now
}

void renderer_set_color(float r, float g, float b, float a){
    cur_r=r;cur_g=g;cur_b=b;cur_a=a;
    glColor4f(r,g,b,a);
}

void renderer_draw_rect(float x, float y, float w, float h){
    glDisable(GL_TEXTURE_2D);
    glColor4f(cur_r,cur_g,cur_b,cur_a);
    glBegin(GL_QUADS);
      glVertex2f(x, y);
      glVertex2f(x + w, y);
      glVertex2f(x + w, y + h);
      glVertex2f(x, y + h);
    glEnd();
}

void renderer_draw_triangle(float x, float y, float w, float h){
    glDisable(GL_TEXTURE_2D);
    glColor4f(cur_r,cur_g,cur_b,cur_a);
    glBegin(GL_TRIANGLES);
      glVertex2f(x, y + h);
      glVertex2f(x + w/2.0f, y);
      glVertex2f(x + w, y + h);
    glEnd();
}

void renderer_draw_textured(GLuint tex, float x, float y, float w, float h, float u0, float v0, float u1, float v1){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glColor4f(1,1,1,1);
    glBegin(GL_QUADS);
      glTexCoord2f(u0,v0); glVertex2f(x,y);
      glTexCoord2f(u1,v0); glVertex2f(x+w,y);
      glTexCoord2f(u1,v1); glVertex2f(x+w,y+h);
      glTexCoord2f(u0,v1); glVertex2f(x,y+h);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

