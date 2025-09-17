#ifndef PLAYER_H
#define PLAYER_H
typedef struct { float x,y; } Vec2;
typedef struct { float x,y,w,h; } Rect;

typedef struct {
    Vec2 pos;
    Vec2 vel;
    float speed;
    float jumpVel;
    int onGround;
} Player;

void player_init(Player *p);
Rect player_get_aabb(const Player *p);

#endif

