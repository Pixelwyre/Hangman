#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "screens/main_menu.h"
#include "screens/about_section.h"
#include "screens/ingame_ui.h"
#include "screens/loading_screen.h"
#include "game/hangman.h"
#include "screens/graphics/texture_manager.h"
#include "utility/utilities.h"

#define SDL_MAIN_HANDLED

int main(int argc, char *argv[]) {
    //initialise OpenGL attributes in SDL2
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL Init failed: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        printf("TTF Init failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Hangman",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer =
            SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //initialise our renderer
    if (!renderer) {
        printf("Renderer failed: %s\n", SDL_GetError());
        return 1;
    }

    //initialize our loading screen
    if (!loadingScreenInit(window, renderer)) {
        printf("Loading screen init failed\n");
        return 1;
    }

    //start async texture loading (on background thread)
    if (!textureManagerStartAsyncLoad()) {
        printf("Failed to start texture loading\n");
        return 1;
    }

    //show loading screen while textures load
    bool loadingComplete = false;
    while (!loadingComplete) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                loadingScreenDestroy();
                textureManagerDestroyAll();
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                TTF_Quit();
                SDL_Quit();
                return 0;
            }
        }

        float progress = textureManagerGetProgress();
        loadingScreenRender(renderer, window, progress);

        //check if surfaces are loaded
        if (textureManagerSurfacesLoaded()) {
            //now convert surfaces to textures on main thread
            if (textureManagerProcessLoadedSurfaces(renderer)) {
                loadingComplete = true;
            }
        }

        SDL_Delay(16); // ~60 FPS
    }

    //destroy loading screen
    loadingScreenDestroy();

    //initialise ui menus using textures that we loaded already
    if (!mainMenuInit(window, renderer)) return 1;
    if (!aboutSectionInit(window, renderer)) return 1;

    bool shouldQuit = false;
    bool inMenu = true;
    bool inAbout = false;
    bool inGame = false;

    GameState game;

    //timing
    Uint64 lastTime = SDL_GetPerformanceCounter();

    while (!shouldQuit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            //window close
            if (event.type == SDL_QUIT) {
                shouldQuit = true;
            }

            //main menu input handling
            if (inMenu) {
                MenuAction action =
                        mainMenuHandleEvent(window, renderer, &event);

                if (action == MENU_START) {
                    char *wordFile = getRandomWordFileName();
                    char *word = getRandomWordFromFile(wordFile);
                    if (!word) {
                        printf("Failed to get word\n");
                        shouldQuit = true;
                        break;
                    }

                    game = initHangman(wordFile, word, 6);
                    free(word);

                    if (!ingameUiInit(window, renderer, &game)) {
                        printf("Ingame UI failed\n");
                        shouldQuit = true;
                        break;
                    }

                    inMenu = false;
                    inGame = true;
                } else if (action == MENU_ABOUT) {
                    inMenu = false;
                    inAbout = true;
                }
            }

            //about section input handling
            else if (inAbout) {
                if (event.type == SDL_KEYDOWN &&
                    event.key.keysym.sym == SDLK_ESCAPE) {
                    inAbout = false;
                    inMenu = true;
                }
            }

            //IN-GAME INPUT
            else if (inGame) {
                ingameUiHandleEvent(&event);
            }
        }

        //time step
        Uint64 current = SDL_GetPerformanceCounter();
        float deltaTime =
                (float) (current - lastTime) / SDL_GetPerformanceFrequency();
        lastTime = current;

        //render
        if (inMenu) {
            mainMenuRender(renderer, window);
        } else if (inAbout) {
            aboutSectionRender(renderer, window);
        } else if (inGame) {
            ingameUiUpdate(deltaTime);
            ingameUiRender(renderer, window);

            //return to menu if user pressed Esc after game over
            if (ingameUiShouldQuit()) {
                setShouldQuit(false); //reset flag
                ingameUiDestroy();
                inGame = false;
                inMenu = true;
            }
        }
    }

    //destroy screens on exit
    textureManagerDestroyAll();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}