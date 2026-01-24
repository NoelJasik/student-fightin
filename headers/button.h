//
// Created by Dawid Roś on 25/11/2025.
//

#ifndef UNIPROJECT_BUTTON_H
#define UNIPROJECT_BUTTON_H
#pragma once
#include <SDL.h>

class Button {
public:
    Button();
    bool isActionButton = false;
    bool handleEvent(const SDL_Event& e, int& current_tower);
    void render(SDL_Renderer* renderer);
    SDL_Rect rect{};
    SDL_Color color = {255, 255, 255, 255};
    int towerId = 0;
};


#endif //UNIPROJECT_BUTTON_H