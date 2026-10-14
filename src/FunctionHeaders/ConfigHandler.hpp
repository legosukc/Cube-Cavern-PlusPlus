#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include "../define.h"

#include "../nlohmann/json.hpp"

namespace Game::Config {

    inline nlohmann::json ConfigJSON;
    inline nlohmann::json GlobalConfigJSON;

    void OpenConfig(const char* ConfigPath,
                           const char* DefaultConfigPath);

    void Init();
}

#endif