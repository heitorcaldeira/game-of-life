#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define WIDTH    800
#define HEIGHT   600

int sdlc(int c) {
  if (c != 0) {
    printf("Something went wrong with sdl: %s", SDL_GetError());
    exit(1);
  }

  return c;
}

void *sdlp(void *p) {
  printf("Pointer: %p", p);
  if (p == NULL) {
    printf("Something went wrong with sdl: %s", SDL_GetError());
    exit(1);
  }

  return p;
}

int main() {
  sdlc(SDL_Init(SDL_INIT_VIDEO));
  SDL_Window* window = sdlp(SDL_CreateWindow("GAME OF L!FE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0));
  SDL_Renderer* renderer = sdlp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

  sdlc(SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0));
  sdlc(SDL_RenderClear(renderer));
  SDL_RenderPresent(renderer);

  SDL_Delay(2000);
  SDL_Quit();
  return 0;
}
