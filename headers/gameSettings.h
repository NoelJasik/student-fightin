//
// Created by magnussen on 20.01.2026.
//

#ifndef UNIPROJECT_GAMESETTINGS_H
#define UNIPROJECT_GAMESETTINGS_H

namespace gameSettings {

    class gameSettings {
    };

    // Dałem tutaj żeby w innych plikach CPP się dało zaimportować
    class ScreenSize {
    private:
        static const int _HEIGHT = 720;
        static const int _WIDTH = 1280;

    public:
        static int getWidth() {
            return _WIDTH;
        }

        static int getHeight() {
            return _HEIGHT;
        }
    };
} // gameSettings

#endif //UNIPROJECT_GAMESETTINGS_H