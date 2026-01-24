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
#include "headers/topBar.h"
using namespace std;
// Można wyciągnąć z tego klase screensize
using namespace gameSettings;
int currentWave = 1;
int maxWave = 8;
float playerHealth = 100.0f;
vector<gameObject> towers;
vector<gameObject> enemies;

float lastTickTime = 0.0f;

bool activeWave=false;
bool endWave=false;
bool gameWon = false;
bool showReplayButton = false;
Button replayButton;
ekonomia uiEkonomia; // tutaj deklaracja ekonomi
bool gameLost = false;
Button defeatReplayButton;
// ------ MENU -------
int showmenu(SDL_Renderer* renderer, TextRenderer* textRenderer) {
if(!renderer || !textRenderer) {
    std::cerr << "Renderer or TextRenderer is null!" << std::endl;
    return -1;
}
    const int NumMenu = 2;
    const char* labels[NumMenu] = {"Kontynuuj", "Wyjscie"};
    bool selected[NumMenu]={false, false};
    SDL_Color colors[2] = {{255,255,255},{255,0,0}};

    int screenW, screenH;
    SDL_GetRendererOutputSize(renderer, &screenW, &screenH);

    int textW[NumMenu], textH[NumMenu];
    for (int i =0;i<NumMenu;i++) {
        textRenderer->measure(labels[i], textW[i], textH[i]);
    }
    SDL_Rect pos[NumMenu];
    pos[0].x=screenW/2 - textW[0]/2;
    pos[0].y=screenH/2 - textH[0];
    pos[1].x=screenW/2 - textW[1]/2;
    pos[1].y=screenH/2 + textH[1];

    SDL_Event menuevent;
    while(true)
    {
        while (SDL_PollEvent(&menuevent))
            {
                switch(menuevent.type) {
                    case SDL_QUIT:
                        return 1;
                    case SDL_MOUSEMOTION: {
                        int x = menuevent.motion.x;
                        int y = menuevent.motion.y;
                        for (int i=0;i<NumMenu;i++) {
                            selected[i] = (x>=pos[i].x && x<=pos[i].x+textW[i] && y>=pos[i].y && y<=pos[i].y+textH[i]);

                        }
                        break;
                    }
                    case SDL_MOUSEBUTTONDOWN: {
                        int x = menuevent.button.x;
                        int y = menuevent.button.y;
                        for (int i = 0; i < NumMenu; i++) {
                            if (x >= pos[i].x && x <= pos[i].x + textW[i] &&
                                y >= pos[i].y && y <= pos[i].y + textH[i]) {
                                return i;
                                }
                        }
                        break;
                    }


                    case SDL_KEYDOWN:
                        if (menuevent.key.keysym.sym == SDLK_ESCAPE) {
                            return 0;
                        }
                        break;
                    }
            }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        for (int i = 0; i < NumMenu; i++) {
            textRenderer->render(labels[i],pos[i].x, pos[i].y, colors[selected[i] ? 1 : 0]);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
        }
    return 0;
}


// ------ GRAFIKI ------
SDL_Surface *background_surface = IMG_Load("assets/bg.jpg");
SDL_Surface *TTF_RenderText_Solid(TTF_Font *font, const char *str, int fg, SDL_Color sdl_color);
SDL_Surface *tower_surface = IMG_Load("assets/student.png");
SDL_Surface *tower2_surface = IMG_Load("assets/Koparka1.png");
SDL_Surface *tower3_surface = IMG_Load("assets/sklep.png");
SDL_Surface *enemy_surface = IMG_Load("assets/kibolUnit.png");

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
            tower = gameObject(_x, _y, 40, 82, "Student", 100, 10, 1.0, false, 0, 0, 0.05f, 60, 1);
            break;
        case 2:
            tower = gameObject(_x, _y, 140, 150, "Koparka", 10000, 50, 5, false, 0, 0, 0.0005f, 8600, 2);
            tower.setMaxMoveSpeed(0, 10);
            tower.setCurrentMoveSpeed(0, 0);
            tower.lvl = 420;
            break;
        case 3:
            tower = gameObject(_x, _y, 105, 65, "Duet", 150, 1, 1.0, false, 0, 0, 0.05f, 0, 3);
            break;
    }
    for (int i = 0; i < (currentWave==1?0:(currentWave - 2)); i++) {
        tower.levelUp();
    }
    // wycentrowanie
    tower.rect.x = _x - tower.rect.w / 2;
    tower.rect.y = _y - tower.rect.h / 2;
    // można było by to lepiej zooptymalizować xd
    // tower.lvl = currentWave==1?1:(currentWave - 1);
    // for (int i = 1; i < tower.lvl; i++) {
    //     tower.levelUp();
    // }
    // clamp żeby w ekranie się zmieściło
    if (tower.rect.x < 0) tower.rect.x = 0;
    if (tower.rect.y < 0) tower.rect.y = 0;
    if (tower.rect.x + tower.rect.w > ScreenSize::getWidth()) tower.rect.x = ScreenSize::getWidth() - tower.rect.w;
    if (tower.rect.y + tower.rect.h > ScreenSize::getHeight()) tower.rect.y = ScreenSize::getHeight() - tower.rect.h;

    towers.push_back(tower);
}

