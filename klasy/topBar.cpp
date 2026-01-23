//
// Created by mateu on 21.01.2026.
//

#include "../headers/topBar.h"

topBar::topBar(int screenWidth)
{
    rect = {0, 0, screenWidth, 100};
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
    if (e.type == SDL_MOUSEBUTTONDOWN &&
        e.button.button == SDL_BUTTON_LEFT)
    {
        SDL_Point p{e.button.x, e.button.y};

        if (SDL_PointInRect(&p, &rect)) {

            // start fali
            if (startWaveButton.handleEvent(e, current_tower)) {
                startWaveClicked = true;
                return true;
            }

            // buttony unity
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
                    TextRenderer& text, int currentWave,
                    bool activeWave,
                    float playerHealth,
                    int maxWave)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // TŁO TOPBARA
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
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
    std::string koszt = std::to_string(10 + (currentWave==1?0:(currentWave - 2)) * 5) + "$";
    int textW = 0, textH = 0;
    text.measure(koszt, textW, textH);
    text.render(koszt, 10, (rect.h - textH) / 2, SDL_Color{0, 0, 0, 255});


    std::string koszt2 = std::to_string(50 + (currentWave==1?0:(currentWave - 2))  * 5)  + "$";
    text.measure(koszt2, textW, textH);
    text.render(koszt2, 70, (rect.h - textH) / 2, SDL_Color{0, 0, 0, 255});

    std::string koszt3 = std::to_string(75 + (currentWave==1?0:(currentWave - 2)) * 5)  + "$";
    text.measure(koszt3, textW, textH);
    text.render(koszt3, 130, (rect.h - textH) / 2, SDL_Color{0, 0, 0, 255});
    // ---------- KASA ----------
    std::string moneyText = "Saldo: $ " + std::to_string(money);

    text.measure(moneyText, textW, textH);

    int moneyX = lastButtonRight + 40;                 // ⬅️ 40 px od buttonów
    int moneyY = rect.y + (rect.h - textH) / 2;        // wyśrodkowanie pionowe

    text.render(moneyText, moneyX, moneyY);
    std::string wave = "Semestr: "+std::to_string(currentWave==1?1:currentWave-1)+"/"+std::to_string(maxWave);
    int waveW, waveH=0;
    text.measure(wave, waveW, waveH);
    int waveX=lastButtonRight + waveW+160;
    int waveY = rect.y + (rect.h - waveH) / 2;
    if(currentWave!=0) {
        text.render(wave, waveX, waveY);
    }

    if(currentWave<11 &&  !activeWave) {
        // rozpocznij fale
        int btnW = 200;
        int btnH = 40;

        int btnX = rect.w - btnW - 20;   // 20px od prawej
        int btnY = rect.y + (rect.h - btnH) / 2;

        startWaveButton.rect = {btnX, btnY, btnW, btnH};
        startWaveButton.isActionButton = true;

        SDL_SetRenderDrawColor(renderer, 70, 130, 70, 255);
        SDL_RenderFillRect(renderer, &startWaveButton.rect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &startWaveButton.rect);

        std::string startTxt = "Rozpocznij Sesje";
        int tW = 0, tH = 0;
        text.measure(startTxt, tW, tH);

        text.render(
            startTxt,
            btnX + (btnW - tW) / 2,
            btnY + (btnH - tH) / 2
        );
    }

    // ---------- HP GRACZA ----------

    // TODO - przekminić miejsce żeby się nie wyświetlało pod powiadomieniami
    std::string healthText = "Zdrowie: " + std::to_string(static_cast<int>(playerHealth));
    int healthW = 0, healthH = 0;
    text.measure(healthText, healthW, healthH);
    int healthX = waveX + waveW + 100 ;
    int healthY = rect.y + (rect.h - healthH) / 2;
    text.render(healthText, healthX, healthY);

}
void topBar::resetStartWaveClicked() {
    startWaveClicked=false;
}

