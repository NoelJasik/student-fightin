//
// Created by mateu on 21.01.2026.
//

#include "../headers/topBar.h"

topBar::topBar(int screenWidth)
{
    rect = {0, 0, screenWidth, 70};
    int buttonSize = 50;
    int buttonY = (rect.h - buttonSize) / 2;
    Button b1, b2, b3;


    b1.rect = {10, buttonY, buttonSize, buttonSize};

    b1.towerId = 1;

    b2.rect = {70, buttonY, buttonSize, buttonSize};
    b2.towerId = 2;

    b3.rect = {130, buttonY, buttonSize, buttonSize};
    b3.towerId = 3;

    buttons.push_back(b1);
    buttons.push_back(b2);
    buttons.push_back(b3);
}

bool topBar::handleEvent(const SDL_Event& e, int& current_tower)
{
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        SDL_Point p{e.button.x, e.button.y};

        // klik w TopBar → blokujemy grę
        if (SDL_PointInRect(&p, &rect)) {
            for (auto& b : buttons) {
                if (b.handleEvent(e, current_tower))
                    return true;
            }
            return true;
        }
    }
    return false;
}

void topBar::render(SDL_Renderer* renderer,
                    int current_tower,
                    int money,
                    TextRenderer& text, int currentWave)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // TŁO TOPBARA
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &rect);

    int lastButtonRight = 0;

    for (auto& b : buttons)
    {
        // rysuj przycisk
        b.render(renderer);

        // aktywny tower → biała ramka
        if (b.towerId == current_tower)
        {
            SDL_Rect border{
                b.rect.x - 2,
                b.rect.y - 2,
                b.rect.w + 4,
                b.rect.h + 4
            };

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &border);
        }

        // zapamiętaj prawą krawędź
        lastButtonRight = std::max(lastButtonRight, b.rect.x + b.rect.w);
    }
    //-----------KOSZTY----------
    std::string koszt = "$10";
    int textW = 0, textH = 0;
    text.measure(koszt, textW, textH);
    text.render(koszt, 10, (rect.h - textH) / 2);


    std::string koszt2 = "$50";
    text.measure(koszt2, textW, textH);
    text.render(koszt2, 70, (rect.h - textH) / 2);

    std::string koszt3 = "$25";
    text.measure(koszt3, textW, textH);
    text.render(koszt3, 130, (rect.h - textH) / 2);
    // ---------- KASA ----------
    std::string moneyText = "Saldo: $ " + std::to_string(money);

    text.measure(moneyText, textW, textH);

    int moneyX = lastButtonRight + 40;                 // ⬅️ 40 px od buttonów
    int moneyY = rect.y + (rect.h - textH) / 2;        // wyśrodkowanie pionowe

    text.render(moneyText, moneyX, moneyY);
    if(currentWave!=0) {
        std::string wave = "Fala: "+std::to_string(currentWave)+"/10";
        int waveW, waveH=0;
        text.measure(wave, waveW, waveH);
        int waveX=lastButtonRight + waveW+160;
        int waveY = rect.y + (rect.h - waveH) / 2;
        text.render(wave, waveX, waveY);
    }
}