//
// Created by mateu on 20.01.2026.
//

#include "../headers/uiStatsBox.h"
#include "../headers/TextRenderer.h"
#include "../headers/notification.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>
void uiStatsBox::render(SDL_Renderer* renderer,
                        TextRenderer& text,
                        gameObject& obj)
{
    const int PADDING = 6;

    std::string title = obj.name + "  SEM." + std::to_string(obj.lvl);
    std::string line1 = "HP:  " + roundToString(obj.hp, 1);
    std::string line2 = "DMG: " + roundToString(obj.attackDamage, 1);
    std::string line3 = "MOC: "  + roundToString(obj.attackForce, 2);

    int wt, ht, w1, h1, w2, h2, w3, h3;
    text.measure(title, wt, ht);
    text.measure(line1, w1, h1);
    text.measure(line2, w2, h2);
    text.measure(line3, w3, h3);

    int boxW = std::max({wt, w1, w2, w3}) + PADDING * 2;
    int boxH = ht + h1 + h2 + h3 + PADDING * 6 + 24;

    SDL_Rect box{
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
    y += h3 + 6;

    // ---------- PRZYCISK LVL UP ----------
    lvlUpButton = {
        box.x + PADDING,
        y,
        box.w - PADDING * 2,
        22
    };

    SDL_SetRenderDrawColor(renderer, 60, 160, 60, 230);
    SDL_RenderFillRect(renderer, &lvlUpButton);

    text.render("LVL UP", lvlUpButton.x + 8, lvlUpButton.y + 3);
}
bool uiStatsBox::handleEvent(const SDL_Event& e, gameObject& obj, NotificationManager& notification_manager)
{
    if (e.type == SDL_MOUSEBUTTONDOWN &&
        e.button.button == SDL_BUTTON_LEFT)
    {
        int mx = e.button.x;
        int my = e.button.y;

        if (mx >= lvlUpButton.x &&
            mx <= lvlUpButton.x + lvlUpButton.w &&
            my >= lvlUpButton.y &&
            my <= lvlUpButton.y + lvlUpButton.h)
        {
            if (obj.lvl+1>8) {
                notification_manager.add("Maksymalny lvl jednostki to 8");

            }else if(obj.lvl+1==8){
                obj.levelUp();
            }
            else {
                obj.levelUp();
            }
            return true;
        }
    }
    return false;
}

