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

void drawMove(SDL_Renderer *gRenderer, unsigned int cell, int screenWidth, int screenHeight, bool player, int cellWidth, int cellHeight);
void drawBoard(SDL_Renderer *gRenderer, int screen_w, int screen_h);
void drawCircle(SDL_Renderer *gRenderer, int xPos, int yPos, int r);

int findCurrentMove(std::array<char, 9> board);
void cellToCords(int cell, int &row, int &column);
bool arrayIncludes(char arr[], int arrLength, char searchFor);

bool checkPlayerWin(std::array<char, 9> board, int movesDone, bool player);

enum gameResults
{
    GAME_RESULT_CIRCLE_WIN,
    GAME_RESULT_CROSS_WIN,
    GAME_RESULT_TIE,
};

gameResults game(SDL_Window *gWindow, SDL_Renderer *gRenderer)
{
    SDL_Event e;
    bool endGame = false;
    gameResults gameWinner;

    std::array<GButtonTexture, 9> cells;

    for (auto &cell : cells)
    {
        cell = GButtonTexture(gRenderer);
    }

    std::array<char, 9> moves;
    moves.fill(-1);

    int screen_w, screen_h;

    SDL_GetWindowSize(gWindow, &screen_w, &screen_h);

    int cellWidth = screen_w / 3;
    int cellHeight = screen_h / 3;

    while (!endGame)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                endGame = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
            {
                int mouseX, mouseY;

                SDL_GetMouseState(&mouseX, &mouseY);

                int currentCell = mouseX / cellWidth + mouseY / cellHeight * 3;
                int currentMoveIndex = findCurrentMove(moves);

                if (arrayIncludes(moves.data(), moves.size(), currentCell))
                {
                    break;
                }

                moves[currentMoveIndex] = currentCell;

                if (currentMoveIndex == -1)
                {
                    endGame = true;
                    gameWinner = GAME_RESULT_TIE;
                    break;
                }

                if (currentMoveIndex == 8)
                {
                    endGame = true;
                    gameWinner = GAME_RESULT_TIE;
                    break;
                }
                bool currentPlayer = currentMoveIndex % 2;
                if (currentMoveIndex >= 4 && checkPlayerWin(moves, currentMoveIndex, currentPlayer))
                {
                    gameWinner = (currentPlayer ? GAME_RESULT_CROSS_WIN : GAME_RESULT_CIRCLE_WIN);
                    endGame = true;
                    break;
                }
            }
            break;
            case SDL_WINDOWEVENT:
            {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    SDL_GetWindowSize(gWindow, &screen_w, &screen_h);
                    cellWidth = screen_w / 3;
                    cellHeight = screen_h / 3;
                }
                break;
            }
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(gRenderer);

        drawBoard(gRenderer, screen_w, screen_h);

        for (int i = 0; i < 9 && moves[i] != -1; i++)
        {
            drawMove(gRenderer, moves[i], screen_w, screen_h, i % 2, cellWidth, cellHeight);
        }
        SDL_RenderPresent(gRenderer);
    }

    return gameWinner;
}
bool wonInRow(int row[], int rowLength)
{
    for (int i = 0; i < rowLength; i++)
    {
        if (row[i] == 3)
        {
            return true;
        }
    }
    return false;
}
bool checkPlayerWin(std::array<char, 9> board, int movesDone, bool player)
{
    std::array<int, 3> rows{};
    std::array<int, 3> columns{};
    std::array<int, 2> cross{};

    for (int i = 0; i <= movesDone; i++)
    {
        if (i % 2 != player)
        {
            continue;
        }

        int cell = board[i];

        int row, column;
        cellToCords(cell, row, column);

        if (cell % 2 == 0)
        {
            if (cell == 4)
            {
                cross[0]++;
                cross[1]++;
            }
            else if (cell == 0 || cell == 8)
            {
                cross[0]++;
            }
            else
            {
                cross[1]++;
            }
        }
        rows[row]++;
        columns[column]++;
    }

    return wonInRow(rows.data(), rows.size()) || wonInRow(columns.data(), columns.size()) || wonInRow(cross.data(), cross.size());
}
// @return if return = -1 board is full
int findCurrentMove(std::array<char, 9> board)
{
    for (int i = 0; i < 9; i++)
    {
        if (board[i] == -1)
        {
            return i;
        }
    }
    return -1;
}
void cellToCords(int cell, int &row, int &column)
{
    row = cell % 3;
    column = cell / 3;
}
void drawMove(SDL_Renderer *gRenderer, unsigned int cell, int screenWidth, int screenHeight, bool player, int cellWidth, int cellHeight)
{
    int row, column;

    cellToCords(cell, row, column);

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
bool arrayIncludes(char arr[], int arrLength, char searchFor)
{
    for (int i = 0; i < arrLength; i++)
    {
        if (arr[i] == searchFor)
        {
            return true;
        }
    }
    return false;
}