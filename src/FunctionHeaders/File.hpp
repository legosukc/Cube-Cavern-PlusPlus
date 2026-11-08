#pragma once

#include <string>

#include <SDL3/SDL_filesystem.h>

namespace Utils::File {

    extern std::string PreferredPath;

    inline void Init(const char* OrganisationName, const char* AppName) {
        char* PreferredPath = SDL_GetPrefPath(OrganisationName, AppName);
        Utils::File::PreferredPath = PreferredPath;
        SDL_free(PreferredPath);
    }

    char* GetPathInAssetsChar(const char* Path, size_t PathLen);
    char* GetPathInAssetsChar(const char* Path);

    char* GetPathInDataChar(const char* Path, size_t PathLen);
    char* GetPathInDataChar(const char* Path);
}