//
// Created by starflea on 11/20/25.
//

#include "gameObject.h"
#include "SDL.h"
#include <string>

class gameObject {
public:
    SDL_Rect rect;
    std::string name;
    float hp;
    gameObject(int _x, int _y, int _w, int _h, std::string _name, float _hp) {;
        rect.x = _x;
        rect.y = _y;
        rect.w = _w;
        rect.h = _h;
        name = _name;
        hp = _hp;
    }


};