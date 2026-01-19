//
// Created by Dawid Roś on 18/01/2026.
//
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "../headers/ekonomia.h"
#include <iostream>
using namespace std;
int kasa = 100;//dodanie kasy ~dawid trzeba będzie voida zrobić z update
ekonomia::ekonomia() = default;
    void ekonomia::update(SDL_Renderer *renderer) {

        TTF_Font* font = TTF_OpenFont("assets/PlaywriteCU-Regular.ttf", 32); // pobieranie fonta
        TTF_Font* fontNotification = TTF_OpenFont("assets/PlaywriteCU-Regular.ttf", 16); // pobieranie fonta
        if (!font) {
            SDL_Log("Font error: %s", TTF_GetError());
        }
        SDL_Color textColor = {200, 23, 20, 255}; // ogarnianie aby to działało( dawid )w sensie ta czcionka i ten cały ttf
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, "$000", textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect;
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        textRect.x = 0;
        textRect.y = 720-textRect.h;
        SDL_FreeSurface(textSurface);


        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);


    };

