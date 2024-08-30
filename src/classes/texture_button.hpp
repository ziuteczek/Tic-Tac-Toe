#include <string>
#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "texture.hpp"

#ifndef G_BUTTON_TEXTURE_H
#define G_BUTTON_TEXTURE_H

enum btnType
{
    BUTTON_TYPE_NONE,
    BUTTON_TYPE_CHECK,
    BUTTON_TYPE_RANGE,
    BUTTON_TYPE_COLOR,
    BUTTON_TYPE_LIST,
    BUTTON_TYPES_TOTAL
};
enum paddingDirections
{
    PADDING_DIRECTION_TOP,
    PADDING_DIRECTION_RIGHT,
    PADDING_DIRECTION_BOTTOM,
    PADDING_DIRECTION_LEFT
};
struct paddingSizes
{
    int top = 0;
    int right = 0;
    int bottom = 0;
    int left = 0;
};

class GButtonTexture
{
private:
    SDL_Point btnPos = {0, 0};

    int gBtnHeight = 0;
    int gBtnWidth = 0;
    int clickHeight = 0;
    int clickWidth = 0;

    GButtonStatus mouseStatus = MOUSE_BUTTON_OUT;

    SDL_Texture *gTexture = nullptr;
    SDL_Renderer *gRenderer = nullptr;

    paddingSizes padding;

    std::vector<GTexture> optionsTextTextures;

    void (GButtonTexture::*clickDrawFunction)(void) = nullptr;

    btnType buttonType = BUTTON_TYPE_NONE;

    void drawCheckBtn();
    void drawRangeBtn();
    void drawListBtn();

public:
    GButtonTexture(SDL_Renderer *textureRenderer);
    GButtonTexture() = default;

    void setRenderer(SDL_Renderer *gRenderer);

    GButtonStatus getButtonStatus();

    void render();

    void detectButtonsStatus(SDL_Event *e);

    bool loadImgTexture(std::string path);
    bool loadTextTexture(std::string text, SDL_Color textColor, int fontSize, btnType buttonType = BUTTON_TYPE_NONE);
    void loadListOptions(std::vector<std::string> optionsTxt, int fontSize = 20);

    void setBtnPos(int x, int y);

    int getHeight();
    int getWidth();
    int getPadding(paddingDirections paddingDirection);

    bool checked = false;

    void free();

    ~GButtonTexture();
};
void GButtonTexture::loadListOptions(std::vector<std::string> optionsTxt, int fontSize /*= 20*/)
{
    if (optionsTxt.size() == 0)
    {
        return;
    }

    int optionsQuantity = optionsTxt.size();
    int32_t optionWidth = INT32_MIN;

    optionsTextTextures.resize(optionsQuantity);

    for (int i = 0; i < optionsQuantity; i++)
    {
        GTexture optionTexture(gRenderer);
        optionTexture.loadTextTexture(optionsTxt[i], {0, 0, 0}, fontSize);

        optionWidth = std::max(optionWidth, optionTexture.getWidth());

        optionsTextTextures[i] = optionTexture;
    }
    clickHeight = optionsTextTextures[0].getHeight();
    clickWidth = optionWidth;
}
int GButtonTexture::getPadding(paddingDirections paddingDirection)
{
    int paddingToReturn;
    switch (paddingDirection)
    {
    case PADDING_DIRECTION_TOP:
        paddingToReturn = padding.top;
        break;
    case PADDING_DIRECTION_RIGHT:
        paddingToReturn = padding.right;
        break;
    case PADDING_DIRECTION_BOTTOM:
        paddingToReturn = padding.bottom;
        break;
    case PADDING_DIRECTION_LEFT:
        paddingToReturn = padding.left;
        break;
    }
    return paddingToReturn;
}

GButtonTexture::GButtonTexture(SDL_Renderer *textureRenderer) : gRenderer(textureRenderer) {}

int GButtonTexture::getWidth()
{
    return gBtnWidth;
}
int GButtonTexture::getHeight()
{
    return gBtnHeight;
}

void GButtonTexture::setBtnPos(int x, int y)
{
    btnPos = {x, y};
}

bool GButtonTexture::loadImgTexture(std::string path)
{
    free();

    SDL_Surface *loadImgSurface = IMG_Load(path.c_str());

    if (loadImgSurface == nullptr)
    {
        SDL_FreeSurface(loadImgSurface);
        return false;
    }

    gTexture = SDL_CreateTextureFromSurface(gRenderer, loadImgSurface);

    if (gTexture == nullptr)
    {
        SDL_FreeSurface(loadImgSurface);
        return false;
    }

    gBtnWidth = loadImgSurface->w;
    gBtnHeight = loadImgSurface->h;

    SDL_FreeSurface(loadImgSurface);

    return true;
}
bool GButtonTexture::loadTextTexture(std::string text, SDL_Color textColor, int fontSize, btnType buttonType /*= BUTTON_TYPE_NONE*/)
{
    free();

    this->buttonType = buttonType;

    TTF_Font *textFont = TTF_OpenFont("PixelifySans-Regular.ttf", fontSize);

    if (textFont == nullptr)
    {
        TTF_CloseFont(textFont);
        return false;
    }

    SDL_Surface *textSurface = TTF_RenderText_Solid(textFont, text.c_str(), textColor);

    if (textSurface == nullptr)
    {
        TTF_CloseFont(textFont);
        SDL_FreeSurface(textSurface);
        return false;
    }

    gTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

    gBtnHeight = textSurface->h;
    gBtnWidth = textSurface->w;

    switch (this->buttonType)
    {
    case BUTTON_TYPE_CHECK:
        padding.right = gBtnHeight;
        clickDrawFunction = &GButtonTexture::drawCheckBtn;
        break;
    case BUTTON_TYPE_LIST:
        int optionWidth = 0;
        for (auto &optionTexture : optionsTextTextures)
        {
            optionWidth = std::max(optionWidth, optionTexture.getWidth());
        }
        break;
    }

    SDL_FreeSurface(textSurface);
    TTF_CloseFont(textFont);

    return true;
}

