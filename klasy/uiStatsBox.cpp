//
// Created by mateu on 20.01.2026.
//

#include "../headers/uiStatsBox.h"
#include "../headers/TextRenderer.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>
void uiStatsBox::render(SDL_Renderer* renderer,
                        TextRenderer& text,
                        const gameObject& obj)
{
    const int PADDING = 6;


    std::string title = obj.name;
    std::string line1 = "HP:  " + roundToString(obj.hp, 1);
    std::string line2 = "DMG: " + roundToString(obj.attackdamage, 1);
    std::string line3 = "AS: " + roundToString(obj.attackspeed, 2);

    int wt,ht,w1,h1,w2,h2,w3,h3;
    text.measure(title, wt, ht);
    text.measure(line1, w1, h1);
    text.measure(line2, w2, h2);
    text.measure(line3, w3, h3);

    int boxW = std::max({wt, w1, w2, w3}) + PADDING * 2;
    int boxH = ht + h1 + h2 + h3 + PADDING * 5;

    SDL_Rect box {
        obj.rect.x + obj.rect.w / 2 - boxW / 2,
        obj.rect.y - boxH - 8,
        boxW,
        boxH
    };

    if (box.y < 0)
        box.y = obj.rect.y + obj.rect.h + 8;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 220);
    SDL_RenderFillRect(renderer, &box);
    int y = box.y + PADDING;
    text.render(title, box.x + PADDING, y);
    y += ht + 4;
    text.render(line1, box.x + PADDING, y);
    y += h1 + 2;
    text.render(line2, box.x + PADDING, y);
    y += h2 + 2;
    text.render(line3, box.x + PADDING, y);
}