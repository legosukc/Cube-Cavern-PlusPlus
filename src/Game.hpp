#ifndef GAME_H
#define GAME_H

#include "define.h"

#ifdef BUILD_CLIENT
#include "Client/SDLClasses/Window.hpp"
#endif


namespace Game {
#ifdef BUILD_CLIENT
    inline Game::Classes::Window Window;
#endif

    void Update();
    void Draw();
}

#endif