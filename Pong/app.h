#ifndef APP_H
#define APP_H

#include "game.h"
#include <SDL2/SDL.h>

#define TICK_INTERVAL 15

typedef struct Controller {
    SDL_Window* window;
    SDL_Renderer* renderer;
} Controller;

int start(Controller* controller, Game* game);

#endif
