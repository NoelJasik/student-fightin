

#ifndef UNIPROJECT_GAMEOBJECT_H
#define UNIPROJECT_GAMEOBJECT_H
#pragma once
#include <SDL.h>
#include <string>
#include <vector>

class gameObject {
// robie wszystko publiczne bo mam wywalone w optymalizacje, a do debugowania/pracy jest łatwiej
public:
    SDL_Rect rect{0,0,0,0};
    std::string name;
    float hp;
    float ySpeed = 0;
    float xSpeed = 0;
    float attackdamage;
    float attackspeed;
    bool isEnemy = false;
    // jak jest true to usuwamy z listy obiektów
    bool destroy = false;
    gameObject();
    gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float attackdamage, float attackspeed);
    gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float attackdamge, float attackspeed, bool _isEnemy);

    void moveBySpeed();
    void setMoveSpeed(int _ySpeed, int _xSpeed);
    void update();
    float getDistance(gameObject other);
    static float calculateDistance(gameObject a, gameObject b);
    std::vector<gameObject*> checkCollisions(std::vector<gameObject>& others);
};

#endif //UNIPROJECT_GAMEOBJECT_H