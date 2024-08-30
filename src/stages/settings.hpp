#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <array>
#include <iostream>
#include <vector>

#include "../classes/button.hpp"
#include "../classes/texture.hpp"
#include "../classes/texture_button.hpp"

enum gameDifficulty
{
    GAME_DIFFICULTY_EASY,
    GAME_DIFFICULTY_MEDIUM,
    GAME_DIFFICULTY_HARD,
    GAME_DIFFICULTY_TOTAL
};

enum settingsOptions
{
    SETTING_OPTION_VOLUME,
    SETTING_OPTION_BOT,
    SETTING_OPTION_DIFFICULTY,
    SETTING_OPTION_FULLSCREEN,
    SETTING_OPTION_CIRCLE_COLOR,
    SETTING_OPTION_CROSS_COLOR,
    SETTING_OPTION_EXIT,
    SETTING_OPTION_TOTAL
};

struct gameSettings
{
    Uint8 volume = 128;
    bool bot = false;
    gameDifficulty difficulty = GAME_DIFFICULTY_MEDIUM;
    SDL_Color circle_color = {0, 0, 0};
    SDL_Color cross_color = {0, 0, 0};
};

std::array<GButtonTexture, SETTING_OPTION_TOTAL> genSettingsButtons(SDL_Renderer *gRenderer, int screenW, int screenH);
void setButtonsPositions(std::array<GButtonTexture, SETTING_OPTION_TOTAL> &optionButtons, int screenW, int screenH);

bool settings(SDL_Window *gWindow, SDL_Renderer *gRenderer, gameSettings *settings, Mix_Chunk *hoverSound)
{
    bool quit = false;
    bool closeGame = false;

    SDL_Cursor *pointer = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    SDL_Cursor *arrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);

    SDL_Event e;

    int screenW, screenH;

    SDL_GetWindowSize(gWindow, &screenW, &screenH);

    std::array<GButtonTexture, SETTING_OPTION_TOTAL> buttons = genSettingsButtons(gRenderer, screenW, screenH);

    while (!quit)
    {
        bool btnHoverd = false;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                quit = true;
                closeGame = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                for (auto &optionBtn : buttons)
                {
                    optionBtn.detectButtonsStatus(&e);
                    if (optionBtn.getButtonStatus() == MOUSE_BUTTON_DOWN)
                    {
                        optionBtn.checked = !optionBtn.checked;
                    }
                }
                break;
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    SDL_GetWindowSize(gWindow, &screenW, &screenH);
                    setButtonsPositions(buttons, screenW, screenH);
                }
                break;
            }
        }
        for (auto &optionButton : buttons)
        {
            if (optionButton.getButtonStatus() == MOUSE_BUTTON_OVER)
            {
                btnHoverd = true;
                break;
            }
        }
        if (buttons[SETTING_OPTION_EXIT].getButtonStatus() == MOUSE_BUTTON_DOWN)
        {
            quit = true;
        }
        if (btnHoverd)
        {
            if (SDL_GetCursor() != arrow)
            {
                SDL_SetCursor(pointer);
                Mix_PlayChannel(-1, hoverSound, 0);
            }
        }
        else
        {
            SDL_SetCursor(arrow);
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(gRenderer);

        for (auto &button : buttons)
        {
            button.render();
        }

        SDL_RenderPresent(gRenderer);
    }

    SDL_FreeCursor(pointer);
    SDL_FreeCursor(arrow);

    for (auto &btn : buttons)
    {
        btn.free();
    }
    return closeGame;
}

void setButtonsPositions(std::array<GButtonTexture, SETTING_OPTION_TOTAL> &optionButtons, int screenW, int screenH)
{
    for (int i = 0; i < SETTING_OPTION_TOTAL; i++)
    {
        GButtonTexture &currentBtn = optionButtons[i];

        int btnXPos = (screenW - currentBtn.getWidth() - currentBtn.getPadding(PADDING_DIRECTION_RIGHT) + currentBtn.getPadding(PADDING_DIRECTION_LEFT)) / 2;
        int btnYPos = screenH / SETTING_OPTION_TOTAL - (currentBtn.getHeight() - currentBtn.getPadding(PADDING_DIRECTION_TOP) + currentBtn.getPadding(PADDING_DIRECTION_BOTTOM)) / 2;

        btnYPos *= i + 1;

        currentBtn.setBtnPos(btnXPos, btnYPos);
    }
}

std::array<GButtonTexture, SETTING_OPTION_TOTAL> genSettingsButtons(SDL_Renderer *gRenderer, int screenW, int screenH)
{
    std::array<GButtonTexture, SETTING_OPTION_TOTAL> optionButtons;

    for (auto &button : optionButtons)
    {
        button.setRenderer(gRenderer);
    }

    int fontSize = 35;
    SDL_Color textColor = {0, 0, 0};

    optionButtons[SETTING_OPTION_VOLUME].loadTextTexture("Volume", textColor, fontSize);
    optionButtons[SETTING_OPTION_BOT].loadTextTexture("Play against bot", textColor, fontSize, BUTTON_TYPE_CHECK);
    optionButtons[SETTING_OPTION_DIFFICULTY].loadTextTexture("Difficulty", textColor, fontSize);
    optionButtons[SETTING_OPTION_FULLSCREEN].loadTextTexture("Fullscreen", textColor, fontSize, BUTTON_TYPE_CHECK);
    optionButtons[SETTING_OPTION_CIRCLE_COLOR].loadTextTexture("Circle color", textColor, fontSize);
    optionButtons[SETTING_OPTION_CROSS_COLOR].loadTextTexture("Cross color", textColor, fontSize);
    optionButtons[SETTING_OPTION_EXIT].loadTextTexture("Exit", textColor, fontSize);

    setButtonsPositions(optionButtons, screenW, screenH);

    return optionButtons;
}