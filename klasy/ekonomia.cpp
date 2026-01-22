//
// Created by Dawid Roś on 18/01/2026.
//
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "../headers/ekonomia.h"
#include <iostream>
#include <string>
using namespace std;
ekonomia::ekonomia() = default;
    void ekonomia::update(SDL_Renderer *renderer){


    };

    int ekonomia::getMoney() {
        return kasa;
    }



