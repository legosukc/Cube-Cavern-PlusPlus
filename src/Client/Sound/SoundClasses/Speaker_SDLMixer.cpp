#include "Speaker_SDLMixer.hpp"

#include <SDL3/SDL_stdinc.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "../../../MathClasses/Vector3.hpp"

#include "../SoundAPIHandlers/SDLMixer_Sound.hpp"

namespace Game::Sound::Classes {

    Speaker_SDLMixer::~Speaker_SDLMixer() {
        MIX_DestroyAudio(this->AudioBuffer);
    }

    void Speaker_SDLMixer::Play() {
        MIX_PlayAudio(Game::Sound::SDLMixer::Mixer, this->AudioBuffer);
    }

    void Speaker_SDLMixer::Pause() {
        MIX_PauseTrack(Game::Sound::SDLMixer::DefaultTrack);
    }

    void Speaker_SDLMixer::Stop() {
        MIX_StopTrack(Game::Sound::SDLMixer::DefaultTrack, 0);
    }

    void Speaker_SDLMixer::SetPosition(const Math::Vector3& Position) {}
}