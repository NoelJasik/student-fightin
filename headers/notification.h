//
// Created by mateu on 14.01.2026.
//

#ifndef UNIPROJECT_NOTIFICATION_H
#define UNIPROJECT_NOTIFICATION_H
#pragma once
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

struct Notification {
    std::string text;
    Uint32 startTime;
    Uint32 duration;
    Notification(const std::string& text, Uint32 duration = 15000);
};

class NotificationManager {
public:
    NotificationManager(SDL_Renderer* renderer, TTF_Font* font);

    void add(const std::string& tex);
    void update();
    void render(SDL_Window* window);
    void handleEvent(const SDL_Event& e, SDL_Window* window);
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<Notification> notifications;

};
#endif //UNIPROJECT_NOTIFICATION_H