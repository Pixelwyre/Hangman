#ifndef ABOUT_SECTION_H
#define ABOUT_SECTION_H

#include <SDL2/SDL.h>
#include <stdbool.h>

bool aboutSectionInit(SDL_Window *window, SDL_Renderer *renderer);

void aboutSectionDestroy();

void aboutSectionRender(SDL_Renderer *renderer, SDL_Window *window);

#endif
