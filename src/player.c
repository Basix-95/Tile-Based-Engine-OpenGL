#include "player.h"

void player_init(Player *p){
    p->pos.x = 0;
    p->pos.y = 0;
    p->vel.x = p->vel.y = 0;
    p->speed = 160.0f;
    p->jumpVel = 320.0f;
    p->onGround = 0;
}

Rect player_get_aabb(const Player *p){
    Rect r;
    r.x = p->pos.x;
    r.y = p->pos.y;
    r.w = 20.0f;
    r.h = 28.0f;
    return r;
}

