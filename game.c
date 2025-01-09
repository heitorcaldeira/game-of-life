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

int nb[][2] = {
	{-1, -1}, // top left
	{-1, 0},  // top center
	{-1, 1},  // top right
	{0, 1},   // right center
	{1, 1},   // bottom right
	{1, 0},   // bottom center
	{1, -1},  //bottom left
	{0, -1},  // left center
};

int game_running = 0;
int quit = 0;
int drawing = 0;
int erasing = 0;
int mx, my;
int g_row, g_col;
int grid[GRID_HEIGHT - 1][GRID_WIDTH - 1];

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

int can_fill(int x, int y) {
  mx = x;
  my = y;
  g_row = my / CELL_SIZE;
  g_col = mx / CELL_SIZE;
  if (g_row <= 0 || g_col <= 0 || g_row >= GRID_HEIGHT - 1 || g_col >= GRID_WIDTH - 1) {
    return 0; // outside the board
  }

  return 1;
}

void poll_events() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      quit = 1;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
      if (!can_fill(event.button.x, event.button.y)) return;

      if (grid[g_row][g_col] == 1) {
        grid[g_row][g_col] = 0;
        erasing = 1;
      } 
      if (grid[g_row][g_col] == 0) {
        drawing = 1;
        grid[g_row][g_col] = 1;
      }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
      drawing = 0;
      erasing = 0;
    } else if (event.type == SDL_MOUSEMOTION) {
      if (!can_fill(event.button.x, event.button.y)) return;

      if (drawing) {
        grid[g_row][g_col] = 1;
      }
      if (erasing) {
        grid[g_row][g_col] = 0;
      } 
    } else if (event.type == SDL_KEYDOWN) {
      if (game_running) game_running = 0;
      else game_running = 1;
    }
  }
}

void matrixcopy(void * destmat, void * srcmat) 
{
  memcpy(destmat,srcmat, (GRID_HEIGHT - 1)*(GRID_WIDTH - 1) * sizeof(int));
}

void game_loop(SDL_Renderer *renderer) {
  int copy[GRID_HEIGHT - 1][GRID_WIDTH - 1];
  matrixcopy(copy, grid);
  for (int row = 0; row < GRID_HEIGHT - 1; row++) {
    for (int col = 0; col < GRID_WIDTH - 1; col++) {
      int c = grid[row][col] == 1;
      if (c) {
        SDL_SetRenderDrawColor(renderer, 0, 200, 100, 255);
      } else {
        SDL_SetRenderDrawColor(renderer, 18, 18, 18, 255);
      }

      SDL_Rect rect = { .x = col * CELL_SIZE + 1, .y = row * CELL_SIZE + 1, .w = CELL_SIZE - 1, .h = CELL_SIZE - 1};
      sdlc(SDL_RenderFillRect(renderer, &rect));

      if (game_running) {
        int live_nb = 0;
        for (int i = 0; i < 8; i++) {
          int cx = row + nb[i][0];
          int cy = col + nb[i][1];
          if (cx >= 0 && cx < GRID_HEIGHT - 1 && cy >= 0 && cy < GRID_WIDTH - 1) {
            int value = grid[cx][cy];
            if (value == 1) live_nb += 1;
          }
        }

        if (c == 1 && live_nb < 2) {
          copy[row][col] = 0;
        } 
        if (c == 1 && (live_nb == 2 || live_nb == 3)) {
          copy[row][col] = 1;
        } 
        if (c == 1 && live_nb > 3) {
          copy[row][col] = 0;
        } 
        if (c == 0 && live_nb == 3) {
          copy[row][col] = 1;
        }
      }
    }
  }

  matrixcopy(grid, copy);
}

int main() {
  sdlc(SDL_Init(SDL_INIT_VIDEO));
  SDL_Window* window = sdlp(SDL_CreateWindow("GAME OF L!FE (Press Space to start)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0));
  SDL_Renderer* renderer = sdlp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

  sdlc(SDL_SetRenderDrawColor(renderer, 18, 18, 18, 255));
  sdlc(SDL_RenderClear(renderer));

  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

  for (int row = 0; row <= GRID_HEIGHT; row++) {
    sdlc(SDL_RenderDrawLine(renderer, WIN_OFFSET, row * CELL_SIZE + WIN_OFFSET, WIDTH - WIN_OFFSET, row * CELL_SIZE + WIN_OFFSET));
  }

  for (int col = 0; col <= GRID_WIDTH; col++) {
    sdlc(SDL_RenderDrawLine(renderer, col * CELL_SIZE + WIN_OFFSET, WIN_OFFSET, col * CELL_SIZE + WIN_OFFSET, HEIGHT - WIN_OFFSET));
  }

  int lastTime = SDL_GetTicks();

  while (!quit) {
    poll_events();

    int frameStart = SDL_GetTicks();
    int delta = lastTime + 100;

    if (frameStart > delta) {
      game_loop(renderer);
      lastTime = frameStart;
    }

    SDL_RenderPresent(renderer);
  }
  
  SDL_Quit();
  return 0;
}
