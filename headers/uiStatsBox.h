//
// Created by mateu on 20.01.2026.
//

#ifndef UNIPROJECT_UISTATSBOX_H
#define UNIPROJECT_UISTATSBOX_H
#include <gameObject.h>
#include <SDL.h>
#include "TextRenderer.h"
class uiStatsBox {
public:
    static void render(SDL_Renderer* renderer,
                       TextRenderer& text,
                       const gameObject& obj);
};
#endif //UNIPROJECT_UISTATSBOX_H