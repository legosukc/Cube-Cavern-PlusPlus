#pragma once

#ifndef SOUND_CLIENT_H
#define SOUND_CLIENT_H

#include <AL/al.h>
#include <AL/alext.h>

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

#include <SDL3_mixer/SDL_mixer.h>

#include "SoundClasses/Listener.hpp"
#include "SoundClasses/AudioSource.hpp"
#include "SoundClasses/Speaker.hpp"


namespace Game::Sound {

	extern Classes::ListenerStruct Listener;

	enum class AudioBackendEnum : Uint8 {
		OpenAL,
		SDL_Mixer,
		Cubeb,
		NoAudio,
	};
	extern AudioBackendEnum AudioBackend;

	extern bool AudioEnabled;

	inline bool Init();
	inline void Destroy();

	// Free returned PCM data using "std::free".
	char* DecodeAudioFile(const char* Path, size_t* AudioSize);

	using Classes::AudioSource;
	using Classes::Speaker;

	extern AudioSource* (*CreateAudioSource)();
	extern void (*CreateAudioSourceBulk)(size_t Count, AudioSource* Sources[]);

	extern Speaker* (*CreateSpeaker)();
	extern void (*CreateSpeakerBulk)(size_t Count, Speaker* Speakers[]);
}

#endif