#include "Sound.hpp"

#include "../../define.h"

#include <SDL3/SDL_stdinc.h>

namespace Game::Sound {

    AudioBackendEnum AudioBackend = AudioBackendEnum::NoAudio;

    bool AudioEnabled = false;
}