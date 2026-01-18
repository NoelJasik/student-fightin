//
// Created by mateu on 14.01.2026.
//
#include "../headers/notification.h"
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <algorithm>
#include <iostream>
int textW = 0, textH = 0;
TTF_Font* font;
static const int MAX_VISIBLE = 3;
// ===== Notification =====
Notification::Notification(const std::string& text, Uint32 duration)
    : text(text), startTime(SDL_GetTicks()), duration(4000){}


// ===== NotificationManager =====
NotificationManager::NotificationManager(SDL_Renderer* renderer, TTF_Font* font)
    : renderer(renderer), font(font) {
}
void NotificationManager::add(const std::string& text) {
    notifications.emplace_back(text);
}
void NotificationManager::render(SDL_Window* window) {
    if (notifications.empty()) return;

    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);

    const int PADDING = 12;
    const int MARGIN  = 15;
    const int SPACING = 10;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    int offsetY = MARGIN;
    int visible = std::min((int)notifications.size(), MAX_VISIBLE);

    for (int i = 0; i < visible; i++) {
        const Notification& n = notifications[i];

        //text
        SDL_Color textColor = {255,255,255,255};
        SDL_Color bgColor   = {40,40,40,200};

        SDL_Surface* surface =
            TTF_RenderUTF8_Blended(font, n.text.c_str(), textColor);
        if (!surface) continue;

        SDL_Texture* texture =
            SDL_CreateTextureFromSurface(renderer, surface);

        int textW = surface->w;
        int textH = surface->h;
        SDL_FreeSurface(surface);
        //prawy gorny rog
        SDL_Rect bgRect {
            winW - (textW + PADDING * 2) - MARGIN,
            offsetY,
            textW + PADDING * 2,
            textH + PADDING * 2
        };

        SDL_SetRenderDrawColor(renderer,
            bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderFillRect(renderer, &bgRect);

        //tekst
        SDL_Rect textRect {
            bgRect.x + PADDING,
            bgRect.y + PADDING,
            textW,
            textH
        };
        SDL_RenderCopy(renderer, texture, nullptr, &textRect);
        SDL_DestroyTexture(texture);
        offsetY += bgRect.h + SPACING;
    }

}
void NotificationManager::update()
{
    Uint32 now = SDL_GetTicks();

    for (auto it = notifications.begin(); it != notifications.end(); )
    {
        if (now - it->startTime >= it->duration)
        {
            it = notifications.erase(it); //usun
        }
        else
        {
            ++it;
        }
    }
}













