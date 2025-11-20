

#ifndef UNIPROJECT_GAMEOBJECT_H
#define UNIPROJECT_GAMEOBJECT_H
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


#endif //UNIPROJECT_GAMEOBJECT_H