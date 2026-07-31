#include "define.h"

#include "Game.hpp"

namespace Game {
#ifdef BUILD_CLIENT
    Game::Classes::Window Window;
#endif
}