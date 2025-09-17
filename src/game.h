#ifndef GAME_H
#define GAME_H

#include <GLFW/glfw3.h>


void game_init(GLFWwindow *win);
void game_update_and_render(GLFWwindow *win, double dt);
void game_shutdown(void);

#endif // GAME_H

