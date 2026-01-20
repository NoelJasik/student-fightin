//
// Created by mateu on 20.01.2026.
//
#include "../headers/TextRenderer.h"
#include <iostream>

TextRenderer::TextRenderer(SDL_Renderer* renderer,
                           const std::string& fontPath,
                           int fontSize)
    : renderer(renderer), font(nullptr)
{
    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        std::cerr << "TTF_OpenFont error: " << TTF_GetError() << std::endl;
    }
}

TextRenderer::~TextRenderer()
{
    if (font) {
        TTF_CloseFont(font);
    }
}

void TextRenderer::render(const std::string& text, int x, int y, SDL_Color color)
{
    if (!font) return;

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h);

    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}
void TextRenderer::measure(const std::string& text, int& w, int& h)
{
    if (!font) {
        w = 0;
        h = 0;
        return;
    }

    // SDL_ttf liczy rozmiar bez renderowania
    TTF_SizeUTF8(font, text.c_str(), &w, &h);
}
