//
// Created by mateu on 21.01.2026.
//

#ifndef UNIPROJECT_TOPBAR_H
#define UNIPROJECT_TOPBAR_H
#pragma once
#include <SDL.h>
#include <vector>
#include "button.h"
#include "TextRenderer.h"
class topBar {
public:
    SDL_Rect rect{};
    std::vector<Button> buttons;

    topBar(int screenWidth);
    Button startWaveButton;
    bool startWaveClicked = false;
    bool handleEvent(const SDL_Event& e, int& current_tower);
    void render(SDL_Renderer* renderer, int current_tower, int money,  TextRenderer& text, int currentWave, bool activeWave, float playerHealth);
    void resetStartWaveClicked();
};
#endif //UNIPROJECT_TOPBAR_H