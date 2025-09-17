#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "renderer.h"

int key_state[1024];

static void key_cb(GLFWwindow* w, int key, int sc, int action, int mods){
    (void)w;(void)sc;(void)mods;
    if(key < 0 || key >= 1024) return;
    if(action == GLFW_PRESS) key_state[key] = 1;
    else if(action == GLFW_RELEASE) key_state[key] = 0;

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(w, GLFW_TRUE);
}

int main(void){
    if(!glfwInit()){
        fprintf(stderr,"Failed to init GLFW\n");
        return -1;
    }

    int W = 960, H = 640;
    GLFWwindow *win = glfwCreateWindow(W, H, "2D Tile Platformer (C)", NULL, NULL);
    if(!win){
        fprintf(stderr,"Failed create window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(win);
    glfwSetKeyCallback(win, key_cb);

    renderer_init(W, H);
    game_init(win);

    double last = glfwGetTime();
    while(!glfwWindowShouldClose(win)){
        double now = glfwGetTime();
        double dt = now - last;
        last = now;
        if(dt > 0.05) dt = 0.05;

        game_update_and_render(win, dt);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    game_shutdown();
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}

