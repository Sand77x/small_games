#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define PLAYER_OFFSET 20
#define PLAYER_WIDTH 15
#define PLAYER_HEIGHT 100
#define PLAYER_VELOCITY 8
#define BALL_WIDTH 20
#define BALL_HEIGHT 20
#define DEFAULT_BALL_XVEL 5
#define DEFAULT_BALL_YVEL 2
#define MAX_BALL_VEL 6

typedef struct Keys {
    int w, s;
    int up, down;
} Keys;

typedef struct Body {
    SDL_Rect* dim;
    SDL_Texture* tex;
} Body;

typedef struct Entity {
    Body body;
    float xVelocity;
    float yVelocity;
} Entity;

typedef struct Game {
    Entity player1;
    Entity player2;
    Entity ball;
    int player1Score;
    int player2Score;
} Game;

void updatePlayers(Entity* p1, Entity* p2, Keys keys);
void updateBall(Entity* p1, Entity* p2, Entity* ball);
void updateGame(Game* game);

#endif
