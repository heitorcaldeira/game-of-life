#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#define WIDTH         800
#define HEIGHT        600
#define CELL_SIZE     20
#define WIN_OFFSET    20
#define GRID_WIDTH    WIDTH / CELL_SIZE
#define GRID_HEIGHT   HEIGHT / CELL_SIZE

int grid[GRID_HEIGHT - 1][GRID_WIDTH - 1];
size_t quit = 0;
size_t drawing = 0;
int mx, my;

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

    if (event.type == SDL_MOUSEBUTTONDOWN) {
      drawing = 1;
    }

    if (event.type == SDL_MOUSEBUTTONUP) {
      drawing = 0;
    }

    if (event.type == SDL_MOUSEMOTION) {
      mx = event.button.x;
      my = event.button.y;
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

  if (drawing) {
    int g_row = my / CELL_SIZE;
    int g_col = mx / CELL_SIZE;

    if (g_row == 0 || g_col == 0 || g_row == GRID_HEIGHT - 1 || g_col == GRID_WIDTH - 1) {
      return; // outside the board
    }

    if (grid[g_row][g_col] == 1) {
      grid[g_row][g_col] = 0;
    } else {
      grid[g_row][g_col] = 1;
    }
  }

  for (size_t row = 0; row < GRID_HEIGHT; row++) {
    for (size_t col = 0; col < GRID_WIDTH; col++) {
      if (grid[row][col] == 1) {
        SDL_SetRenderDrawColor(renderer, 0, 200, 100, 255);
      } else {
        SDL_SetRenderDrawColor(renderer, 18, 18, 18, 255);
      }

      SDL_Rect rect = { .x = col * CELL_SIZE + 1, .y = row * CELL_SIZE + 1, .w = CELL_SIZE - 1, .h = CELL_SIZE - 1};
      SDL_RenderFillRect(renderer, &rect);
    }
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
    SDL_Delay(100);
  }
  
  SDL_Quit();
  return 0;
}
