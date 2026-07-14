#pragma once

#include <iostream>

#include <AL/al.h>
#include <AL/alext.h>

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

#include <SDL3_mixer/SDL_mixer.h>

#include "../FunctionHeaders/Exceptions.hpp"

#include "../MathClasses/Vector3.hpp"

#include "SoundClasses/Listener.hpp"
#include "SoundClasses/AudioSource.hpp"
#include "SoundClasses/Speaker.hpp"


namespace Game::Sound {

	Classes::ListenerStruct Listener;

	enum class AudioBackendEnum : Uint8 {
		OpenAL,
		SDL_Mixer,
		Cubeb,
		NoAudio,
	} AudioBackend;

	bool AudioEnabled = false;

	inline bool Init();
	inline void Destroy();

	// Free returned PCM data using "std::free".
	char* DecodeAudioFile(const char* Path, size_t* AudioSize);

	using Classes::AudioSource;
	using Classes::Speaker;

	AudioSource* (*CreateAudioSource)();
	void (*CreateAudioSourceBulk)(size_t Count, AudioSource* Sources[]);

	Speaker* (*CreateSpeaker)();
	void (*CreateSpeakerBulk)(size_t Count, Speaker* Speakers[]);
}



char* Game::Sound::DecodeAudioFile(const char* Path, size_t* AudioSize) {
	
	constexpr size_t DecodeIncrement = sizeof(float) * 441000;

	int DecodedNumBytes;
	char* PCMData;
	SDL_AudioSpec AudioSpec;
	MIX_AudioDecoder* AudioDecoder;
	SDL_IOStream* AudioFile;

	AudioFile = SDL_IOFromFile(Path, "rb");
	if (AudioFile == NULL) {
		return NULL;
	}

	size_t TotalDecodedSize = static_cast<size_t>(SDL_GetIOSize(AudioFile));

	AudioDecoder = MIX_CreateAudioDecoder_IO(AudioFile, true, 0);

	AudioSpec.format = SDL_AudioFormat::SDL_AUDIO_F32;
	AudioSpec.channels = 1;
	AudioSpec.freq = 441000;

	PCMData = static_cast<char*>(std::malloc(DecodeIncrement));
	char* DecodePtr = PCMData;
	while (true) {

		DecodedNumBytes = MIX_DecodeAudio(AudioDecoder, DecodePtr, TotalDecodedSize, &AudioSpec);

		if (DecodedNumBytes == -1) {
			std::free(PCMData);
			MIX_DestroyAudioDecoder(AudioDecoder);
			Exceptions::ThrowExceptionExpression<Exceptions::IOError>("Unrecoverable error while decoding audio: ", Path, " SDL_Error: ", SDL_GetError());
		} else if (DecodedNumBytes < DecodeIncrement) {
			TotalDecodedSize += DecodedNumBytes;
			std::cout << "fully decoded" << std::endl;
			break;
		}

		PCMData = static_cast<char*>(std::realloc(PCMData, TotalDecodedSize + DecodeIncrement));
		DecodePtr = PCMData + TotalDecodedSize;
		TotalDecodedSize += DecodeIncrement;
	}
	*AudioSize = TotalDecodedSize;

	MIX_DestroyAudioDecoder(AudioDecoder);

	return PCMData;
}


#include "OpenAL_Sound.hpp"
#include "SDLMixer_Sound.hpp"


bool Game::Sound::Init() {

	std::cout << "Initalizing Sound." << std::endl;
	const Uint64 StartMS = SDL_GetTicks();

	if (!MIX_Init()) {
		Exceptions::ThrowSDLError("Failed to initialize SDL_mixer (needed for decoding audio, even if you selected another SoundAPI.)");
	}

	Game::Sound::AudioBackend = Game::Sound::AudioBackendEnum::OpenAL;

	bool Success;
	switch (Game::Sound::AudioBackend) {
	case AudioBackendEnum::OpenAL:
		Success = Game::Sound::OpenAL::Init();
		break;
	case AudioBackendEnum::SDL_Mixer:
		Success = Game::Sound::SDLMixer::Init();
		break;
	}

	Game::Sound::AudioEnabled = Success;
	if (Success) {
		std::cout << "Initialized Sound";
	} else {
		Game::Sound::Destroy();
		std::clog << ".\nFailed to initalize Sound, audio has been disabled";
	}
	std::clog << ", took " << SDL_GetTicks() - StartMS << " milliseconds." << std::endl;

	return Success;
}

void Game::Sound::Destroy() {

	switch (Game::Sound::AudioBackend) {
	case AudioBackendEnum::OpenAL:
		Game::Sound::OpenAL::Destroy();
		break;
	case AudioBackendEnum::SDL_Mixer:
		Game::Sound::SDLMixer::Destroy();
		break;
	}
}