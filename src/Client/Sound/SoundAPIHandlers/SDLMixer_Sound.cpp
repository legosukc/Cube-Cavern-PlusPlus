#include "SDLMixer_Sound.hpp"

#ifndef SDL_PLATFORM_VITA

#include "../../../define.h"

#include <SDL3_mixer/SDL_mixer.h>

#include "../../../MathClasses/Vector3.hpp"

namespace Game::Sound::SDLMixer {
    MIX_Track* DefaultTrack = NULL;
    MIX_Mixer* Mixer = NULL;
}

void Game::Sound::SDLMixer::Listener_SDLMixer::SetPosition(
    const Math::Vector3& Position) {
    MIX_Point3D CastPosition = {Position.X, Position.Y, Position.Z};
    MIX_SetTrack3DPosition(Game::Sound::SDLMixer::DefaultTrack, &CastPosition);
}

#else
#warning \
    "Included 'SDLMixer_Sound.hpp' despite it not being supported on the targetted platform."
#endif