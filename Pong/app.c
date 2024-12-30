#include "app.h"
#include <SDL2/SDL.h>
#include <stdio.h>

static uint32_t nextTime;

int start(Controller* controller, Game* game)
{
    // set next frame time
    nextTime = SDL_GetTicks() + TICK_INTERVAL;

    int close_requested = 0;
    Keys keys = { 0 };
    while (close_requested == 0) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                close_requested = 1;
                break;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                    keys.w = 1;
                    break;
                case SDL_SCANCODE_S:
                    keys.s = 1;
                    break;
                case SDL_SCANCODE_UP:
                    keys.up = 1;
                    break;
                case SDL_SCANCODE_DOWN:
                    keys.down = 1;
                    break;
                default:
                    break;
                }
            } else if (e.type == SDL_KEYUP) {
                switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                    keys.w = 0;
                    break;
                case SDL_SCANCODE_S:
                    keys.s = 0;
                    break;
                case SDL_SCANCODE_UP:
                    keys.up = 0;
                    break;
                case SDL_SCANCODE_DOWN:
                    keys.down = 0;
                    break;
                default:
                    break;
                }
            }
        }

        // update players
        updatePlayers(&game->player1, &game->player2, keys);

        // update ball
        updateBall(&game->player1, &game->player2, &game->ball);

        // update game
        updateGame(game);

        // set window to black
        SDL_SetRenderDrawColor(controller->renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(controller->renderer);

        // set draw color to white
        SDL_SetRenderDrawColor(controller->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // draw ball
        SDL_RenderCopy(controller->renderer, game->ball.body.tex, NULL, game->ball.body.dim);

        // draw players
        SDL_RenderDrawRect(controller->renderer, game->player1.body.dim);
        SDL_RenderDrawRect(controller->renderer, game->player2.body.dim);

        // switch buffers
        SDL_RenderPresent(controller->renderer);

        // delay for constant fps
        uint32_t now = SDL_GetTicks();
        if (nextTime > now) {
            SDL_Delay(nextTime - SDL_GetTicks());
        }

        nextTime += TICK_INTERVAL;
    }

    return 0;
}
