//
// Created by Dawid Roś on 25/11/2025.
//
#include <SDL.h>
#include "../headers/button.h"


Button::Button() = default; // chuj wie co to robi ale działa

void Button::update(SDL_Renderer *renderer, const SDL_Event& e, int& current_tower) { // pobiera wszystkie dane w sensie prędzej to zmienne ale tak
    // do tego trzeba zrobić 3 takie dla każdego z towera EDIT 30.11.2025 Lizak: zrobione :) jeśli cos jest nie tak to napiszczie
    if (e.type == SDL_MOUSEBUTTONDOWN && 
        e.button.button == SDL_BUTTON_LEFT &&
        e.button.x >= 50 && e.button.x <= 100 &&
        e.button.y >= 50 && e.button.y <= 100) {
        // to coś sprawdza czy mysz jest w 50-100 x i y tam bęzie przycisk

        current_tower = 1; //wybiera wieżę
    }
    SDL_Rect buttonRect = {50, 50, 50, 50}; //  a to rysuje przycisk
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &buttonRect);

    if (e.type == SDL_MOUSEBUTTONDOWN &&
        e.button.button == SDL_BUTTON_LEFT &&
        e.button.x >= 125 && e.button.x <= 185 &&
        e.button.y >= 50 && e.button.y <= 100) {

        current_tower = 2;
        }
    buttonRect = {125, 50, 50, 50};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &buttonRect);

    if (e.type == SDL_MOUSEBUTTONDOWN &&
        e.button.button == SDL_BUTTON_LEFT &&
        e.button.x >= 200 && e.button.x <= 250 &&
        e.button.y >= 50 && e.button.y <= 100) {

        current_tower = 3;
        }
    buttonRect = { 200, 50, 50, 50};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &buttonRect);

}
