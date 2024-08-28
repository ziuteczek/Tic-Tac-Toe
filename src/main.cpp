#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <array>
#include <iostream>

#include "stages/menu.hpp"
#include "stages/game.hpp"
#include "stages/settings.hpp"

#include "classes/texture.hpp"
#include "classes/button.hpp"
#include "classes/texture_button.hpp"

void init();
void load(SDL_Window **gWindow, SDL_Renderer **gRenderer, Mix_Chunk **hoverSound);
void close(SDL_Window *gWindow, SDL_Renderer *gRenderer, Mix_Chunk *hoverSound);

int main(int argc, char *argv[])
{
    init();

    SDL_Window *gWindow = nullptr;
    SDL_Renderer *gRenderer = nullptr;

    Mix_Chunk *hoverSound = nullptr;

    gameSettings gameSettings;

    load(&gWindow, &gRenderer, &hoverSound);

    bool startGame = false;

    int menuOption = menu(gWindow, gRenderer, hoverSound);


    switch (menuOption)
    {
    case MENU_OPTION_START:
        game(gWindow, gRenderer);
        break;
    case MENU_OPTION_OPTIONS:
        settings(gWindow, gRenderer, &gameSettings, hoverSound);
        break;
    }

    close(gWindow, gRenderer, hoverSound);
}
void close(SDL_Window *gWindow, SDL_Renderer *gRenderer, Mix_Chunk *hoverSound)
{
    SDL_DestroyWindow(gWindow);

    SDL_DestroyRenderer(gRenderer);

    Mix_FreeChunk(hoverSound);

    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}
void init()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    TTF_Init();

    IMG_Init(IMG_INIT_PNG);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}
void load(SDL_Window **gWindow, SDL_Renderer **gRenderer, Mix_Chunk **hoverSound)
{
    *gWindow = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE);
    *gRenderer = SDL_CreateRenderer(*gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    *hoverSound = Mix_LoadWAV("sound/hover.wav");
}