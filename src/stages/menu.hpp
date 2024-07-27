#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <array>
#include <iostream>

#include "../classes/button.hpp"
#include "../classes/texture.hpp"
#include "../classes/texture_button.hpp"

enum menuOptions
{
    MENU_OPTION_START,
    MENU_OPTION_OPTIONS,
    MENU_OPTION_QUIT,
    MENU_OPTIONS_TOTAL
};

void centerMenuElements(SDL_Window *gWindow, std::array<GButtonTexture, MENU_OPTIONS_TOTAL> &optionButtons)
{
    int windowHeight, windowWidth;

    SDL_GetWindowSize(gWindow, &windowWidth, &windowHeight);

    int eachOptionSeparation = windowHeight / (MENU_OPTIONS_TOTAL + 1);

    for (int i = 0; i < MENU_OPTIONS_TOTAL; ++i)
    {
        int buttonCenteredXPos = windowWidth / 2 - optionButtons[i].getWidth() / 2;
        int buttonYPos = (i + 1) * eachOptionSeparation - optionButtons[i].getHeight() / 2;
        optionButtons[i].setBtnPos(buttonCenteredXPos, buttonYPos);
    }
}
/*
    @return
    0 - Esc
    1 - Options
    2 - Start game
*/
int menu(SDL_Window *gWindow, SDL_Renderer *gRenderer)
{
    bool quitMenu = false;
    menuOptions chosenOption;
    SDL_Event e;

    std::array<GButtonTexture, MENU_OPTIONS_TOTAL> optionButtons = {GButtonTexture(gRenderer), GButtonTexture(gRenderer), GButtonTexture(gRenderer)};

    const SDL_Color menuTextColor = {0x00, 0x00, 0x00};
    const int menuFontSize = 55;

    optionButtons[MENU_OPTION_START].loadTextTexture("START", menuTextColor, menuFontSize);
    optionButtons[MENU_OPTION_OPTIONS].loadTextTexture("OPTIONS", menuTextColor, menuFontSize);
    optionButtons[MENU_OPTION_QUIT].loadTextTexture("QUIT", menuTextColor, menuFontSize);

    SDL_Cursor *arrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    SDL_Cursor *click = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    centerMenuElements(gWindow, optionButtons);

    while (!quitMenu)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                quitMenu = true;
                chosenOption = MENU_OPTION_QUIT;
                break;
            }
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                centerMenuElements(gWindow, optionButtons);
            }
            for (int i = 0; i < MOUSE_BUTTON_TOTAL; i++)
            {
                optionButtons[i].detectButtonsStatus(&e);

                GButtonStatus currentButtonsStatus = optionButtons[i].getButtonStatus();

                if (optionButtons[i].getButtonStatus() == MOUSE_BUTTON_OVER)
                {
                    SDL_SetCursor(click);
                    break;
                }
                else
                {
                    SDL_SetCursor(arrow);
                }

                if (currentButtonsStatus == MOUSE_BUTTON_DOWN)
                {
                    chosenOption = *reinterpret_cast<menuOptions *>(&i);
                    quitMenu = true;
                }
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(gRenderer);

        for (auto &menuButton : optionButtons)
        {
            menuButton.render();
        }
        SDL_RenderPresent(gRenderer);
    }
    SDL_FreeCursor(arrow);
    SDL_FreeCursor(click);

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(gRenderer);

    for (auto &buttons : optionButtons)
    {
        buttons.free();
    }

    return chosenOption;
}