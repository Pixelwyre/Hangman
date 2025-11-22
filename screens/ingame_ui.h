#ifndef INGAME_UI_H
#define INGAME_UI_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../game/hangman.h"

bool ingameUiInit(SDL_Window *window, SDL_Renderer *renderer, GameState *game);

void ingameUiDestroy();

void ingameUiUpdate(float deltaTime);

void ingameUiRender(SDL_Renderer *renderer, SDL_Window *window);

void ingameUiHandleEvent(SDL_Event *event);

bool ingameUiIsWaitingAfterGameover(void);

bool ingameUiIsGameOver(void);

void setShouldQuit(bool b);

bool ingameUiShouldQuit(void);

#endif