bool isStillWave() {
    if(activeWave && enemies.size()==0) {
        return false;
    }else if (activeWave==false){
        return false;
    }else {
        return true;
    }
}
// Logika odpalania fal i spawnowania przeciwników
void startWave() {
    if (gameWon) return;
    activeWave=isStillWave();
    if(currentWave<=maxWave && !activeWave) {
        const int _enemyCount = 10 + (currentWave - 1) * 2;
        const int _enemySpread = 500 + (currentWave - 1) * 50;
        // const int _enemyCount = 1;
        // const int _enemySpread = 200;
        for (int i = 0; i < _enemyCount; i++) {
            float basePower = 1.0f + (currentWave - 1) * 0.1f;
            float rnd = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); // 0..1
            float randomScale = 0.85f + rnd * 0.3f; // ~0.85 .. 1.15
            float power = basePower * randomScale;
            float sizeScale = 0.7f + rnd * 0.6f; // ~0.7 .. 1.3

            int w = static_cast<int>(58 * sizeScale);
            int h = static_cast<int>(68 * sizeScale);
            if (w < 20) w = 20;
            if (h < 30) h = 30;
            if (w > 120) w = 120;
            if (h > 170) h = 170;

            int hp = static_cast<int>(50 * power);
            int dmg = static_cast<int>(10 * power);
            int attackForce = static_cast<int>(1.0f * power);

            float baseSpeed = 2.0f;
            float velX = - (baseSpeed / power);
            if (velX < -6.0f) velX = -6.0f;
            if (velX > -0.5f) velX = -0.5f;

            gameObject enemy = gameObject(
                ScreenSize::getWidth() + rand() % _enemySpread,
                (rand() % (ScreenSize::getHeight() - 120)) + 120,
                w, h, "Enemy", hp, dmg, attackForce, true, velX, 0, 0.02f, 70 * power);
            enemies.push_back(enemy);
            }
        activeWave=true;
        currentWave++;
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
    if (activeWave && enemies.empty()) {
        activeWave = false;
    }

    // TODO Dodać do klasy gameobject funkcję onDestroy która się tu odpala, tam wrzucimy logike na dodawanie siana itd
    std::erase_if(towers, [](const gameObject &t) {
        // displayDebug();
        return t.destroy;
    });
    std::erase_if(enemies, [](const gameObject &e) {
        if (e.isAttackingAGH) {
            playerHealth -= e.attackDamage;
        }

        return e.destroy;

    });

    if (selectedTower && selectedTower->destroy) {
        selectedTower = nullptr;
    }
    if (currentWave > maxWave && enemies.empty() && !activeWave && !gameLost) {
        gameWon = true;
        showReplayButton = true;

        replayButton.rect.w = 240;
        replayButton.rect.h = 60;
        replayButton.rect.x = ScreenSize::getWidth() / 2 - replayButton.rect.w / 2;
        replayButton.rect.y = ScreenSize::getHeight() / 2 + 60;

        replayButton.isActionButton = true;
        replayButton.color = {0, 100, 0, 255};
    }
}

