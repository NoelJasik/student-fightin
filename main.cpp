#include <iostream>
#include <SDL.h>
#include <vector>
#include "headers/gameObject.h"
#include "headers/button.h"
#include "headers/notification.h"
#include "headers/TextRenderer.h"
#include "math.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "headers/inputBox.h"
#include "ekonomia.h"
#include "headers/uiStatsBox.h"
#include "headers/gameSettings.h"

using namespace std;
// Można wyciągnąć z tego klase screensize
using namespace gameSettings;

vector<gameObject> towers;
vector<gameObject> enemies;

// ------ GRAFIKI ------
SDL_Surface *background_surface = IMG_Load("assets/bg.jpg");

// TODO - wrzucić to do klasy żeby się dało łatwo pobierać
// daje jako zmienne bo w obliczeniach się przyda

uiStatsBox stats_box;
gameObject *selectedTower = nullptr;

void checkCollisions() {
    for (auto &currentTower: towers) {
        // Dla AKTUALNEJ wieży sprawdzamy kolizje z całą resztą listy
        std::vector<gameObject *> hits = currentTower.checkCollisions(enemies);

        if (!hits.empty()) {
            // cout << "Obiekt " << currentTower.name << " koliduje z " << hits.size() << " obiektami." << endl;

            for (auto hitObject: hits) {
                currentTower.combatWith(*hitObject);
            }
        }
    }
}


void spawnTower(int _x, int _y, int _type) {
    gameObject tower;

    // trzeba porobić klasy do wież
    switch (_type) {
        default:
            return;
        case 1:
            tower = gameObject(_x, _y, 50, 90, "Student", 100, 10, 1.0);
            break;
        case 2:
            tower = gameObject(_x, _y, 100, 200, "Koparka", 200, 10, 1.0);
            tower.setMaxMoveSpeed(0, 10);
            break;
        case 3:
            tower = gameObject(_x, _y, 100, 100, "Studenciak (budynek)", 150, 10, 1.0);
            break;
    }
    // wycentrowanie
    tower.rect.x = _x - tower.rect.w / 2;
    tower.rect.y = _y - tower.rect.h / 2;
    // clamp żeby w ekranie się zmieściło
    if (tower.rect.x < 0) tower.rect.x = 0;
    if (tower.rect.y < 0) tower.rect.y = 0;
    if (tower.rect.x + tower.rect.w > ScreenSize::getWidth()) tower.rect.x = ScreenSize::getWidth() - tower.rect.w;
    if (tower.rect.y + tower.rect.h > ScreenSize::getHeight()) tower.rect.y = ScreenSize::getHeight() - tower.rect.h;

    towers.push_back(tower);
}

// Logika odpalania fal i spawnowania przeciwników
void startWave(int _enemyCount, int _enemySpread) {
    for (int i = 0; i < _enemyCount; i++) {
        gameObject enemy = gameObject(ScreenSize::getWidth() + rand() % _enemySpread, rand() % ScreenSize::getHeight(),
                                      50, 90, "Enemy", 50, 10, 1.0, true, -0.5f, 0);
        enemies.push_back(enemy);
    }
}

// TODO zrobić żeby to sie wyświetlało co X ticków, żeby łatwiej się testowało garbage collector.
// void displayDebug() {
//         std::cout << "--- Current Game Objects ---" << std::endl;
//         std::cout << "Towers (" << towers.size() << "):" << std::endl;
//         for (const auto& tower : towers) {
//             std::cout << "  - Name: " << tower.name << ", Pos: (" << tower.rect.x << ", " << tower.rect.y << "), HP: " << tower.hp << std::endl;
//         }
//         std::cout << "Enemies (" << enemies.size() << "):" << std::endl;
//         for (const auto& enemy : enemies) {
//             std::cout << "  - Name: " << enemy.name << ", Pos: (" << enemy.rect.x << ", " << enemy.rect.y << "), HP: " << enemy.hp << std::endl;
//         }
//         std::cout << "----------------------------" << std::endl;
// }

// Czyścimy przeciwników i naszych z pamięci
void gameObjectCleanup() {
    // TODO Dodać do klasy gameobject funkcję onDestroy która się tu odpala, tam wjebiemy logike na dodawanie siana itd
    std::erase_if(towers, [](const gameObject &t) {
        // displayDebug();
        return t.destroy;
    });
    std::erase_if(enemies, [](const gameObject &e) {
        return e.destroy;
    });

    if (selectedTower && selectedTower->destroy) {
        selectedTower = nullptr;
    }
}

SDL_Surface *TTF_RenderText_Solid(TTF_Font *font, const char *str, int fg, SDL_Color sdl_color);

