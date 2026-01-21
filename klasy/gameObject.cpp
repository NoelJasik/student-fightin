//
// Created by starflea on 11/20/25.
//

#include "../headers/gameObject.h"
#include <string>
#include <thread>
#include <vector>
#include <SDL.h>
#include <iostream>
#include "../headers/gameSettings.h"

using namespace gameSettings;

gameObject::gameObject() {
    rect.x = 0;
    rect.y = 0;
    rect.w = 0;
    rect.h = 0;
    name = "default";
    hp = 100;
    update();
}

gameObject::gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float _attackdamage,
                       float _attackspeed) {
    lvl=1;
    rect.x = _x;
    rect.y = _y;
    rect.w = _w;
    rect.h = _h;
    trueXPos = _x;
    trueYPos = _y;
    name = _name;
    attackDamage = _attackdamage;
    attackForce = _attackspeed;
    hp = _hp;
    update();
}

gameObject::gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float _attackdamage,
                       float _attackspeed, bool _isEnemy) {
    rect.x = _x;
    rect.y = _y;
    rect.w = _w;
    rect.h = _h;
    trueXPos = _x;
    trueYPos = _y;
    name = _name;
    hp = _hp;
    attackDamage = _attackdamage;
    attackForce = _attackspeed;
    isEnemy = _isEnemy;
    update();
}

gameObject::gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float _attackdamage,
                       float _attackspeed, bool _isEnemy, float _maxSpeedX, float _maxSpeedY) {
    lvl=1;
    rect.x = _x;
    rect.y = _y;
    rect.w = _w;
    rect.h = _h;
    trueXPos = _x;
    trueYPos = _y;
    name = _name;
    hp = _hp;
    attackDamage = _attackdamage;
    attackForce = _attackspeed;
    isEnemy = _isEnemy;
    setMaxMoveSpeed(_maxSpeedY, _maxSpeedX);
    // setCurrentMoveSpeed(_maxSpeedY, _maxSpeedX);
    update();
}

gameObject::gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float _attackdamage,
                       float _attackspeed, bool _isEnemy, float _maxSpeedX, float _maxSpeedY, float _accelerationSpeed, float _mass) {
    lvl=1;
    rect.x = _x;
    rect.y = _y;
    rect.w = _w;
    rect.h = _h;
    trueXPos = _x;
    trueYPos = _y;
    name = _name;
    hp = _hp;
    attackDamage = _attackdamage;
    attackForce = _attackspeed;
    isEnemy = _isEnemy;
    accelerationSpeed = _accelerationSpeed;
    mass = _mass;
    setMaxMoveSpeed(_maxSpeedY, _maxSpeedX);
    // setCurrentMoveSpeed(_maxSpeedY, _maxSpeedX);
    update();
}
gameObject::gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, float _attackdamage,
                       float _attackspeed, bool _isEnemy, float _maxSpeedX, float _maxSpeedY, float _accelerationSpeed, float _mass, SDL_Texture* _image) {
    lvl=1;
    rect.x = _x;
    rect.y = _y;
    rect.w = _w;
    rect.h = _h;
    trueXPos = _x;
    trueYPos = _y;
    name = _name;
    hp = _hp;
    attackDamage = _attackdamage;
    attackForce = _attackspeed;
    isEnemy = _isEnemy;
    accelerationSpeed = _accelerationSpeed;
    mass = _mass;
    image = _image;
    setMaxMoveSpeed(_maxSpeedY, _maxSpeedX);
    // setCurrentMoveSpeed(_maxSpeedY, _maxSpeedX);
    update();
}