void onPlayerDeath() {
    enemies.clear();
    towers.clear();
    gameLost = true;

    defeatReplayButton.rect.w = 240;
    defeatReplayButton.rect.h = 60;
    defeatReplayButton.rect.x = ScreenSize::getWidth() / 2 - defeatReplayButton.rect.w / 2;
    defeatReplayButton.rect.y = ScreenSize::getHeight() / 2 + 60;

    defeatReplayButton.isActionButton = true;
    defeatReplayButton.color = {150, 0, 0, 255};
}




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
    bool isPaused = false;
    SDL_Rect player{10, 10, 10, 20};
    // 0 - brak 1 - piechota 2 - killdozer 3 - działko
    int current_tower = 0;
    topBar topBar(ScreenSize::getWidth());

    SDL_CreateWindowAndRenderer(ScreenSize::getWidth(), ScreenSize::getHeight(), 0, &window, &renderer);
    TextRenderer notficiationsTextRenderer(renderer, "assets/GravitasOne-Regular.ttf", 16);
    TextRenderer bigTextRenderer(renderer, "assets/GravitasOne-Regular.ttf", 72);
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

    // auto tower_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 2, 6);
    if (!tower_surface) {
        cout << "IMG_Load error: " << IMG_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    auto tower_texture = SDL_CreateTextureFromSurface(renderer, tower_surface);
    SDL_FreeSurface(tower_surface);
    if (!tower_texture) {
        cout << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    auto tower2_texture = SDL_CreateTextureFromSurface(renderer, tower2_surface);
    SDL_FreeSurface(tower2_surface);
    if (!tower2_texture) {
        cout << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 2;
    }
    auto tower3_texture = SDL_CreateTextureFromSurface(renderer, tower3_surface);
    SDL_FreeSurface(tower3_surface);
    if (!tower3_texture) {
        cout << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 3;
    }


    if (!enemy_surface) {
        cout << "IMG_Load error: " << IMG_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    auto enemy_texture = SDL_CreateTextureFromSurface(renderer, enemy_surface);
    SDL_FreeSurface(enemy_surface);
    if (!enemy_texture) {
        cout << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Resetowanie w pamięci (jednak przydatne xdd)
    SDL_SetRenderTarget(renderer, background_texture);
    SDL_RenderClear(renderer);

    // SDL_SetRenderTarget(renderer, player_texture);
    // SDL_SetRenderDrawColor(renderer, 204, 102, 0, 255);
    // SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, tower_texture);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, nullptr);


    Button uiButton; // renderuje przycisk

    NotificationManager notification_manager(renderer, &notficiationsTextRenderer);

    // czasem się nie resetuje to daje na sztywno
    // currentWave = 1;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (gameLost) {
                int dummy = 0;
                if (defeatReplayButton.handleEvent(e, dummy)) {
                    // RESET GRY
                    gameLost = false;
                    gameWon = false;
                    activeWave = false;
                    currentWave = 1;
                    playerHealth = 100.0f;

                    towers.clear();
                    enemies.clear();
                    uiEkonomia.kasa = uiEkonomia.kasaStartowa;

                    notification_manager.add("Nowa gra rozpoczęta!");
                    continue;
                }
            }
            if (gameWon && showReplayButton) {
                int dummy = 0;
                if (replayButton.handleEvent(e, dummy)) {
                    gameWon = false;
                    showReplayButton = false;
                    activeWave = false;
                    currentWave = 1;
                    playerHealth = 100.0f;

                    towers.clear();
                    enemies.clear();
                    uiEkonomia.kasa = uiEkonomia.kasaStartowa;

                    notification_manager.add("Nowa gra rozpoczęta!");
                    continue;
                }
            }
            // inputBox.handleEvent(e);
            // if (inputBox.isActive())
            //  continue;
            bool uiConsumed = false;
            int prev_tower = current_tower;
            uiConsumed = topBar.handleEvent(e, current_tower);

            if (uiConsumed &&
               e.type == SDL_MOUSEBUTTONDOWN &&
               e.button.button == SDL_BUTTON_LEFT &&
               current_tower != prev_tower)
            {
                switch (current_tower) {
                    case 1: notification_manager.add("Wybrałes stodenta"); break;
                    case 2: notification_manager.add("Wybrałes koparke"); break;
                    case 3: notification_manager.add("Wybrałes sklep monopolowy"); break;
                    default: break;
                }
            }

            if (topBar.startWaveClicked) {
                uiEkonomia.liczenie(50 + (currentWave * 100),true);
                startWave();
                topBar.resetStartWaveClicked();
            }
            if (!uiConsumed && selectedTower) {
                uiConsumed = stats_box.handleEvent(e, *selectedTower, notification_manager, uiEkonomia);
            }

            if (uiConsumed)
                continue;
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
                        break;
                    // case SDLK_q:
                    //     startWave();
                    //     break;
                    case SDLK_p:
                        isPaused = true;
                        int menuresult = showmenu(renderer, &notficiationsTextRenderer);
                        std::cout << "Menu result: " << menuresult << std::endl;
                        if (menuresult == 1) {
                            std::cout << "Setting running = false" << std::endl;
                            running = false;
                        }
                        isPaused = false;
                        break;
                }


                // cout << current_tower << endl;
            }


            // stawianie wieży w pozycji kursora
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int kosztjednostki = 0;
                // można stawiać tylko w lewej połowie ekranu
                int towerHeight = 0;
                switch (current_tower) {
                    case 1: towerHeight = 100;

                        kosztjednostki = 10 + (currentWave==1?0:(currentWave - 2))* 5;
                        break;
                    case 2: towerHeight = 100;

                        kosztjednostki = 50 + (currentWave==1?0:(currentWave - 2)) * 5;
                        break;
                    case 3: towerHeight = 100;

                        kosztjednostki = 75 + (currentWave==1?0:(currentWave - 2)) * 5;
                        break;
                }
                if (e.button.button == SDL_BUTTON_LEFT &&
                    e.button.x <= ScreenSize::getWidth() / 2 &&
                    current_tower != 0 &&
                    e.button.y - towerHeight / 2 >= topBar.rect.h) {
                    //sprawdzenie czy nie nachodzi na topbar
                    double distance = 0; // zmienne do przechowywania dystansu i czy jednsotka moze byc postawiona
                    float can_be_placed = true;
                    if (current_tower == 1) {
                        for (int i = 0; i < towers.size(); i++) {
                            distance = gameObject::calculateDistance(
                                towers[i], gameObject(e.button.x, e.button.y, 20, 30, "Infantry Tower", 100, 10, 1.0));
                            // cout << "Distance: " << distance << " [i]" << i << endl;
                            if (distance < 50) {
                                can_be_placed = false;
                                // to jest zepsute, bo psuje gameplay zatrzymując całą grę
                                // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Komunikat", "Nie mozesz postawic tutaj jednostki, znajduje sie ona zbyt blisko innej", NULL);
                                break;
                            }
                        }
                    } else if (current_tower == 3) {
                        for (int i = 0; i < towers.size(); i++) {
                            distance = gameObject::calculateDistance(
                                towers[i], gameObject(e.button.x, e.button.y, 20, 30, "Infantry Tower", 100, 10, 1.0));
                            if (distance < 75) {
                                can_be_placed = false;
                                // to jest zepsute, bo psuje gameplay zatrzymując całą grę
                                // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Komunikat", "Nie mozesz postawic tutaj jednostki, znajduje sie ona zbyt blisko innej", NULL);
                                break;
                            }
                        }
                    }
                    if (can_be_placed == true && current_tower != 0) {
                        selectedTower = nullptr;
                        // inputBox.open(e.button.x, e.button.y, current_tower);
                        if (uiEkonomia.getMoney() >= kosztjednostki) {
                            bool dodawanie = false;
                            uiEkonomia.liczenie(kosztjednostki, dodawanie);//tutaj dodać odejmowanie kasy

                            spawnTower(
                                e.button.x,
                                e.button.y,
                                current_tower

                        );
                        }else {
                            notification_manager.add("jestes za biedny");
                        }
                    } else {
                        selectedTower = nullptr;
                        notification_manager.add("Za bilisko! Nie mozna postawic jednostki.");
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
        // ---------- Fizyka/Logika ----------
        // sprawdzanie kolizji
        // Przechodzimy przez każdy obiekt w wektorze (używamy referencji & żeby nie kopiować)
        if (!gameWon && !gameLost) {
            checkCollisions();
        }
        // Umieranie
        if (playerHealth <= 0 && !gameLost) {
            onPlayerDeath();
        }

        // -------------- render --------------

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background_texture, nullptr, nullptr);
        isStillWave();
        topBar.render(renderer, current_tower, uiEkonomia.getMoney(), notficiationsTextRenderer, currentWave, isStillWave(), playerHealth, maxWave);
        // TODO - renderowanie wież i przeciwników z ich grafikami, zamiast tej samej do wszystkiego
        float deltaTime = (SDL_GetTicks() - lastTickTime) / 1000.0f;
        for (auto &t: towers) {
            if (t.imageID == 1) {
                SDL_RenderCopy(renderer, tower_texture, nullptr, &t.rect);
            } else if (t.imageID == 3) {
                SDL_RenderCopy(renderer, tower3_texture, nullptr, &t.rect);
                if (isStillWave()) {
                    uiEkonomia.kasa += t.lvl * 0.5 * deltaTime; // 50 groszy na sekunde * level, im wyzszy level tym wiecej kasy
                }

            } else if (t.imageID == 2) {
                // Renderowanie koparki na końcu. chyba to będzie bardziej wydajniejsze bo one będą rzadziej wykorzystywane
                SDL_RenderCopy(renderer, tower2_texture, nullptr, &t.rect);
            }
            // SDL_RenderCopy(renderer, tower_texture, nullptr, &t.rect);
            t.update();
        }
        for (auto &e: enemies) {
            SDL_RenderCopy(renderer, enemy_texture, nullptr, &e.rect);
            e.update();
        }
        notification_manager.render(window);
        notification_manager.update();
        // SDL_RenderCopy(renderer, player_texture, nullptr, &player);

        // czyszczenie pamięci po update, bo tam sprawdzamy kolizje
        gameObjectCleanup();
        // rysuje przycisk
        SDL_Event drawEvent{};
        uiEkonomia.update(renderer);
        inputBox.render(renderer);
        if (selectedTower) {


            stats_box.render(
                renderer,
                notficiationsTextRenderer,
                *selectedTower

            );
        }
        if (gameWon) {
            const char* winText = "WYGRANA!";
            int textW, textH;
            bigTextRenderer.measure(winText, textW, textH);

            int x = ScreenSize::getWidth() / 2 - textW / 2;
            int y = ScreenSize::getHeight() / 2 - textH;

            SDL_Color gold = {255, 215, 0, 255};
            bigTextRenderer.render(winText, x, y, gold);

            if (showReplayButton) {
                replayButton.render(renderer);

                notficiationsTextRenderer.render(
                    "ZAGRAJ PONOWNIE",
                    replayButton.rect.x + 20,
                    replayButton.rect.y + 18,
                    {255, 255, 255, 255}
                );
            }
        }
        if (gameLost) {
            // SZARY OVERLAY
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 220);
            SDL_Rect overlay = {0, 0, ScreenSize::getWidth(), ScreenSize::getHeight()};
            SDL_RenderFillRect(renderer, &overlay);

            // NAPIS PORAŻKA
            const char* loseText = "PRZEGRANA";
            int textW, textH;
            bigTextRenderer.measure(loseText, textW, textH);

            int x = ScreenSize::getWidth() / 2 - textW / 2;
            int y = ScreenSize::getHeight() / 2 - textH;

            SDL_Color red = {200, 30, 30, 255};
            bigTextRenderer.render(loseText, x, y, red);

            // PRZYCISK
            defeatReplayButton.render(renderer);
            notficiationsTextRenderer.render(
                "ZAGRAJ PONOWNIE",
                defeatReplayButton.rect.x + 20,
                defeatReplayButton.rect.y + 18,
                {255, 255, 255, 255}
            );
        }

            SDL_RenderPresent(renderer);
            lastTickTime = SDL_GetTicks();
            SDL_Delay(10);
            continue;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();


    return 0;
}