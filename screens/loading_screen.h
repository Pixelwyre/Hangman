#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include <SDL2/SDL.h>
#include <stdbool.h>

bool loadingScreenInit(SDL_Window *window, SDL_Renderer *renderer);

void loadingScreenRender(SDL_Renderer *renderer, SDL_Window *window, float progress);

void loadingScreenDestroy(void);

#endif