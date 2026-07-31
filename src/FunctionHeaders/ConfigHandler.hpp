#pragma once

#include "../define.h"

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_iostream.h>

#include "../nlohmann/json.hpp"


namespace Game::Config {

	extern nlohmann::json ConfigJSON;
	extern nlohmann::json GlobalConfigJSON;

	inline void OpenConfig(const char* ConfigPath, const char* DefaultConfigPath);

	inline void Init();
}