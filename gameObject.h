

#ifndef UNIPROJECT_GAMEOBJECT_H
#define UNIPROJECT_GAMEOBJECT_H
#pragma once
#include <SDL.h>
#include <string>

class gameObject {
// robie wszystko publiczne bo mam wywalone w optymalizacje, a do debugowania/pracy jest łatwiej
public:
    SDL_Rect rect{0,0,0,0};
    std::string name;
    float hp;
    float ySpeed = 0;
    float xSpeed = 0;
    gameObject();
    gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp);
    void moveBySpeed();
    void setMoveSpeed(int _ySpeed, int _xSpeed);
    void update();

};

#endif //UNIPROJECT_GAMEOBJECT_H