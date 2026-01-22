

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
    // float &playerHealthRef;
public:
    SDL_Rect rect{0,0,0,0};
    std::string name;
    // Pozycję do przemieszczania się
    float trueXPos;
    float trueYPos;
    int lvl;
    float hp;
    float maxYSpeed = 0;
    float maxXSpeed = 0;
    float startPosX = 0;
    float startPosY = 0;
    float attackDamage;
    float attackForce;
    float currentYSpeed = 0;
    float currentXSpeed = 0;
    bool isAttackingAGH = false;
    // w m/s^2
    float accelerationSpeed = 0.05f;
    // w kg
    float mass = 1;
    bool isEnemy = false;
    // jak jest true to usuwamy z listy obiektów
    bool destroy = false;
    SDL_Texture *image = nullptr;
    gameObject();
    gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float _attackDamage, float _attackForce);
    gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float _attackDamage, float _attackForce, bool _isEnemy);
    gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float _attackDamage, float _attackForce, bool _isEnemy, float _maxSpeedX, float _maxSpeedY);
    gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float _attackDamage, float _attackForce, bool _isEnemy, float _maxSpeedX, float _maxSpeedY, float _accelerationSpeed, float _mass);
    gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float _attackDamage, float _attackForce, bool _isEnemy, float _maxSpeedX, float _maxSpeedY, float _accelerationSpeed, float _mass, SDL_Texture* _image);
    // gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float _attackDamage, float _attackForce, bool _isEnemy, float _maxSpeedX, float _maxSpeedY, float _accelerationSpeed, float _mass, SDL_Texture* _image, float &_playerHealth);
    void moveBySpeed();

void moveToPoint(int _x, int _y);

void setCurrentMoveSpeed(float _ySpeed, float _xSpeed);
    void setMaxMoveSpeed(float _ySpeed, float _xSpeed);
    void update();
    float getDistance(gameObject other);
    static float calculateDistance(gameObject a, gameObject b);
    void combatWith(gameObject &enemy);

void levelUp();

std::vector<gameObject*> checkCollisions(std::vector<gameObject> &others);
};

#endif //UNIPROJECT_GAMEOBJECT_H