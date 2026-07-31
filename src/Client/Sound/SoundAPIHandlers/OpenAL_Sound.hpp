#ifndef SOUND_OPENAL_H
#define SOUND_OPENAL_H

#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include "../../../MathClasses/Vector3.hpp"

//#include "../Sound_Client.hpp"

#include "../SoundClasses/Listener.hpp"
#include "../SoundClasses/AudioSource.hpp"

#include "../SoundClasses/AudioSource_OpenAL.hpp"
#include "../SoundClasses/Speaker_OpenAL.hpp"

namespace Game::Sound::OpenAL {

    ALCdevice* Device = NULL;
    ALCcontext* Context = NULL;

    struct Listener_OpenAL : Game::Sound::Classes::ListenerStruct {
        virtual void SetPosition(const Math::Vector3& Position) override;
        virtual void SetVelocity(const Math::Vector3& Velocity) override;
        virtual void SetOrientation(const Math::Vector3& Look,
                                    const Math::Vector3& Up) override;
    };

	inline bool Init();
	inline void Destroy();
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

bool Game::Sound::OpenAL::Init() {
    // alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    Game::Sound::OpenAL::Device = alcOpenDevice(NULL);  // open default device
    if (Game::Sound::OpenAL::Device == NULL) {
        std::clog << "Failed to open OpenAL audio device";
        return false;
    }

    Game::Sound::OpenAL::Context =
        alcCreateContext(Game::Sound::OpenAL::Device, NULL);
    if (Game::Sound::OpenAL::Context == NULL) {
        std::clog << "Failed to create OpenAL context";
        return false;
    }

    if (!alcMakeContextCurrent(Game::Sound::OpenAL::Context)) {
        std::clog << "Failed to set the current OpenAL context";
        return false;
    }

    Game::Sound::Listener = Game::Sound::OpenAL::Listener_OpenAL();

    Game::Sound::CreateAudioSource =
        Game::Sound::OpenAL::AudioSource::Create;
    Game::Sound::CreateAudioSourceBulk =
        Game::Sound::OpenAL::AudioSource::CreateBulk;

    Game::Sound::CreateSpeaker = Game::Sound::OpenAL::Speaker::Create;
    Game::Sound::CreateSpeakerBulk = Game::Sound::OpenAL::Speaker::CreateBulk;

    return true;
}

void Game::Sound::OpenAL::Destroy() {
    using namespace Game::Sound::OpenAL;

    Game::Sound::Listener = Game::Sound::Classes::ListenerStruct();

    if (OpenAL::Context != NULL) {
        alcDestroyContext(OpenAL::Context);
        OpenAL::Context = NULL;
    }

    if (OpenAL::Device == NULL) {
        return;
    }

    if (!alcCloseDevice(OpenAL::Device)) {
        std::clog << "Failed to close OpenAL audio device." << std::endl;
    } else {
        OpenAL::Device = NULL;
    }
}

#endif