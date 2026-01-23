//
// Created by Dawid Roś on 25/11/2025.
//
#include <SDL.h>
#include "../headers/button.h"

#include "topBar.h"

Button::Button() = default;



bool Button::handleEvent(const SDL_Event& e, int& current_tower)
{
    if (e.type == SDL_MOUSEBUTTONDOWN &&
        e.button.button == SDL_BUTTON_LEFT &&
        e.button.x >= rect.x &&
        e.button.x <= rect.x + rect.w &&
        e.button.y >= rect.y &&
        e.button.y <= rect.y + rect.h)
    {
        if (!isActionButton)
            current_tower = towerId;

        return true;
    }
    return false;
}

void Button::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}
