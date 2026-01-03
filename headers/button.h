//
// Created by Dawid Roś on 25/11/2025.
//

#ifndef UNIPROJECT_BUTTON_H
#define UNIPROJECT_BUTTON_H
#include <SDL.h>


class Button {
    public:
    bool IsSelected = false;
    Button();

    void update(SDL_Renderer *renderer, const SDL_Event &e, int &current_tower); // to jest odpowiedzialne za pobranie danych do przycisku
};


#endif //UNIPROJECT_BUTTON_H