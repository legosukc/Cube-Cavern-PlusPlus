#pragma once

#include <string>

#include <SDL3/SDL_filesystem.h>

namespace Utils::File {

    extern std::string PreferredPath;

    inline void Init(const char* OrganisationName, const char* AppName);
}