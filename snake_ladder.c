#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define CELL_SIZE (SCREEN_WIDTH / 10)

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int snakes[][2] = {{99, 21}, {89, 33}, {76, 47}, {54, 31}, {40, 3}, {27, 5}};
int ladders[][2] = {{4, 25}, {13, 46}, {33, 49}, {50, 69}, {62, 81}, {74, 92}};

void draw_board() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i <= 10; i++) {
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, SCREEN_WIDTH, i * CELL_SIZE);
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, SCREEN_HEIGHT);
    }
}

void draw_snakes_and_ladders() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < 6; i++) {
        SDL_RenderDrawLine(renderer,
            (snakes[i][0] % 10) * CELL_SIZE + CELL_SIZE / 2, (9 - (snakes[i][0] / 10)) * CELL_SIZE + CELL_SIZE / 2,
            (snakes[i][1] % 10) * CELL_SIZE + CELL_SIZE / 2, (9 - (snakes[i][1] / 10)) * CELL_SIZE + CELL_SIZE / 2);
    }
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < 6; i++) {
        SDL_RenderDrawLine(renderer,
            (ladders[i][0] % 10) * CELL_SIZE + CELL_SIZE / 2, (9 - (ladders[i][0] / 10)) * CELL_SIZE + CELL_SIZE / 2,
            (ladders[i][1] % 10) * CELL_SIZE + CELL_SIZE / 2, (9 - (ladders[i][1] / 10)) * CELL_SIZE + CELL_SIZE / 2);
    }
}

void draw_player(int position) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    int x = (position % 10) * CELL_SIZE + CELL_SIZE / 2;
    int y = (9 - (position / 10)) * CELL_SIZE + CELL_SIZE / 2;
    SDL_Rect rect = {x - 10, y - 10, 20, 20};
    SDL_RenderFillRect(renderer, &rect);
}

int roll_dice() {
    return (rand() % 6) + 1;
}

int update_position(int position) {
    for (int i = 0; i < 6; i++) {
        if (position == snakes[i][0]) return snakes[i][1];
        if (position == ladders[i][0]) return ladders[i][1];
    }
    return position;
}

void game_loop() {
    int player = 0;
    SDL_Event event;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = 1;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                int dice = roll_dice();
                player += dice;
                if (player > 100) player -= dice;
                player = update_position(player);
                if (player == 100) printf("Player wins!\n");
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        draw_board();
        draw_snakes_and_ladders();
        draw_player(player);
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
}

int main() {
    srand(time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    window = SDL_CreateWindow("Snake and Ladder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    game_loop();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

