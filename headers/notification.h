//
// Created by mateu on 14.01.2026.
//

#ifndef UNIPROJECT_NOTIFICATION_H
#define UNIPROJECT_NOTIFICATION_H
#pragma once
#include <SDL.h>
#include <string>

struct Notification {
public:
    std::string text;
    Uint32 startTime;
    Uint32 duration;
    Notification(const std::string& text, Uint32 duration)
       : text(text),
         startTime(SDL_GetTicks()),
         duration(duration) {}
};

#endif //UNIPROJECT_NOTIFICATION_H