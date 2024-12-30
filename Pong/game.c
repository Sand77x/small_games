#include "game.h"
#include <math.h>

void updatePlayers(Entity* p1, Entity* p2, Keys keys)
{
    if (keys.w) {
        p1->yVelocity = -PLAYER_VELOCITY;
    } else if (keys.s) {
        p1->yVelocity = PLAYER_VELOCITY;
    } else {
        p1->yVelocity = 0;
    }

    if (keys.up) {
        p2->yVelocity = -PLAYER_VELOCITY;
    } else if (keys.down) {
        p2->yVelocity = PLAYER_VELOCITY;
    } else {
        p2->yVelocity = 0;
    }

    p1->body.dim->y += p1->yVelocity;
    p2->body.dim->y += p2->yVelocity;

    // check out of bounds
    if (p1->body.dim->y + p1->body.dim->h > SCREEN_HEIGHT) {
        p1->body.dim->y = SCREEN_HEIGHT - p1->body.dim->h;
    }

    if (p1->body.dim->y < 0) {
        p1->body.dim->y = 0;
    }

    if (p2->body.dim->y + p1->body.dim->h > SCREEN_HEIGHT) {
        p2->body.dim->y = SCREEN_HEIGHT - p2->body.dim->h;
    }

    if (p2->body.dim->y < 0) {
        p2->body.dim->y = 0;
    }
}

void updateBall(Entity* p1, Entity* p2, Entity* ball)
{
    int ballX = ball->body.dim->x;
    int ballY = ball->body.dim->y;
    int ballW = ball->body.dim->w;
    int ballH = ball->body.dim->h;
    int pX = p1->body.dim->x;
    int pY = p1->body.dim->y;

    // check collision with player 1
    if ((ballX <= pX + p1->body.dim->w && ballX >= PLAYER_OFFSET - ballW) && (ballY + ballH >= pY && ballY <= pY + p1->body.dim->h)) {
        ball->body.dim->x = pX + p1->body.dim->w;
        ball->xVelocity = -ball->xVelocity;

        if (p1->yVelocity != 0)
            ball->yVelocity += p1->yVelocity;
        else if (ball->yVelocity > 0)
            ball->yVelocity -= 1;
        else
            ball->yVelocity += 1;
    }

    pX = p2->body.dim->x;
    pY = p2->body.dim->y;

    // check collision with player 2
    if ((ballX + ballW >= pX && ballX + ballW <= SCREEN_WIDTH - PLAYER_OFFSET) && (ballY + ballH >= pY && ballY <= pY + p2->body.dim->h)) {
        ball->body.dim->x = pX - ballW;
        ball->xVelocity = -ball->xVelocity;

        if (p2->yVelocity != 0)
            ball->yVelocity += p2->yVelocity;
        else if (ball->yVelocity > 0)
            ball->yVelocity -= 1;
        else
            ball->yVelocity += 1;
    }

    // check collision with top and bottom of the screen
    if (ballY + ballH >= SCREEN_HEIGHT) {
        ball->body.dim->y = SCREEN_HEIGHT - ballH;
        ball->yVelocity = -ball->yVelocity;
    }
    if (ballY <= 0) {
        ball->body.dim->y = 0;
        ball->yVelocity = -ball->yVelocity;
    }

    // cap max ball velocity
    if (ball->yVelocity > MAX_BALL_VEL) {
        ball->yVelocity = MAX_BALL_VEL;
    }

    if (ball->yVelocity < -MAX_BALL_VEL) {
        ball->yVelocity = -MAX_BALL_VEL;
    }

    ball->body.dim->x += ball->xVelocity;
    ball->body.dim->y += ball->yVelocity;

    // if ball out of bounds
    if (ballX > SCREEN_WIDTH || ballX < 0) {
        ball->body.dim->x = SCREEN_WIDTH / 2 - BALL_WIDTH;
        ball->body.dim->y = SCREEN_HEIGHT / 2 - BALL_HEIGHT;
        ball->yVelocity = 0.4;
    }
}

void updateGame(Game* game)
{
}
