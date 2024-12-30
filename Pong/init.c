#include "init.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_video.h"
#include "game.h"
#include <stdio.h>

int init(Controller* controller, Game* game)
{
    // init sdl subsystems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    // get window and renderer
    if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &controller->window, &controller->renderer) != 0) {
        printf("failed to get window / renderer: %s\n", SDL_GetError());
        return 1;
    }

    // create rects for entities
    SDL_Rect* rect1 = malloc(sizeof(SDL_Rect));
    SDL_Rect* rect2 = malloc(sizeof(SDL_Rect));
    SDL_Rect* rect3 = malloc(sizeof(SDL_Rect));

    if (rect1 == NULL || rect2 == NULL || rect3 == NULL) {
        printf("failed to allocate for player / ball rects: %s\n", SDL_GetError());
    }

    game->player1.body.dim = rect1;
    game->player2.body.dim = rect2;
    game->ball.body.dim = rect3;

    rect1 = rect2 = rect3 = NULL;

    // load ball image
    SDL_Surface* ballSurface
        = SDL_LoadBMP("assets/ball.bmp");
    if (ballSurface == NULL) {
        printf("failed to load assets/ball.bmp: %s\n", SDL_GetError());
    }

    SDL_Texture* ballTexture = SDL_CreateTextureFromSurface(controller->renderer, ballSurface);
    if (ballTexture == NULL) {
        printf("failed to create ball texture: %s\n", SDL_GetError());
    }

    game->ball.body.tex = ballTexture;
    SDL_FreeSurface(ballSurface);

    // set player textures to null
    game->player1.body.tex = NULL;
    game->player2.body.tex = NULL;

    // set player scores to 0
    game->player1Score = game->player2Score = 0;

    // set default velocities
    game->player1.xVelocity = 0;
    game->player1.yVelocity = 0;
    game->player2.xVelocity = 0;
    game->player2.yVelocity = 0;
    game->ball.xVelocity = DEFAULT_BALL_XVEL;
    game->ball.yVelocity = DEFAULT_BALL_YVEL;

    // set default positions
    *game->player1.body.dim = (SDL_Rect) { PLAYER_OFFSET, SCREEN_HEIGHT / 2 - PLAYER_HEIGHT / 2, PLAYER_WIDTH, PLAYER_HEIGHT };
    *game->player2.body.dim = (SDL_Rect) { SCREEN_WIDTH - PLAYER_OFFSET - PLAYER_WIDTH, SCREEN_HEIGHT / 2 - PLAYER_HEIGHT / 2, PLAYER_WIDTH, PLAYER_HEIGHT };
    *game->ball.body.dim = (SDL_Rect) { SCREEN_WIDTH / 2 - BALL_WIDTH / 2, SCREEN_HEIGHT / 2 - BALL_HEIGHT / 2, BALL_WIDTH, BALL_HEIGHT };

    // success
    return 0;
}

void close(Controller* controller, Game* game)
{
    // free entity rects
    free(game->player1.body.dim);
    free(game->player2.body.dim);
    free(game->ball.body.dim);

    // free ball texture
    SDL_DestroyTexture(game->ball.body.tex);

    // free window and renderer
    SDL_DestroyRenderer(controller->renderer);
    SDL_DestroyWindow(controller->window);

    // quit sdl subsystems
    SDL_Quit();
}
