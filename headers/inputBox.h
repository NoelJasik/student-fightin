//
// Created by mateu on 20.01.2026.
//

#ifndef UNIPROJECT_NAMEDIALOG_H
#define UNIPROJECT_NAMEDIALOG_H
#pragma once
#include <string>
#include <SDL.h>
#include <TextRenderer.h>

class InputBox {
public:
    explicit InputBox(TextRenderer* textRenderer);

    // otwarcie inputa
    void open(int x, int y, int contextType);

    // obsługa eventów SDL
    void handleEvent(const SDL_Event& e);

    // render okienka
    void render(SDL_Renderer* renderer);

    // stany
    bool isActive() const;
    bool isConfirmed() const;

    // dane
    std::string getText() const;
    int getX() const;
    int getY() const;
    int getContextType() const;

    // reset po użyciu
    void reset();

private:
    TextRenderer* textRenderer;

    bool active = false;
    bool confirmed = false;

    std::string text;

    int x = 0;
    int y = 0;
    int contextType = 0;
};
#endif //UNIPROJECT_NAMEDIALOG_H