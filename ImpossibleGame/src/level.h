#ifndef LEVEL_H
#define LEVEL_H

#include <stddef.h>
#include <SDL2/SDL.h>

typedef struct {
    SDL_FRect* boxes;
    size_t    box_count;
} Level;

Level load_level(const char* path);
void free_level(Level level);

#endif
