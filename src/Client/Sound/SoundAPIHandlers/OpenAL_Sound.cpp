#include "OpenAL_Sound.hpp"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include "../../../MathClasses/Vector3.hpp"

namespace Game::Sound::OpenAL {
    ALCdevice* Device = NULL;
    ALCcontext* Context = NULL;
}

void Game::Sound::OpenAL::Listener_OpenAL::SetPosition(
    const Math::Vector3& Position) {
    alListenerfv(AL_POSITION, reinterpret_cast<const ALfloat*>(&Position));
}

void Game::Sound::OpenAL::Listener_OpenAL::SetVelocity(
    const Math::Vector3& Velocity) {
    alListenerfv(AL_VELOCITY, reinterpret_cast<const ALfloat*>(&Velocity));
}

void Game::Sound::OpenAL::Listener_OpenAL::SetOrientation(
    const Math::Vector3& Look,
    const Math::Vector3& Up) {
    Math::Vector3 Orientation[2] = {Look, Up};
    alListenerfv(AL_ORIENTATION,
                 reinterpret_cast<const ALfloat*>(&Orientation));
}