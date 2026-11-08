#include "LuaConsole.hpp"

#include <iostream>
#include <string>

#include <SDL3/SDL_thread.h>

namespace Game::Lua::CLibraries::Console::etc {
    SDL_Thread* _OnPromptThread = NULL;

    volatile bool _RunThread = true;
    volatile bool _NewPrompt = false;
    std::string _Prompt;
}

int Game::Lua::CLibraries::Console::etc::_OnPromptThreadFunc(void*) {
    using namespace Game::Lua::CLibraries::Console::etc;
    if (!SDL_SetCurrentThreadPriority(
            SDL_ThreadPriority::SDL_THREAD_PRIORITY_LOW)) {
        std::clog << "Failed to set thread priority for _OnPromptThreadFunc."
                  << std::endl;
    }

    while (_RunThread) {
        std::getline(std::cin, _Prompt);
        _NewPrompt = true;
    }

    return -1;
}