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

void settings(SDL_Window *gWindow, SDL_Renderer *gRenderer, gameSettings *settings)
{
    bool quit = false;

    SDL_Event e;

    int screenW, screenH;

    SDL_GetWindowSize(gWindow, &screenW, &screenH);

    std::array<GButtonTexture, SETTING_OPTION_TOTAL> buttons = genSettingsButtons(gRenderer, screenW, screenH);

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            }
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                SDL_GetWindowSize(gWindow, &screenW, &screenH);
                setButtonsPositions(buttons, screenW, screenH);
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(gRenderer);

        for (auto &button : buttons)
        {
            button.render();
        }

        SDL_RenderPresent(gRenderer);
    }
}

void setButtonsPositions(std::array<GButtonTexture, SETTING_OPTION_TOTAL> &optionButtons, int screenW, int screenH)
{

    for (int i = 0; i < SETTING_OPTION_TOTAL; i++)
    {
        GButtonTexture &currentBtn = optionButtons[i];

        int btnXPos = screenW / 2 - currentBtn.getWidth() / 2;
        int btnYPos = screenH / SETTING_OPTION_TOTAL - currentBtn.getHeight() / 2;

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

    optionButtons[SETTING_OPTION_BOT].loadTextTexture("Play against bot", textColor, fontSize);
    optionButtons[SETTING_OPTION_VOLUME].loadTextTexture("Volume", textColor, fontSize);
    optionButtons[SETTING_OPTION_DIFFICULTY].loadTextTexture("Difficulty", textColor, fontSize);
    optionButtons[SETTING_OPTION_CIRCLE_COLOR].loadTextTexture("Circle color", textColor, fontSize);
    optionButtons[SETTING_OPTION_CROSS_COLOR].loadTextTexture("Cross color", textColor, fontSize);
    optionButtons[SETTING_OPTION_FULLSCREEN].loadTextTexture("Fullscreen", textColor, fontSize);

    setButtonsPositions(optionButtons, screenW, screenH);

    return optionButtons;
}