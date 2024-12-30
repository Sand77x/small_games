#include "app.h"
#include "game.h"
#include "init.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    Controller controller;
    Game game;

    if (init(&controller, &game) != 0) {
        printf("failed to initialize app.\n");
        close(&controller, &game);
        return 1;
    }

    printf("successfully initialized app.\n");

    if (start(&controller, &game) != 0) {
        printf("game crashed due to an error.\n");
        close(&controller, &game);
        return 1;
    }

    close(&controller, &game);

    printf("successfully closed app.\n");
    return 0;
}
