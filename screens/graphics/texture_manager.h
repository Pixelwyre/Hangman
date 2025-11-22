#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <stdbool.h>

//main menu textures
typedef struct {
    SDL_Texture *background;
    SDL_Texture *start;
    SDL_Texture *startHover;
    SDL_Surface *startSurface;
    SDL_Texture *about;
    SDL_Texture *aboutHover;
    SDL_Surface *aboutSurface;
} MainMenuTextures;

typedef struct {
    SDL_Texture *frames[180];
    SDL_Texture *livesTextures[7];
    SDL_Texture *pauseTex;
    SDL_Texture *lettersTex[2];
    SDL_Surface *lettersSurf[2];
    SDL_Texture *powerUI_bg;
    SDL_Texture *powerUI_boxes[9];
    SDL_Surface *powerUI_boxSurfs[9];
} IngameUITextures;

typedef struct {
    SDL_Texture *background;
} AboutSectionTextures;

extern MainMenuTextures g_mainMenuTextures;
extern IngameUITextures g_ingameUITextures;
extern AboutSectionTextures g_aboutTextures;

bool textureManagerInitMainMenu(SDL_Renderer *renderer);
bool textureManagerInitIngameUi(SDL_Renderer *renderer);
bool textureManagerInitAboutSection(SDL_Renderer *renderer);

void textureManagerDestroyMainMenu(void);
void textureManagerDestroyIngameUi(void);
void textureManagerDestroyAboutSection(void);

bool textureManagerInit(SDL_Renderer *renderer);

void textureManagerDestroyAll(void);

// ============================================================================
// THREADED LOADING API
// ============================================================================

bool textureManagerStartAsyncLoad(void);

bool textureManagerProcessLoadedSurfaces(SDL_Renderer *renderer);

//get loading progress (0.0 to 1.0)
float textureManagerGetProgress(void);

//check if surface loading is complete
bool textureManagerSurfacesLoaded(void);

//check if texture creation is complete
bool textureManagerIsFullyLoaded(void);

#endif