#include "File.hpp"

#include <iostream>
#include <string>

#include <SDL3/SDL_filesystem.h>

void Utils::File::Init(const char* OrganisationName, const char* AppName) {
    char* PreferredPath = SDL_GetPrefPath(OrganisationName, AppName);
    Utils::File::PreferredPath = PreferredPath;
    SDL_free(PreferredPath);
}