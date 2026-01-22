//
// Created by Dawid Roś on 18/01/2026.
//

#ifndef UNIPROJECT_EKONOMIA_H
#define UNIPROJECT_EKONOMIA_H


class ekonomia {
    int kasa = 100;
public:
    void odejmowaniekasy(int naleznosc);
    void dodawaniekasy();
    ekonomia();



    void update(SDL_Renderer * renderer);
    int getMoney();
    void liczenie(int naleznosc, bool dodawanie) {
        if (dodawanie) kasa = kasa + naleznosc;
            else {
                kasa = kasa - naleznosc;
             }
    };

};


#endif //UNIPROJECT_EKONOMIA_H