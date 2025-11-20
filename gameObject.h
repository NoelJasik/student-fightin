

#ifndef UNIPROJECT_GAMEOBJECT_H
#define UNIPROJECT_GAMEOBJECT_H
#pragma once
#include <SDL.h>
#include <string>

class gameObject {
public:
    SDL_Rect rect{0,0,0,0};
    std::string name;
    float hp;
    gameObject();
    gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp);

};

#endif //UNIPROJECT_GAMEOBJECT_H