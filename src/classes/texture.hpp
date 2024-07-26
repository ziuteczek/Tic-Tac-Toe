#include <string>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#ifndef G_TEXTURE_H
#define G_TEXTURE_H
class GTexture
{
private:
    int gHeight = 0;
    int gWidth = 0;

    SDL_Texture *gTexture = nullptr;

    SDL_Renderer *gRenderer;

public:
    GTexture(SDL_Renderer *textureRenderer) : gRenderer(textureRenderer) {}

    bool loadImgTexture(std::string path);
    bool loadTextTexture(std::string text, SDL_Color textColor, int fontSize);

    void render(int x, int y, SDL_Rect *clip);

    int getHeight();
    int getWidth();

    void free();

    ~GTexture();
};
bool GTexture::loadImgTexture(std::string path)
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

    gWidth = loadImgSurface->w;
    gHeight = loadImgSurface->h;

    SDL_FreeSurface(loadImgSurface);

    return true;
}
bool GTexture::loadTextTexture(std::string text, SDL_Color textColor, int fontSize)
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

    gHeight = textSurface->h;
    gWidth = textSurface->w;

    SDL_FreeSurface(textSurface);
    TTF_CloseFont(textFont);

    return true;
}
void GTexture::render(int x, int y, SDL_Rect *clip)
{
    SDL_Rect target = {x, y, gWidth, gHeight};

    if (clip != nullptr)
    {
        target.w = clip->w;
        target.y = clip->y;
    }

    SDL_RenderCopy(gRenderer, gTexture, clip, &target);
}
int GTexture::getHeight()
{
    return gHeight;
}
int GTexture::getWidth()
{
    return gWidth;
}
void GTexture::free()
{
    if (gTexture != nullptr)
    {
        SDL_DestroyTexture(gTexture);
        gTexture = nullptr;
    }

    gHeight = 0;
    gWidth = 0;
}
GTexture::~GTexture()
{
    free();
}
#endif