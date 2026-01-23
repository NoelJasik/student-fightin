//
// Created by mateu on 20.01.2026.
//

#ifndef UNIPROJECT_UISTATSBOX_H
#define UNIPROJECT_UISTATSBOX_H
#include <gameObject.h>
#include <SDL.h>
#include "TextRenderer.h"
#include "notification.h"
#include "ekonomia.h"
class uiStatsBox {
public:
    SDL_Rect lvlUpButton;

    void render(SDL_Renderer* renderer,
                TextRenderer& text,
                gameObject& obj);

    bool handleEvent(const SDL_Event& e, gameObject& obj, NotificationManager& notification_manager, ekonomia& ekonomia);
};
#endif //UNIPROJECT_UISTATSBOX_H