int main(int argc, char *argv[]) {
    // Sprawdzanie errorów
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL_Init error: " << SDL_GetError() << endl;
        return 1;
    }

    if (TTF_Init() == -1) {
        // inicjalizacja sld2_ttf dawid
        SDL_Log("TTF_Init error: %s", TTF_GetError());
    }


    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event e;
    SDL_Init(SDL_INIT_EVERYTHING);
    bool running = true;
    SDL_Rect player{10, 10, 10, 20};
    // 0 - brak 1 - piechota 2 - killdozer 3 - działko
    int current_tower = 0;


    SDL_CreateWindowAndRenderer(ScreenSize::getWidth(), ScreenSize::getHeight(), 0, &window, &renderer);
    TextRenderer notficiationsTextRenderer(renderer, "assets/GravitasOne-Regular.ttf", 16);
    InputBox inputBox(&notficiationsTextRenderer);
    // Dałem statyczne, bo to jest tło
    auto background_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
                                                ScreenSize::getWidth(),
                                                ScreenSize::getHeight());

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
    SDL_Surface *tower_surface = IMG_Load("assets/aghUnit.png");
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


    Button uiButton; // renderuje przycisk
    ekonomia uiEkonomia;
    NotificationManager notification_manager(renderer, &notficiationsTextRenderer);
    while (running) {
        while (SDL_PollEvent(&e)) {
            // inputBox.handleEvent(e);
            // if (inputBox.isActive())
            //  continue;
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
                    case SDLK_9:
                        current_tower = 0;
                    case SDLK_q:
                        startWave(1000, 100000);
                }


                // cout << current_tower << endl;
            }


            // stawianie wieży w pozycji kursora
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                // można stawiać tylko w lewej połowie ekranu
                if (e.button.button == SDL_BUTTON_LEFT && e.button.x <= ScreenSize::getWidth() / 2 && current_tower !=
                    0) {
                    //zapobieganie kolizji jednostek Mateusz 16.12
                    double distance = 0; // zmienne do przechowywania dystansu i czy jednsotka moze byc postawiona
                    float can_be_placed = true;
                    if (current_tower == 1) {
                        for (int i = 0; i < towers.size(); i++) {
                            distance = gameObject::calculateDistance(
                                towers[i], gameObject(e.button.x, e.button.y, 20, 30, "Infantry Tower", 100, 10, 1.0));
                            // cout << "Distance: " << distance << " [i]" << i << endl;
                            if (distance < 75) {
                                can_be_placed = false;
                                // to jest zjebane, bo psuje gameplay zatrzymując całą grę
                                // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Komunikat", "Nie mozesz postawic tutaj jednostki, znajduje sie ona zbyt blisko innej", NULL);
                                break;
                            }
                        }
                    } else if (current_tower == 3) {
                        for (int i = 0; i < towers.size(); i++) {
                            distance = gameObject::calculateDistance(
                                towers[i], gameObject(e.button.x, e.button.y, 20, 30, "Infantry Tower", 100, 10, 1.0));
                            if (distance < 95) {
                                can_be_placed = false;
                                // to jest zjebane, bo psuje gameplay zatrzymując całą grę
                                // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Komunikat", "Nie mozesz postawic tutaj jednostki, znajduje sie ona zbyt blisko innej", NULL);
                                break;
                            }
                        }
                    }
                    if (can_be_placed == true && current_tower != 0) {
                        selectedTower = nullptr;
                        // inputBox.open(e.button.x, e.button.y, current_tower);
                        spawnTower(
                            e.button.x,
                            e.button.y,
                            current_tower
                        );
                    } else {
                        selectedTower = nullptr;
                        notification_manager.add("Nie mozna postawic jednsotki");
                    }
                }
                if (e.button.button == SDL_BUTTON_RIGHT) {
                    selectedTower = nullptr; // domslnie nic nie zaznaczone

                    SDL_Point mousePoint{e.button.x, e.button.y};

                    for (auto &t: towers) {
                        if (SDL_PointInRect(&mousePoint, &t.rect)) {
                            selectedTower = &t;
                            break;
                        }
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
            uiEkonomia.update(renderer);
        }

        // if (inputBox.isConfirmed())
        //     {
        //         spawnTower(
        //         inputBox.getX(),
        //         inputBox.getY(),
        //         current_tower,
        //         inputBox.getText()
        //     );
        //     inputBox.reset();
        //     }
        // ---------- Fizyka ----------
        // sprawdzanie kolizji
        // Przechodzimy przez każdy obiekt w wektorze (używamy referencji & żeby nie kopiować)
        checkCollisions();

        // -------------- render --------------

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background_texture, nullptr, nullptr);

        for (auto &t: towers) {
            SDL_RenderCopy(renderer, tower_texture, nullptr, &t.rect);
            t.update();
        }
        for (auto &e: enemies) {
            SDL_RenderCopy(renderer, tower_texture, nullptr, &e.rect);
            e.update();
        }
        notification_manager.render(window);
        notification_manager.update();
        // SDL_RenderCopy(renderer, player_texture, nullptr, &player);

        // czyszczenie pamięci po update, bo tam sprawdzamy kolizje
        gameObjectCleanup();
        // rysuje przycisk
        SDL_Event drawEvent{};
        uiButton.update(renderer, drawEvent, current_tower);
        uiEkonomia.update(renderer);
        inputBox.render(renderer);
        if (selectedTower) {
            stats_box.render(
                renderer,
                notficiationsTextRenderer,
                *selectedTower
            );
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();


    return 0;
}
