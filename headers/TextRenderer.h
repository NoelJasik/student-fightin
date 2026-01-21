//
// Created by mateu on 20.01.2026.
//

#ifndef UNIPROJECT_TEXTRENDERER_H
#define UNIPROJECT_TEXTRENDERER_H
#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <cmath>
#include <sstream>
#include <iomanip>
static std::string roundToString(float value, int decimals)
{
    float factor = std::pow(10.0f, decimals);
    float rounded = std::round(value * factor) / factor;

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(decimals) << rounded;
    return ss.str();
}
class TextRenderer {
public:
    TextRenderer(SDL_Renderer* renderer, const std::string& fontPath, int fontSize);
    ~TextRenderer();

    void render(const std::string& text, int x, int y,
                SDL_Color color = {255, 255, 255, 255});
    void measure(const std::string& text, int& w, int& h);

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
};
#endif //UNIPROJECT_TEXTRENDERER_H