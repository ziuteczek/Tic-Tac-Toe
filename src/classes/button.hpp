#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <iostream>

#ifndef BUTTON_STATUS_H
#define BUTTON_STATUS_H
enum GButtonStatus
{
    MOUSE_BUTTON_OUT,
    MOUSE_BUTTON_OVER,
    MOUSE_BUTTON_DOWN,
    MOUSE_BUTTON_UP,
    MOUSE_BUTTON_TOTAL
};
#endif

#ifndef GBUTTON_H
#define GBUTTON_H
class GButton
{
private:
    SDL_Point btnPos = {0, 0};
    int gBtnHeight = 0;
    int gBtnWidth = 0;
    GButtonStatus mouseStatus = GButtonStatus::MOUSE_BUTTON_OUT;

public:
    GButton(int x, int y, int w, int h);
    GButtonStatus getMouseStatus();
    void detectButtonsStatus(SDL_Event *e);
};

GButton::GButton(int x, int y, int w, int h) : btnPos({x, y}), gBtnHeight(h), gBtnWidth(w) {}

void GButton::detectButtonsStatus(SDL_Event *e)
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

GButtonStatus GButton::getMouseStatus()
{
    return mouseStatus;
}
#endif
