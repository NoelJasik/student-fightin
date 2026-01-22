//
// Created by Dawid Roś on 18/01/2026.
//

#ifndef UNIPROJECT_EKONOMIA_H
#define UNIPROJECT_EKONOMIA_H


class ekonomia {
    int kasa = 100;
public:
    void odejmowaniekasy(int kosztjednostki);

    ekonomia();



    void update(SDL_Renderer * renderer);
    int getMoney();

    void odejmowanie(int kosztjednostki) {

        kasa = kasa - kosztjednostki;
    };
};


#endif //UNIPROJECT_EKONOMIA_H