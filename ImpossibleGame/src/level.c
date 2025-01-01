#include "level.h"
#include "constants.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define ERR_LEVEL (Level){0,0}

Level load_level(const char* path) {
    FILE* file;
    fopen_s(&file, path, "r");

    if (file == NULL) {
        char err[100];
        strerror_s(err, 100, errno);
        SDL_LogError(1, "Could not load %s: %s", path, err);
        return ERR_LEVEL;
    }

    Level level = {0};
    char line[100];

    if (fgets(line, sizeof(line), file) == NULL) {
        SDL_LogError(1, "Could not load %s: %zu", path, (size_t)1);
        return ERR_LEVEL;
    }

    if (sscanf_s(line, "%d", &level.box_count) != 1) {
        SDL_LogError(1, "Could not load %s: %zu", path, (size_t)1);
        return ERR_LEVEL;
    }

    level.boxes = malloc(sizeof(SDL_FRect)*level.box_count);

    if (level.boxes == NULL) {
        // error
        SDL_LogError(1, "Could not load %s: Allocation error", path);
        return ERR_LEVEL;
    }

    for (size_t i=0; i<level.box_count; i++) {
        if (fgets(line, sizeof(line), file) == NULL) {
            SDL_LogError(1, "Could not load %zu", path, i+2);
            free_level(level);
            return ERR_LEVEL;
        }

        int x, y;
        if (sscanf_s(line, "%d %d", &x, &y) != 2) {
            SDL_LogError(1, "Could not load %zu", path, i+2);
            free_level(level);
            return ERR_LEVEL;
        }

        level.boxes[i] = (SDL_FRect) { .x = MIN_BOX_OFFSET_LEFT+PLAYER_SIZE*x, .y = SCREEN_HEIGHT-GROUND_HEIGHT-PLAYER_SIZE*(y+1), .w = PLAYER_SIZE, .h = PLAYER_SIZE };
    }

    SDL_Log("Level loaded: %s", path);
    return level;
}

void free_level(Level level) {
    free(level.boxes);
}
