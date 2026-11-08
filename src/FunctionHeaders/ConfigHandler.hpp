#pragma once

#include "../nlohmann/json.hpp"

namespace Game::Config {

    extern nlohmann::json ConfigJSON;
    extern nlohmann::json GlobalConfigJSON;

    void OpenConfig(const char* ConfigPath,
                           const char* DefaultConfigPath);

    void Init();
}