#include "Speaker.hpp"

#include "AudioSource.hpp"

#include "../../../MathClasses/Vector3.hpp"

namespace Game::Sound::Classes {

    Speaker::~Speaker() = default;

    void Speaker::SetAudioSource(Game::Sound::Classes::AudioSource*) {}

    Game::Sound::Classes::AudioSource* Speaker::GetAudioSource() const {
        return new Game::Sound::Classes::AudioSource();
    }

    void Speaker::Play() {}
    void Speaker::Pause() {}
    void Speaker::Stop() {}

    void Speaker::SetPosition(const Math::Vector3& Position) {}
    void Speaker::SetVelocity(const Math::Vector3& Velocity) {}
    void Speaker::SetOrientation(const Math::Vector3& Look,
                                 const Math::Vector3& Up) {}
}

namespace Game::Sound {
    Game::Sound::Classes::Speaker* (*CreateSpeaker)() = NULL;
    void (*CreateSpeakerBulk)(size_t Count,
                              Game::Sound::Classes::Speaker* Speakers[]) = NULL;
}