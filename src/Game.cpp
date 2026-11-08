#include "Game.hpp"

#include "define.h"

#ifdef BUILD_CLIENT
#include "Client/SDLClasses/Window.hpp"

namespace Game {
    Game::Classes::Window Window;
}
#endif