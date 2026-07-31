#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include "../define.h"

#include <iostream>

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_iostream.h>

#include "../nlohmann/json.hpp"

#include "File.hpp"

namespace Game::Config {

    extern nlohmann::json ConfigJSON;
    extern nlohmann::json GlobalConfigJSON;

    inline void OpenConfig(const char* ConfigPath,
                           const char* DefaultConfigPath);

    inline void Init();
}

namespace {
    static nlohmann::json _ParseConfig(const char* JSONText,
                                       bool UseDefaultConfig,
                                       const char* DefaultConfigPath) {
        nlohmann::json NewJSON;
        try {
            NewJSON = nlohmann::json::parse(JSONText);
        } catch (std::exception Exception) {
            std::cerr << "Caught exception while parsing JSON: "
                      << Exception.what() << '\n';
            if (UseDefaultConfig) {
                std::cerr << "Default config: " << DefaultConfigPath
                          << " is probably corrupted. (yikes!)" << std::endl;
            } else {
                std::cerr << "Trying default config: " << DefaultConfigPath
                          << std::endl;

                char* DefaultJSONText =
                    static_cast<char*>(SDL_LoadFile(DefaultConfigPath, NULL));
                NewJSON =
                    ::_ParseConfig(DefaultJSONText, true, DefaultConfigPath);
                SDL_free(DefaultJSONText);

                return NewJSON;
            }
        }
        return NewJSON;
    }
}

static nlohmann::json _OpenConfig(const char* ConfigPath,
                                  const char* DefaultConfigPath) {
    const std::string FullConfigPath = Utils::File::PreferredPath + ConfigPath;

    const bool UseDefaultConfig =
        !SDL_GetPathInfo(FullConfigPath.c_str(), NULL);

    if (UseDefaultConfig) {
        SDL_CopyFile(DefaultConfigPath, FullConfigPath.c_str());
    }

    char* JSONText =
        static_cast<char*>(SDL_LoadFile(FullConfigPath.c_str(), NULL));

    nlohmann::json NewJSON =
        ::_ParseConfig(JSONText, UseDefaultConfig, DefaultConfigPath);

    SDL_free(JSONText);
    return NewJSON;
}

namespace Game::Config {
    nlohmann::json ConfigJSON;
    nlohmann::json GlobalConfigJSON;
}

void Game::Config::OpenConfig(const char* ConfigPath,
                              const char* DefaultConfigPath) {
    Game::Config::ConfigJSON = ::_OpenConfig(ConfigPath, DefaultConfigPath);
}

void Game::Config::Init() {
    Game::Config::GlobalConfigJSON =
        ::_OpenConfig("GlobalCfg.json", "defaultGlobalCfg.json");
}

#endif