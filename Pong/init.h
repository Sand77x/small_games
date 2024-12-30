#ifndef INIT_H
#define INIT_H

#include "app.h"
#include "game.h"
#include <SDL2/SDL.h>

int init(Controller* controller, Game* game);
void close(Controller* controller, Game* game);

#endif