void gameObject::moveBySpeed() {
    float deltaTime = static_cast<float>(SDL_GetTicks64() - lastUpdateTime) / 1000.0f;

    // if (deltaTime <= 0) {
    //     return; // Unikaj dzielenia przez zero lub negatywnego deltaTime
    // }
    if (mass == 0) {
        return;
    }

    // Przyśpieszenie dla osi X
    if (maxXSpeed > 0) { // Poruszanie w prawo lub brak ruchu
       if (currentXSpeed < maxXSpeed) {
           currentXSpeed += accelerationSpeed * mass * deltaTime;
       } else {
           currentXSpeed = maxXSpeed;
       }
    }
    else if (maxXSpeed < 0) { // Poruszanie w lewo (maxXSpeed jest ujemne)
        if (currentXSpeed > maxXSpeed) {
            currentXSpeed -= accelerationSpeed * mass * deltaTime;
        }
        else {
            currentXSpeed = maxXSpeed;
        }
    } else if (maxXSpeed == 0) {
        if (currentXSpeed > 0.001f) {
            currentXSpeed -= accelerationSpeed * mass * deltaTime;
        } else if (currentXSpeed < -0.001f) {
            currentXSpeed += accelerationSpeed * mass * deltaTime;
        } else {
            currentXSpeed = 0;
        }

    }
    // std::cout<< currentXSpeed << " " << name << std::endl;

    // Aktualizacja
    // std::cout << (currentXSpeed * deltaTime * 100) << std::endl; // debug
    trueXPos += currentXSpeed;
    trueYPos += currentYSpeed;
    moveToPoint(trueXPos, trueYPos);
    lastUpdateTime = SDL_GetTicks64();
}

void gameObject::moveToPoint(int _x, int _y) {
rect.x = _x - rect.w / 2;
    rect.y = _y - rect.h / 2;
}


void gameObject::setMaxMoveSpeed(float _ySpeed, float _xSpeed) {
    maxXSpeed = _xSpeed;
    maxYSpeed = _ySpeed;
}

void gameObject::setCurrentMoveSpeed(float _ySpeed, float _xSpeed) {
    currentYSpeed = _ySpeed;
    currentXSpeed = _xSpeed;
}

float gameObject::getDistance(gameObject other) {
    float thisCenterX = rect.x + rect.w / 2;
    float thisCenterY = rect.y + rect.h / 2;
    float otherCenterX = other.rect.x + other.rect.w / 2;
    float otherCenterY = other.rect.y + other.rect.h / 2;
    float deltaX = otherCenterX - thisCenterX;
    float deltaY = otherCenterY - thisCenterY;
    return sqrtf(deltaX * deltaX + deltaY * deltaY);
}

float gameObject::calculateDistance(gameObject a, gameObject b) {
    return a.getDistance(b);
}

// Metoda zwraca listę wskaźników na obiekty, z którymi ten konkretny obiekt koliduje
std::vector<gameObject *> gameObject::checkCollisions(std::vector<gameObject> &others) {
    std::vector<gameObject *> collidedObjects;

    for (auto &obj: others) {
        // Sprawdź, czy nie sprawdzamy kolizji z samym sobą, po adresie w pamięci
        if (this == &obj) {
            continue;
        }

        // sprawdza czy dwa prostokąty (rect) na siebie nachodzą, równe wielkości grafiki
        // TODO - zmienić na kółko
        if (SDL_HasIntersection(&this->rect, &obj.rect)) {
            // Jeśli jest kolizja, dodajemy wskaźnik na ten obiekt do listy
            collidedObjects.push_back(&obj);
        }
    }

    return collidedObjects;
}

void gameObject::combatWith(gameObject &enemy) {
    enemy.setCurrentMoveSpeed(0,  attackForce);
    setCurrentMoveSpeed(0, -enemy.attackForce);
    enemy.hp -= attackDamage;
    hp -= enemy.attackDamage;
}


// wywoływane co klatkę
void gameObject::update() {
    moveBySpeed();
    if (hp <= 0) {
        destroy = true;
    }

    // prosta logika usuwania poza ekranem


    if (!isEnemy) {
        // Usuwamy jednostki gracza poza ekranem, tej samej logiki się użyje do hp wieży
        if (rect.x > ScreenSize::getWidth() || rect.x < -100) {
            destroy = true;
        }
    } else if (rect.x < -10) {
        destroy = true;
    }
}
