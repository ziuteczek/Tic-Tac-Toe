#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <array>
#include <iostream>

#include "stages/menu.hpp"
#include "stages/game.hpp"

#include "classes/texture.hpp"
#include "classes/button.hpp"
#include "classes/texture_button.hpp"

void init();
void load(SDL_Window **gWindow, SDL_Renderer **gRenderer);
void close(SDL_Window *gWindow, SDL_Renderer *gRenderer);

int menu(SDL_Window *gWindow, SDL_Renderer *gRenderer);

int main(int argc, char *argv[])
{
    init();

    SDL_Window *gWindow = nullptr;
    SDL_Renderer *gRenderer = nullptr;

    load(&gWindow, &gRenderer);

    bool startGame = false;

    int menuOption = menu(gWindow, gRenderer);

    if (menuOption == MENU_OPTION_START)
    {
        game(gWindow, gRenderer);
    }

    close(gWindow, gRenderer);
}
void close(SDL_Window *gWindow, SDL_Renderer *gRenderer)
{
    SDL_DestroyWindow(gWindow);

    SDL_DestroyRenderer(gRenderer);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
void init()
{
    SDL_Init(SDL_INIT_VIDEO);

    TTF_Init();

    IMG_Init(IMG_INIT_PNG);
}
void load(SDL_Window **gWindow, SDL_Renderer **gRenderer)
{
    *gWindow = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE);
    *gRenderer = SDL_CreateRenderer(*gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}