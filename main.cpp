#include <iostream>
#include <SDL.h>
#include <vector>
#include "gameObject.h"


using namespace std;

vector <SDL_Rect> towers;

// daje jako zmienne bo w obliczeniach się przyda
static int _WIDTH = 1280;
static int _HEIGHT = 720;



void spawnTower(int _x, int _y, int _type) {
    SDL_Rect tower;

    // trzeba porobić klasy do wież
    switch (_type) {
        default:
            return;
        case 1:
            tower.w = 20;
            tower.h = 20;
            break;
        case 2:
            tower.w = 30;
            tower.h = 20;
            break;
        case 3:
            tower.w = 40;
            tower.h = 40;
            break;
    }
    // wycentrowanie
    tower.x = _x - tower.w / 2;
    tower.y = _y - tower.h / 2;
    // clamp żeby w ekranie się zmieściło
    if (tower.x < 0) tower.x = 0;
    if (tower.y < 0) tower.y = 0;
    if (tower.x + tower.w > _WIDTH) tower.x = _WIDTH - tower.w;
    if (tower.y + tower.h > _HEIGHT) tower.y = _HEIGHT - tower.h;

    towers.push_back(tower);

}

int main(int argc, char *argv[]) {
    // Sprawdzanie errorów
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL_Init error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event e;
    SDL_Init(SDL_INIT_EVERYTHING);
    bool running = true;
    SDL_Rect player{10, 10, 10, 20};
    // 0 - brak 1 - piechota 2 - killdozer 3 - działko
    int current_tower = 0;



    SDL_CreateWindowAndRenderer(_WIDTH, _HEIGHT, 0, &window, &renderer);
    //placeholder na teksture w tle
    auto background_texture =
            SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1280, 720);
    //placeholder na teksture kursora
    auto player_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 10, 20);
    //placeholder na teksture wiezy
    auto tower_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 10, 20);


    SDL_SetRenderTarget(renderer, background_texture);
    SDL_SetRenderDrawColor(renderer, 51, 102, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, player_texture);
    SDL_SetRenderDrawColor(renderer, 204, 102, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, tower_texture);
    SDL_SetRenderDrawColor(renderer, 204, 102, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, nullptr);


 while (running) {
     while (SDL_PollEvent(&e)) {

         // -------------- input --------------

         if (e.type == SDL_QUIT) {
             running = false;
         }
            // wybór typu wieży
         if (e.type == SDL_KEYDOWN) {
             switch (e.key.keysym.sym) {
                 case SDLK_1:
                     current_tower = 1;
                     break;
                 case SDLK_2:
                     current_tower = 2;
                     break;
                 case SDLK_3:
                     current_tower = 3;
                     break;
             }
             cout << current_tower << endl;
         }

         // stawianie wieży w pozycji kursora
         if (e.type == SDL_MOUSEBUTTONDOWN) {
             // można stawiać tylko w lewej połowie ekranu
             if (e.button.button == SDL_BUTTON_LEFT && e.button.x <= _WIDTH / 2) {
                 spawnTower(e.button.x , e.button.y, current_tower);
             }
         }
         // wychodzenie z gry
         if (e.type == SDL_KEYDOWN) {
             if (e.key.keysym.sym == SDLK_ESCAPE) {
                 running = false;
             }
         }
     }

     // -------------- render --------------

        SDL_RenderCopy(renderer, background_texture, nullptr, nullptr);
        SDL_RenderCopy(renderer, player_texture, nullptr, &player);


        for (auto& t : towers) {
            SDL_RenderCopy(renderer, tower_texture, nullptr, &t);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}
