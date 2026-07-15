#pragma once

#include "../define.h"

#include <iostream>
#include <string>

#include <SDL3/SDL_filesystem.h>


namespace Utils::File {

	std::string PreferredPath;

	inline void Init(const char* OrganisationName, const char* AppName) {

		char* PreferredPath = SDL_GetPrefPath(OrganisationName, AppName);
		Utils::File::PreferredPath = PreferredPath;
		std::cout << "Preferred path for writing: " << PreferredPath << std::endl;
		SDL_free(PreferredPath);
	}
}