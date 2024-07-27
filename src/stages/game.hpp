#include <iostream>
#include <array>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../classes/button.hpp"
#include "../classes/texture.hpp"
#include "../classes/texture_button.hpp"

#define WINDOW_PADDING 0.10f
#define CELL_PADDING 0.05f

void drawMove(SDL_Renderer *gRenderer, unsigned int cell, int screenWidth, int screenHeight, bool player);
void drawBoard(SDL_Renderer *gRenderer, int screen_w, int screen_h);
void drawCircle(SDL_Renderer *gRenderer, int xPos, int yPos, int r);

char game(SDL_Window *gWindow, SDL_Renderer *gRenderer)
{
    SDL_Event e;
    bool endGame = false;

    std::array<GButtonTexture, 9> cells;

    for (auto &cell : cells)
    {
        cell = GButtonTexture(gRenderer);
    }
    /*
     Every index represent player move, even indexes are o player, odd indexes are x player
     Value represents cell where move was done counting from left to right, from top to bottom
     eg. index 3 of value 6 represens player x move to right center cell
     */
    std::array<char, 9> moves;
    moves.fill(-1);
    moves[0] = 2;
    moves[1] = 3;

    int screen_w, screen_h;

    SDL_GetWindowSize(gWindow, &screen_w, &screen_h);

    while (!endGame)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                endGame = true;
                break;
            }
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                SDL_GetWindowSize(gWindow, &screen_w, &screen_h);
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(gRenderer);

        drawBoard(gRenderer, screen_w, screen_h);

        for (int i = 0; i < 9 && moves[i] != -1; i++)
        {
            drawMove(gRenderer, moves[i], screen_w, screen_h, i % 2);
        }
        SDL_RenderPresent(gRenderer);
    }

    return 0;
}

void drawMove(SDL_Renderer *gRenderer, unsigned int cell, int screenWidth, int screenHeight, bool player)
{
    int row = cell % 3;
    int column = cell / 3;

    int cellWidth = screenWidth / 3;
    int cellHeight = screenHeight / 3;

    SDL_Rect clip = {0, 0, cellWidth, cellHeight};

    SDL_Rect k = {cellWidth * row, cellHeight * column, cellWidth, cellHeight};
    SDL_Rect scren = {0, 0, screenWidth, screenHeight};

    SDL_RenderSetViewport(gRenderer, &k);

    if (!player)
    {
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        int circleRadius = ((cellHeight > cellWidth) ? cellWidth : cellHeight) / 2 * 0.9;
        drawCircle(gRenderer, cellWidth / 2, cellHeight / 2, circleRadius);
    }
    else
    {
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(gRenderer, 0, 0, cellWidth, cellHeight);
        SDL_RenderDrawLine(gRenderer, cellWidth, 0, 0, cellHeight);
    }

    SDL_RenderSetViewport(gRenderer, &scren);
}
void drawBoard(SDL_Renderer *gRenderer, int screen_w, int screen_h)
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);

    SDL_RenderDrawLine(gRenderer, screen_w * 1 / 3, 0, screen_w * 1 / 3, screen_h);
    SDL_RenderDrawLine(gRenderer, screen_w * 2 / 3, 0, screen_w * 2 / 3, screen_h);

    SDL_RenderDrawLine(gRenderer, 0, screen_h * 1 / 3, screen_w, screen_h * 1 / 3);
    SDL_RenderDrawLine(gRenderer, 0, screen_h * 2 / 3, screen_w, screen_h * 2 / 3);
}
void drawCircle(SDL_Renderer *gRenderer, int xPos, int yPos, int r)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;

    while (y >= x)
    {
        SDL_RenderDrawPoint(gRenderer, xPos + x, yPos + y);
        SDL_RenderDrawPoint(gRenderer, xPos - x, yPos + y);
        SDL_RenderDrawPoint(gRenderer, xPos + x, yPos - y);
        SDL_RenderDrawPoint(gRenderer, xPos - x, yPos - y);
        SDL_RenderDrawPoint(gRenderer, xPos + y, yPos + x);
        SDL_RenderDrawPoint(gRenderer, xPos - y, yPos + x);
        SDL_RenderDrawPoint(gRenderer, xPos + y, yPos - x);
        SDL_RenderDrawPoint(gRenderer, xPos - y, yPos - x);

        x++;

        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
    }
}
