#include "constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>

SDL_Window* g_window;
SDL_Renderer* g_renderer;
SDL_Rect g_player;

SDL_Rect* g_boxes;
size_t g_box_count;

void simulate_gravity(SDL_Rect* const, double);
void player_jump(SDL_Rect* const, int, double);
static inline int player_on_ground();
static inline int player_on_box(SDL_Rect*);
void move_boxes_left(double);
SDL_Rect* box_collided();
static inline int are_rects_colliding(SDL_Rect* const, SDL_Rect* const); 

void render_boxes();

int main(int argv, char** args) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_LogError(1, "Failed to initialize SDL.");
    }

    g_window = SDL_CreateWindow("Impossible Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (g_window == NULL) {
        SDL_LogError(1, "Failed to create window.");
        exit(1);
    }

    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (g_renderer == NULL) {
        SDL_LogError(1, "Failed to create renderer.");
        exit(1);
    }
    
    g_player = (SDL_Rect){
        .x = PLAYER_OFFSET_LEFT,
        .y = SCREEN_HEIGHT-GROUND_HEIGHT-PLAYER_SIZE,
        .w = PLAYER_SIZE,
        .h = PLAYER_SIZE 
    };

    g_box_count = 4;
    g_boxes = malloc(sizeof(SDL_Rect)*g_box_count);
    if (g_boxes == NULL) {
        SDL_LogError(1, "Failed to create obstacles");
        exit(1);
    }

    g_boxes[0] = (SDL_Rect){
        .x = MIN_BOX_OFFSET_LEFT+50*0,
        .y = SCREEN_HEIGHT-GROUND_HEIGHT-PLAYER_SIZE,
        .w = PLAYER_SIZE,
        .h = PLAYER_SIZE 
    };
    g_boxes[1] = (SDL_Rect){
        .x = MIN_BOX_OFFSET_LEFT+50*3,
        .y = SCREEN_HEIGHT-GROUND_HEIGHT-PLAYER_SIZE*3,
        .w = PLAYER_SIZE,
        .h = PLAYER_SIZE 
    };
    g_boxes[2] = (SDL_Rect){
        .x = MIN_BOX_OFFSET_LEFT+50*6,
        .y = SCREEN_HEIGHT-GROUND_HEIGHT-PLAYER_SIZE*5,
        .w = PLAYER_SIZE,
        .h = PLAYER_SIZE 
    };

    int exit = 0;
    int pressed_down = 0;
    SDL_Event e;
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    double delta_time = 0;
    while (!exit) {
        last = now;
        now = SDL_GetPerformanceCounter();
        delta_time = (now - last)*1000 / (double)SDL_GetPerformanceFrequency();

        while (SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    exit = 1;
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.scancode == SDL_SCANCODE_UP || e.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                        pressed_down = 1;
                    }
                    break;
                case SDL_KEYUP:
                    if (e.key.keysym.scancode == SDL_SCANCODE_UP || e.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                        pressed_down = 0;
                    }
                    break;
                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 1);
        SDL_RenderClear(g_renderer);

        SDL_Rect* box = box_collided();

        if (box) {
            printf("%d %d | %d %d\n", g_player.x, g_player.y, box->x, box->y);
            if (g_player.y >= box->y && g_player.y <= box->y+PLAYER_SIZE) {
                printf("You died!\n");
                exit = 1;
            }
        }

        simulate_gravity(box, delta_time);
        player_jump(box, pressed_down, delta_time);
        move_boxes_left(delta_time);

        // Rendering
        SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 1);
        SDL_RenderDrawLine(g_renderer, 0, SCREEN_HEIGHT-GROUND_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT-GROUND_HEIGHT);
        SDL_RenderFillRect(g_renderer, &g_player);
        SDL_SetRenderDrawColor(g_renderer, 0x00, 0xFF, 0x00, 1);
        render_boxes();
        // ...

        SDL_RenderPresent(g_renderer);
    }
    

    return 0;
}

void simulate_gravity(SDL_Rect* box, double dt) {
    g_player.y += GRAVITY * dt;
    if (g_player.y+PLAYER_SIZE > SCREEN_HEIGHT-GROUND_HEIGHT) {
        g_player.y = SCREEN_HEIGHT-GROUND_HEIGHT-PLAYER_SIZE;
    }

    if (box && (g_player.y+PLAYER_SIZE > box->y)) {
        g_player.y = box->y-PLAYER_SIZE;
    }
}

static inline int are_rects_colliding(SDL_Rect* const r1, SDL_Rect* const r2) {
    return ((r1->x >  r2->x && r1->x <  r2->x+PLAYER_SIZE)  ||
            (r2->x >  r1->x && r2->x <  r1->x+PLAYER_SIZE)) &&
           ((r1->y >= r2->y && r1->y <= r2->y+PLAYER_SIZE)  ||
            (r2->y >= r1->y && r2->y <= r1->y+PLAYER_SIZE)); 
}

SDL_Rect* box_collided() {
    SDL_Rect* box = NULL;
    for (size_t i=0; i<g_box_count; i++) {
        if (are_rects_colliding(&g_player, &g_boxes[i])) {
            if (box == NULL || box->y > g_boxes[i].y)
                box = &g_boxes[i];
        }
    }

    return box;
}

void player_jump(SDL_Rect* box, int pressed, double dt) {
    const int jump_frames = 40;
    static int jump_len = 0;

    if (jump_len > 0) {
        g_player.y -= JUMP_SPEED*dt;
        jump_len--;
    }

    if (g_player.y < 0) {
        g_player.y = 0;
    }

    if (pressed) {
        if (player_on_ground() || (box && player_on_box(box)))
            jump_len = jump_frames;
    }
}

static inline int player_on_ground() {
    return (g_player.y+PLAYER_SIZE) == (SCREEN_HEIGHT-GROUND_HEIGHT);
}

static inline int player_on_box(SDL_Rect* box) {
    return (g_player.y+PLAYER_SIZE) == box->y;
}

void render_boxes() {
    for (size_t i=0; i<g_box_count; i++) {
        if (g_boxes[i].x > SCREEN_WIDTH || g_boxes[i].w <= 0) continue;
        SDL_RenderDrawRect(g_renderer, &g_boxes[i]) ;
    }
}

void move_boxes_left(double dt) {
    for (size_t i=0; i<g_box_count; i++) {
        if (g_boxes[i].x+PLAYER_SIZE >= 0) {
            g_boxes[i].x -= BOX_SPEED*dt;
        }
        if (g_boxes[i].x <= 0) {
            g_boxes[i].w -= BOX_SPEED*dt;
        }
    }
}
