#pragma once

#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include "../MathClasses/Vector3.hpp"


#include "SoundClasses/Listener.hpp"

namespace Game::Sound::OpenAL {

	ALCdevice* Device = NULL;
	ALCcontext* Context = NULL;

	struct Listener_OpenAL : Game::Sound::Classes::ListenerStruct {
		virtual void SetPosition(const Math::Vector3& Position) override {
			alListenerfv(AL_POSITION, reinterpret_cast<const ALfloat*>(&Position));
		}

		virtual void SetVelocity(const Math::Vector3& Velocity) override {
			alListenerfv(AL_VELOCITY, reinterpret_cast<const ALfloat*>(&Velocity));
		}

		virtual void SetOrientation(const Math::Vector3& Look, const Math::Vector3& Up) override {
			Math::Vector3 Orientation[2] = { Look, Up };
			alListenerfv(AL_ORIENTATION, reinterpret_cast<const ALfloat*>(&Orientation));
		}
	};

	inline bool Init();
	inline void Destroy();
}

#include "SoundClasses/AudioSource_OpenAL.hpp"
#include "SoundClasses/Speaker_OpenAL.hpp"


bool Game::Sound::OpenAL::Init() {

	//alcGetString(NULL, ALC_DEVICE_SPECIFIER);
	Game::Sound::OpenAL::Device = alcOpenDevice(NULL); // open default device
	if (Game::Sound::OpenAL::Device == NULL) {
		std::clog << "Failed to open OpenAL audio device";
		return false;
	}

	Game::Sound::OpenAL::Context = alcCreateContext(Game::Sound::OpenAL::Device, NULL);
	if (Game::Sound::OpenAL::Context == NULL) {
		std::clog << "Failed to create OpenAL context";
		return false;
	}

	if (!alcMakeContextCurrent(Game::Sound::OpenAL::Context)) {
		std::clog << "Failed to set the current OpenAL context";
		return false;
	}


	Game::Sound::Listener = Game::Sound::OpenAL::Listener_OpenAL();

	Game::Sound::CreateAudioSource = Game::Sound::Classes::AudioSource_OpenAL::Create;
	Game::Sound::CreateAudioSourceBulk = Game::Sound::Classes::AudioSource_OpenAL::CreateBulk;

	Game::Sound::CreateSpeaker = Game::Sound::Classes::Speaker_OpenAL::Create;
	Game::Sound::CreateSpeakerBulk = Game::Sound::Classes::Speaker_OpenAL::CreateBulk;

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