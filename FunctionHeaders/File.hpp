#pragma once

#include "../define.h"

#include <string>

#include <SDL3/SDL_filesystem.h>


namespace Utils::File {

	std::string PreferredPath;

	inline void Init(const char* OrganisationName, const char* AppName) {

		char* PreferredPath = SDL_GetPrefPath(OrganisationName, AppName);
		Utils::File::PreferredPath = PreferredPath;
		SDL_free(PreferredPath);
	}
}