#pragma once

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_iostream.h>

#include <json.hpp>

#include "File.hpp"


namespace {

	nlohmann::json _OpenConfig(const char* ConfigPath, const char* DefaultConfigPath) {

		const std::string FullConfigPath = Utils::File::PreferredPath + ConfigPath;

		if (!SDL_GetPathInfo(FullConfigPath.c_str(), NULL)) {
			SDL_CopyFile(DefaultConfigPath, FullConfigPath.c_str());
		}

		char* JSONText = static_cast<char*>(SDL_LoadFile(FullConfigPath.c_str(), NULL));

		nlohmann::json NewJSON = nlohmann::json::parse(JSONText);

		SDL_free(JSONText);

		return NewJSON;
	}
}


namespace Game::Config {

	nlohmann::json ConfigJSON;
	nlohmann::json GlobalConfigJSON;

	inline void OpenConfig(const char* ConfigPath, const char* DefaultConfigPath) {
		Game::Config::ConfigJSON = ::_OpenConfig(ConfigPath, DefaultConfigPath);
	}

	inline void Init() {
		Game::Config::GlobalConfigJSON = ::_OpenConfig("GlobalCfg.json", "defaultGlobalCfg.json");
	}
}