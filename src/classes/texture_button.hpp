#include <string>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#ifndef G_BUTTON_TEXTURE_H
#define G_BUTTON_TEXTURE_H
class GButtonTexture
{
private:
    SDL_Point btnPos = {0, 0};

    int gBtnHeight = 0;
    int gBtnWidth = 0;

    GButtonStatus mouseStatus = GButtonStatus::MOUSE_BUTTON_OUT;

    SDL_Texture *gTexture = nullptr;
    SDL_Renderer *gRenderer;

public:
    GButtonTexture(SDL_Renderer *textureRenderer);
    GButtonTexture() = default;

    GButtonStatus getButtonStatus();

    void render();

    void detectButtonsStatus(SDL_Event *e);

    bool loadImgTexture(std::string path);
    bool loadTextTexture(std::string text, SDL_Color textColor, int fontSize);

    void setBtnPos(int x, int y);

    int getHeight();
    int getWidth();

    void free();

    ~GButtonTexture();
};

int GButtonTexture::getWidth()
{
    return gBtnWidth;
}
int GButtonTexture::getHeight()
{
    return gBtnHeight;
}

GButtonTexture::GButtonTexture(SDL_Renderer *textureRenderer) : gRenderer(textureRenderer) {}

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
        IMG_Quit();
        return false;
    }

    gTexture = SDL_CreateTextureFromSurface(gRenderer, loadImgSurface);

    if (gTexture == nullptr)
    {
        SDL_FreeSurface(loadImgSurface);
        IMG_Quit();
        return false;
    }

    gBtnWidth = loadImgSurface->w;
    gBtnHeight = loadImgSurface->h;

    SDL_FreeSurface(loadImgSurface);

    return true;
}
bool GButtonTexture::loadTextTexture(std::string text, SDL_Color textColor, int fontSize)
{
    free();

    TTF_Font *textFont = TTF_OpenFont("PixelifySans-Regular.ttf", fontSize);

    if (textFont == nullptr)
    {
        TTF_CloseFont(textFont);
        TTF_Quit();
        return false;
    }

    SDL_Surface *textSurface = TTF_RenderText_Solid(textFont, text.c_str(), textColor);

    if (textSurface == nullptr)
    {
        TTF_CloseFont(textFont);
        SDL_FreeSurface(textSurface);
        TTF_Quit();
        return false;
    }

    gTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

    gBtnHeight = textSurface->h;
    gBtnWidth = textSurface->w;

    SDL_FreeSurface(textSurface);
    TTF_CloseFont(textFont);

    return true;
}

void GButtonTexture::render()
{
    SDL_Rect renderPos = {btnPos.x, btnPos.y, gBtnWidth, gBtnHeight};

    SDL_RenderCopy(gRenderer, gTexture, nullptr, &renderPos);
}

void GButtonTexture::detectButtonsStatus(SDL_Event *e)
{
    if (e->type != SDL_MOUSEMOTION && e->type != SDL_MOUSEBUTTONDOWN && e->type != SDL_MOUSEBUTTONUP)
    {
        return;
    }

    int mouse_pos_x, mouse_pos_y;

    SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);

    bool mouseInX = mouse_pos_x > btnPos.x && mouse_pos_x < btnPos.x + gBtnWidth;
    bool mouseInY = mouse_pos_y > btnPos.y && mouse_pos_y < btnPos.y + gBtnHeight;

    if (!(mouseInX && mouseInY))
    {
        mouseStatus = GButtonStatus::MOUSE_BUTTON_OUT;
        return;
    }

    switch (e->type)
    {
    case SDL_MOUSEBUTTONDOWN:
        mouseStatus = GButtonStatus::MOUSE_BUTTON_DOWN;
        break;
    case SDL_MOUSEBUTTONUP:
        mouseStatus = GButtonStatus::MOUSE_BUTTON_UP;
        break;
    default:
        mouseStatus = GButtonStatus::MOUSE_BUTTON_OVER;
    }
}

GButtonTexture::~GButtonTexture()
{
    free();
}

void GButtonTexture::free()
{
    if (gTexture != nullptr)
    {
        SDL_DestroyTexture(gTexture);
        gTexture = nullptr;
    }
    gBtnHeight = 0;
    gBtnWidth = 0;
}

GButtonStatus GButtonTexture::getButtonStatus()
{
    return mouseStatus;
}
#endif