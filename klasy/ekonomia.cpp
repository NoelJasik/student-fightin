//
// Created by Dawid Roś on 18/01/2026.
//
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "../headers/ekonomia.h"
#include <iostream>
#include <string>
using namespace std;
int kasa = 100;//dodanie kasy ~dawid trzeba będzie voida zrobić z update


void ekonomia::odejmowaniekasy(int kasa, int kosztjednostki) {
    kasa = kasa - kosztjednostki;
    cout << kasa;



}

ekonomia::ekonomia() = default;
    void ekonomia::update(SDL_Renderer *renderer){


    };

    int ekonomia::getMoney() {
        return kasa;
    }


