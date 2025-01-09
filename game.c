#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define WIDTH         800
#define HEIGHT        600
#define CELL_SIZE     20
#define WIN_OFFSET    20
#define GRID_WIDTH    WIDTH / CELL_SIZE
#define GRID_HEIGHT   HEIGHT / CELL_SIZE

size_t quit = 0;

int sdlc(int c) {
  if (c != 0) {
    printf("Something went wrong with sdl: %s", SDL_GetError());
    exit(1);
  }

  return c;
}

void *sdlp(void *p) {
  if (p == NULL) {
    printf("Something went wrong with sdl: %s", SDL_GetError());
    exit(1);
  }

  return p;
}

void poll_events() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      quit = 1;
    }
  }
}

void game_loop(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

  for (size_t row = 0; row <= GRID_HEIGHT; row++) {
    SDL_RenderDrawLine(renderer, WIN_OFFSET, row * CELL_SIZE + WIN_OFFSET, WIDTH - WIN_OFFSET, row * CELL_SIZE + WIN_OFFSET);
  }

  for (size_t col = 0; col <= GRID_WIDTH; col++) {
    SDL_RenderDrawLine(renderer, col * CELL_SIZE + WIN_OFFSET, WIN_OFFSET, col * CELL_SIZE + WIN_OFFSET, HEIGHT - WIN_OFFSET);
  }
}

int main() {
  sdlc(SDL_Init(SDL_INIT_VIDEO));
  SDL_Window* window = sdlp(SDL_CreateWindow("GAME OF L!FE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0));
  SDL_Renderer* renderer = sdlp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

  sdlc(SDL_SetRenderDrawColor(renderer, 18, 18, 18, 255));
  sdlc(SDL_RenderClear(renderer));

  while (!quit) {
    poll_events();
    game_loop(renderer);
    SDL_RenderPresent(renderer);
  }
  
  SDL_Quit();
  return 0;
}
