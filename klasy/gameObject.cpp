//
// Created by starflea on 11/20/25.
//

#include "../headers/gameObject.h"
#include <string>
#include <thread>
#include <vector>


gameObject::gameObject() {
    rect.x = 0;
    rect.y = 0;
    rect.w = 0;
    rect.h = 0;
    name = "default";
    hp = 100;
    update();
}
gameObject::gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp) {
        rect.x = _x;
        rect.y = _y;
        rect.w = _w;
        rect.h = _h;
        name = _name;
        hp = _hp;
        update();
}

gameObject::gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp, bool _isEnemy) {
    rect.x = _x;
    rect.y = _y;
    rect.w = _w;
    rect.h = _h;
    name = _name;
    hp = _hp;
    isEnemy = _isEnemy;
    update();
}



void gameObject::moveBySpeed() {
    rect.x += xSpeed;
    rect.y += ySpeed;
}

void gameObject::setMoveSpeed(int _ySpeed, int _xSpeed) {
    ySpeed = _ySpeed;
    xSpeed = _xSpeed;
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
std::vector<gameObject*> gameObject::checkCollisions(std::vector<gameObject>& others) {
    std::vector<gameObject*> collidedObjects;

    for (auto& obj : others) {
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


// wywoływane co klatkę
void gameObject::update() {
   moveBySpeed();
    if (hp <= 0) {
        destroy = true;
    }

    // prosta logika usuwania poza ekranem
    if (rect.x > 2000 || rect.x < -500) {
        destroy = true;
    }

    if(!isEnemy) {

    }
}



