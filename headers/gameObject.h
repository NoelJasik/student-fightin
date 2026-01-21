

#ifndef UNIPROJECT_GAMEOBJECT_H
#define UNIPROJECT_GAMEOBJECT_H
#pragma once
#include <SDL.h>
#include <string>
#include <vector>

class gameObject {
// robie wszystko publiczne bo mam wywalone w optymalizacje, a do debugowania/pracy jest łatwiej
private:
    float lastUpdateTime;
public:
    SDL_Rect rect{0,0,0,0};
    std::string name;
    int lvl;
    float hp;
    float maxYSpeed = 0;
    float maxXSpeed = 0;
    float attackDamage;
    float attackForce;
    float currentYSpeed = 0;
    float currentXSpeed = 0;
    float accelearationSpeed = 0.05f;
    bool isEnemy = false;
    // jak jest true to usuwamy z listy obiektów
    bool destroy = false;
    gameObject();
    gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float _attackDamage, float _attackForce);
    gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float _attackDamage, float _attackForce, bool _isEnemy);
    gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float _attackDamage, float _attackForce, bool _isEnemy, float _maxSpeedX, float _maxSpeedY);
    void moveBySpeed();
    void setCurrentMoveSpeed(float _ySpeed, float _xSpeed);
    void setMaxMoveSpeed(float _ySpeed, float _xSpeed);
    void update();
    float getDistance(gameObject other);
    static float calculateDistance(gameObject a, gameObject b);
    void combatWith(gameObject &enemy);

    std::vector<gameObject*> checkCollisions(std::vector<gameObject> &others);
};

#endif //UNIPROJECT_GAMEOBJECT_H