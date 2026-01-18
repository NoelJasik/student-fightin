//
// Created by starflea on 11/20/25.
//

#include "../headers/gameObject.h"
#include <string>
#include <thread>


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

// wywoływane co klatkę
void gameObject::update() {
   moveBySpeed();
}



