#include <iostream>
#include <SDL.h>
#include <vector>

using namespace std;

vector <SDL_Rect> towers;

// daje jako zmienne bo w obliczeniach się przyda
static int _WIDTH = 1280;
static int _HEIGHT = 720;

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
         if (e.type == SDL_QUIT) {
             running = false;
         }
         // stawianie wieży w pozycji kursora
         if (e.type == SDL_MOUSEBUTTONDOWN) {
             // można stawiać tylko w lewej połowie ekranu
             if (e.button.button == SDL_BUTTON_LEFT && e.button.x <= _WIDTH / 2) {
                 SDL_Rect tower{ e.button.x, e.button.y, player.w, player.h };
                 towers.push_back(tower);
             }
         }
         // wychodzenie z gry
         if (e.type == SDL_KEYDOWN) {
             if (e.key.keysym.sym == SDLK_ESCAPE) {
                 running = false;
             }
         }
     }
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