void GButtonTexture::drawCheckBtn()
{
    SDL_Rect outlineRect = {btnPos.x + gBtnWidth, btnPos.y, gBtnHeight, gBtnHeight};

    SDL_Color previousRenderColor;

    SDL_GetRenderDrawColor(gRenderer, &previousRenderColor.r, &previousRenderColor.g, &previousRenderColor.b, &previousRenderColor.a);

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);

    SDL_RenderDrawRect(gRenderer, &outlineRect);

    if (checked)
    {
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
        SDL_Rect clickRect = {btnPos.x + gBtnWidth + gBtnHeight / 10, btnPos.y + gBtnHeight / 10, int(gBtnHeight * 0.8f), int(gBtnHeight * 0.8f)};
        SDL_RenderFillRect(gRenderer, &clickRect);
    }
    SDL_SetRenderDrawColor(gRenderer, previousRenderColor.r, previousRenderColor.g, previousRenderColor.b, previousRenderColor.a);
}
void GButtonTexture::drawRangeBtn()
{
    SDL_Color previousRenderColor;
    SDL_GetRenderDrawColor(gRenderer, &previousRenderColor.r, &previousRenderColor.g, &previousRenderColor.b, &previousRenderColor.a);
    SDL_SetRenderDrawColor(gRenderer, 0x50, 0x50, 0x50, SDL_ALPHA_OPAQUE);

    SDL_SetRenderDrawColor(gRenderer, previousRenderColor.r, previousRenderColor.g, previousRenderColor.b, previousRenderColor.a);
}
void GButtonTexture::drawListBtn()
{
    SDL_Color previousRenderColor;
    SDL_GetRenderDrawColor(gRenderer, &previousRenderColor.r, &previousRenderColor.g, &previousRenderColor.b, &previousRenderColor.a);
    SDL_SetRenderDrawColor(gRenderer, 0x50, 0x50, 0x50, SDL_ALPHA_OPAQUE);

    SDL_SetRenderDrawColor(gRenderer, previousRenderColor.r, previousRenderColor.g, previousRenderColor.b, previousRenderColor.a);
}
void GButtonTexture::render()
{
    SDL_Rect renderPos = {btnPos.x, btnPos.y, gBtnWidth, gBtnHeight};

    SDL_RenderCopy(gRenderer, gTexture, nullptr, &renderPos);

    if (clickDrawFunction != nullptr)
    {
        (this->*clickDrawFunction)();
    }
}

void GButtonTexture::detectButtonsStatus(SDL_Event *e)
{
    if (e->type != SDL_MOUSEMOTION && e->type != SDL_MOUSEBUTTONDOWN && e->type != SDL_MOUSEBUTTONUP)
    {
        return;
    }

    int mouse_pos_x, mouse_pos_y;

    SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);

    bool mouseInX = mouse_pos_x > btnPos.x - padding.left && mouse_pos_x < btnPos.x + gBtnWidth + padding.right;
    bool mouseInY = mouse_pos_y > btnPos.y - padding.top && mouse_pos_y < btnPos.y + gBtnHeight + padding.bottom;

    if (!(mouseInX && mouseInY))
    {
        mouseStatus = MOUSE_BUTTON_OUT;
        return;
    }

    switch (e->type)
    {
    case SDL_MOUSEBUTTONDOWN:
        mouseStatus = MOUSE_BUTTON_DOWN;
        break;
    case SDL_MOUSEBUTTONUP:
        mouseStatus = MOUSE_BUTTON_UP;
        break;
    default:
        mouseStatus = MOUSE_BUTTON_OVER;
    }
}

GButtonTexture::~GButtonTexture()
{
    free();
}

void GButtonTexture::free()
{
    buttonType = BUTTON_TYPE_NONE;

    if (gTexture != nullptr)
    {
        SDL_DestroyTexture(gTexture);
        gTexture = nullptr;
    }
    gBtnHeight = 0;
    gBtnWidth = 0;

    padding = {0, 0, 0, 0};

    optionsTextTextures.clear();
}

GButtonStatus GButtonTexture::getButtonStatus()
{
    return mouseStatus;
}
void GButtonTexture::setRenderer(SDL_Renderer *gRenderer)
{
    this->gRenderer = gRenderer;
}
#endif