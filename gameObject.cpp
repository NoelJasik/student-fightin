//
// Created by starflea on 11/20/25.
//

#include "gameObject.h"
#include <string>

gameObject::gameObject() {
    rect.x = 0;
    rect.y = 0;
    rect.w = 0;
    rect.h = 0;
    name = "default";
    hp = 100;
}
gameObject::gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp) {
        rect.x = _x;
        rect.y = _y;
        rect.w = _w;
        rect.h = _h;
        name = _name;
        hp = _hp;
}



