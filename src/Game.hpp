#pragma once

#include "define.h"

#ifdef BUILD_CLIENT
#include "Client/SDLClasses/Window.hpp"
#endif


namespace Game {
#ifdef BUILD_CLIENT
    extern Game::Classes::Window Window;
#endif

    void Update();
    void Draw();
}