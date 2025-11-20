//
// Created by starflea on 11/20/25.
//

#include "gameObject.h"
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

// wywoływane co klatkę
void gameObject::update() {
   moveBySpeed();
}



