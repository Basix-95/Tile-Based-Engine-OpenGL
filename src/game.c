#include "game.h"
#include "tileset.h"
#include "renderer.h"
#include "player.h"
#include "tileset.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define TILE_SIZE 32
#define MAP_W 30
#define MAP_H 20

static int tilemap[MAP_H][MAP_W];
static Player player;
static float camX = 0.0f, camY = 0.0f;
static GLuint tilesTex = 0;
static int tex_w=0, tex_h=0;

extern int window_width, window_height;
extern int key_state[1024];

static void create_test_map(void){
    for(int y=0;y<MAP_H;y++) for(int x=0;x<MAP_W;x++) tilemap[y][x]=0;
    for(int x=0;x<MAP_W;x++) tilemap[MAP_H-2][x] = 1;
    for(int x=4;x<9;x++) tilemap[12][x] = 1;
    for(int x=12;x<16;x++) tilemap[10][x] = 1;
    for(int x=18;x<22;x++) tilemap[8][x] = 1;
    tilemap[MAP_H-3][8] = 2;
    tilemap[MAP_H-3][9] = 2;
}

static int world_to_tile_x(float wx){ return (int)floorf(wx / TILE_SIZE); }
static int world_to_tile_y(float wy){ return (int)floorf(wy / TILE_SIZE); }

static int tile_at_pixel(float px, float py){
    int tx = world_to_tile_x(px);
    int ty = world_to_tile_y(py);
    if(tx < 0 || tx >= MAP_W || ty < 0 || ty >= MAP_H) return 1; // outside -> solid
    return tilemap[ty][tx];
}

// simple aabb intersect
static int aabb_intersect(Rect a, Rect b){
    return !(a.x + a.w <= b.x || b.x + b.w <= a.x || a.y + a.h <= b.y || b.y + b.h <= a.y);
}

static void resolve_collisions(Player *p){
    Rect box = player_get_aabb(p);
    int minTx = world_to_tile_x(box.x) - 1;
    int maxTx = world_to_tile_x(box.x + box.w) + 1;
    int minTy = world_to_tile_y(box.y) - 1;
    int maxTy = world_to_tile_y(box.y + box.h) + 1;
    p->onGround = 0;
    for(int ty = minTy; ty <= maxTy; ++ty){
        for(int tx = minTx; tx <= maxTx; ++tx){
            if(tx < 0 || tx >= MAP_W || ty < 0 || ty >= MAP_H) continue;
            int t = tilemap[ty][tx];
            if(t != 1) continue;
            Rect tileBox = { tx * TILE_SIZE, ty * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            if(aabb_intersect(box, tileBox)){
                float overlapL = (box.x + box.w) - tileBox.x;
                float overlapR = (tileBox.x + tileBox.w) - box.x;
                float overlapT = (box.y + box.h) - tileBox.y;
                float overlapB = (tileBox.y + tileBox.h) - box.y;
                float minOverlap = overlapL;
                if(overlapR < minOverlap) minOverlap = overlapR;
                if(overlapT < minOverlap) minOverlap = overlapT;
                if(overlapB < minOverlap) minOverlap = overlapB;

                if(minOverlap == overlapT){
                    p->pos.y -= overlapT;
                    p->vel.y = 0;
                } else if(minOverlap == overlapB){
                    p->pos.y += overlapB;
                    p->vel.y = 0;
                    p->onGround = 1;
                } else if(minOverlap == overlapL){
                    p->pos.x -= overlapL;
                    p->vel.x = 0;
                } else {
                    p->pos.x += overlapR;
                    p->vel.x = 0;
                }
                box = player_get_aabb(p);
            }
        }
    }
}

void game_init(GLFWwindow *win){
    (void)win;
    create_test_map();
    player_init(&player);
    player.pos.x = 64;
    player.pos.y = 64;

    // load tileset optional
    tilesTex = tileset_load("assets/tileset.png", &tex_w, &tex_h);
    if(tilesTex) printf("Loaded tileset %dx%d\n", tex_w, tex_h);

    // GL state for immediate mode
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void game_update_and_render(GLFWwindow *win, double dt){
    (void)win;
    // Input
    float ax = 0.0f;
    if(key_state[GLFW_KEY_A] || key_state[GLFW_KEY_LEFT]) ax -= 1.0f;
    if(key_state[GLFW_KEY_D] || key_state[GLFW_KEY_RIGHT]) ax += 1.0f;

    player.vel.x = ax * player.speed;
    player.vel.y += 800.0f * dt; // gravity

    if(key_state[GLFW_KEY_SPACE] && player.onGround){
        player.vel.y = -player.jumpVel;
        player.onGround = 0;
    }

    // integrate
    player.pos.x += player.vel.x * dt;
    resolve_collisions(&player);

    player.pos.y += player.vel.y * dt;
    resolve_collisions(&player);

    if(player.pos.y > MAP_H * TILE_SIZE + 200){
        player.pos.x = 64; player.pos.y = 64; player.vel.x = player.vel.y = 0;
    }

    // camera follow (smooth)
    float screenW = window_width;
    float screenH = window_height;
    float targetX = player.pos.x + player_get_aabb(&player).w/2 - screenW/2;
    float targetY = player.pos.y + player_get_aabb(&player).h/2 - screenH/2;
    float maxX = MAP_W * TILE_SIZE - screenW;
    float maxY = MAP_H * TILE_SIZE - screenH;
    if(targetX < 0) targetX = 0; if(targetX > maxX) targetX = maxX;
    if(targetY < 0) targetY = 0; if(targetY > maxY) targetY = maxY;
    camX += (targetX - camX) * 0.1f;
    camY += (targetY - camY) * 0.1f;

    // render world
    renderer_begin((int)camX, (int)camY);
    // tiles
    for(int y=0;y<MAP_H;y++){
        for(int x=0;x<MAP_W;x++){
            int t = tilemap[y][x];
            int wx = x * TILE_SIZE;
            int wy = y * TILE_SIZE;
            if(t == 0) continue;
            if(t == 1){
                if(tilesTex){
                    renderer_draw_textured(tilesTex, wx, wy, TILE_SIZE, TILE_SIZE, 0,0,1,1);
                } else {
                    renderer_set_color(0.2f,0.2f,0.2f,1.0f);
                    renderer_draw_rect(wx, wy, TILE_SIZE, TILE_SIZE);
                }
            } else if(t == 2){
                renderer_set_color(0.8f,0.2f,0.2f,1.0f);
                renderer_draw_triangle(wx, wy, TILE_SIZE, TILE_SIZE);
            }
        }
    }
    // player
    renderer_set_color(0.1f,0.6f,0.95f,1.0f);
    Rect pbox = player_get_aabb(&player);
    renderer_draw_rect(pbox.x, pbox.y, pbox.w, pbox.h);
    renderer_end();
}

void game_shutdown(void){
    if(tilesTex) glDeleteTextures(1, &tilesTex);
}

