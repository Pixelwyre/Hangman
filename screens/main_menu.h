#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef enum {
    MENU_NONE = 0,
    MENU_START,
    MENU_ABOUT
} MenuAction;

bool mainMenuInit(SDL_Window *window, SDL_Renderer *renderer);

void mainMenuDestroy();

MenuAction mainMenuHandleEvent(SDL_Window *window, SDL_Renderer *renderer, SDL_Event *e);

void mainMenuRender(SDL_Renderer *renderer, SDL_Window *window);

#endif