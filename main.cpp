#include <iostream>
#include <SDL.h>
#include <vector>
#include "headers/gameObject.h"
#include "headers/button.h"
#include "math.h"
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

vector <gameObject> towers;

// ------ GRAFIKI ------
SDL_Surface* background_surface = IMG_Load("assets/bg.jpg");


// daje jako zmienne bo w obliczeniach się przyda
static int _WIDTH = 1280;
static int _HEIGHT = 720;
//oblicza dystans pomiedzy 2 jednostkami Mateusz 16.12
double CalcualteDistance(gameObject obj1, gameObject obj2) {
    double distance = sqrt(pow(obj1.rect.x-obj2.rect.x, 2) + pow(obj1.rect.y-obj2.rect.y, 2));
    return distance;

}

void spawnTower(int _x, int _y, int _type) {
    gameObject tower;

    // trzeba porobić klasy do wież
    switch (_type) {
        default:
            return;
        case 1:
            tower = gameObject(_x,_y,50,90,"Infantry Tower",100);
            break;
        case 2:
            tower = gameObject(_x,_y,100,200,"Killdozer Tower",200);
            tower.setMoveSpeed(0, 10);
            break;
        case 3:
            tower = gameObject(_x,_y,100,100,"Cannon Tower",150);
            break;
    }
    // wycentrowanie
    tower.rect.x = _x - tower.rect.w / 2;
    tower.rect.y = _y - tower.rect.h / 2;
    // clamp żeby w ekranie się zmieściło
    if (tower.rect.x < 0) tower.rect.x = 0;
    if (tower.rect.y < 0) tower.rect.y = 0;
    if (tower.rect.x + tower.rect.w > _WIDTH) tower.rect.x = _WIDTH - tower.rect.w;
    if (tower.rect.y + tower.rect.h > _HEIGHT) tower.rect.y = _HEIGHT - tower.rect.h;

    towers.push_back(tower);

}

int main(int argc, char *argv[]) {
    // Sprawdzanie errorów
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL_Init error: " << SDL_GetError() << endl;
        return 1;
    }

    if (TTF_Init() == -1) { // inicjalizacja sld2_ttf dawid
        SDL_Log("TTF_Init error: %s", TTF_GetError());
    }
    TTF_Font* font = TTF_OpenFont("assets/GravitasOne-Regular.ttf", 64); // pobieranie fonta
    if (!font) {
        SDL_Log("Font error: %s", TTF_GetError());
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

   // Dałem statyczne, bo to jest tło
    auto background_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, _WIDTH, _HEIGHT);

   // Wczytywanie tła (to można było by przerobić na funkcje)
    if (!background_surface) {
        cout << "IMG_Load error: " << IMG_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);
    SDL_FreeSurface(background_surface);
    if (!background_texture) {
        cout << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Wykorzystajmy (narazie, jak będzie czas to zmienimy) kursor systemowy. Jako inżynier trzeba korzystać z praktycznych rozwiązań i rozwiązywać praktyczne problemy, kursor to problem już dawno rozwiązany
    // auto player_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 10, 20);    SDL_Surface* player_surface = IMG_Load("assets/aghUnit.png");

    auto tower_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 2709, 6468);
    SDL_Surface* tower_surface = IMG_Load("assets/aghUnit.png");
    if (!tower_surface) {
        cout << "IMG_Load error: " << IMG_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    tower_texture = SDL_CreateTextureFromSurface(renderer, tower_surface);
    SDL_FreeSurface(tower_surface);
    if (!tower_texture) {
        cout << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }





    SDL_SetRenderTarget(renderer, background_texture);
    SDL_SetRenderDrawColor(renderer, 51, 102, 0, 255);
    SDL_RenderClear(renderer);

    // SDL_SetRenderTarget(renderer, player_texture);
    // SDL_SetRenderDrawColor(renderer, 204, 102, 0, 255);
    // SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, tower_texture);
    SDL_SetRenderDrawColor(renderer, 204, 102, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, nullptr);


    SDL_Color textColor = {200, 23, 20, 255}; // ogarnianie aby to działało( dawid )w sensie ta czcionka i ten cały ttf
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "$000", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    textRect.x = _WIDTH - textRect.w;
    textRect.y = 0;
    SDL_FreeSurface(textSurface);

    Button uiButton; // renderuje przycisk

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
                 case SDLK_0:
                     current_tower = 0;
             }
             cout << current_tower << endl;
         }


         // stawianie wieży w pozycji kursora
         if (e.type == SDL_MOUSEBUTTONDOWN) {
             // można stawiać tylko w lewej połowie ekranu
             if (e.button.button == SDL_BUTTON_LEFT && e.button.x <= _WIDTH / 2) {
                 //zapobieganie kolizji jednostek Mateusz 16.12
                 double distance=0;// zmienne do przechowywania dystansu i czy jednsotka moze byc postawiona
                 float can_be_placed=true;
                 if (current_tower==1) {
                     for (int i=0; i<towers.size(); i++) {
                         distance=CalcualteDistance(towers[i], gameObject(e.button.x,e.button.y,20,30,"Infantry Tower",100));
                         if (distance<45) {
                             can_be_placed=false;
                             // to jest zjebane, bo psuje gameplay zatrzymując całą grę
                             // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Komunikat", "Nie mozesz postawic tutaj jednostki, znajduje sie ona zbyt blisko innej", NULL);
                             break;
                         }
                     }
                 }else if (current_tower==3) {
                     for (int i=0; i<towers.size(); i++) {
                         distance=CalcualteDistance(towers[i], gameObject(e.button.x,e.button.y,20,30,"Infantry Tower",100));
                         if (distance<65) {
                             can_be_placed=false;
                             // to jest zjebane, bo psuje gameplay zatrzymując całą grę
                             // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Komunikat", "Nie mozesz postawic tutaj jednostki, znajduje sie ona zbyt blisko innej", NULL);
                             break;
                         }
                     }
                 }
                 if (can_be_placed==true) {
                     spawnTower(e.button.x,e.button.y,current_tower);
                 }
             }
         }
         // wychodzenie z gry
         if (e.type == SDL_KEYDOWN) {
             if (e.key.keysym.sym == SDLK_ESCAPE) {
                 running = false;
             }
         }
         // robi update renderu przycisku
         uiButton.update(renderer, e, current_tower);
     }

     // -------------- render --------------

        SDL_RenderCopy(renderer, background_texture, nullptr, nullptr);
        // SDL_RenderCopy(renderer, player_texture, nullptr, &player);


        for (auto& t : towers) {
            SDL_RenderCopy(renderer, tower_texture, nullptr, &t.rect);
            // to trzeba zrobić matematycznie żeby działało niezależnie od fps
            t.update();
        }

        // rysuje przycisk
        SDL_Event drawEvent{};
        uiButton.update(renderer, drawEvent, current_tower);

        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
     }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();


    return 0;
}
