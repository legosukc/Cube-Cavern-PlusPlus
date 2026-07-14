#pragma once

#include <AL/al.h>

#include <SDL3/SDL_stdinc.h>

#include "AudioSource.hpp"


namespace Game::Sound::Classes {

	class AudioSource_OpenAL : Game::Sound::Classes::AudioSource {
		ALuint ALBuffer;

		static Game::Sound::Classes::AudioSource* Create() {
			AudioSource_OpenAL* NewSpeaker = new AudioSource_OpenAL();
			alGenBuffers(1, &NewSpeaker->ALBuffer);
			return reinterpret_cast<Game::Sound::Classes::AudioSource*>(NewSpeaker);
		}

		static void CreateBulk(size_t Count, Game::Sound::Classes::AudioSource* AudioSources[]) {

			ALuint* ALBuffers = SDL_stack_alloc(ALuint, Count);
			alGenBuffers(Count, ALBuffers);

			for (size_t i = 0; i < Count; ++i) {
				AudioSources[i] = new AudioSource_OpenAL;
				reinterpret_cast<AudioSource_OpenAL*>(AudioSources[i])->ALBuffer = ALBuffers[i];
			}
			SDL_stack_free(ALBuffers);
		}

		friend bool Game::Sound::OpenAL::Init();
		
	public:
		virtual ~AudioSource_OpenAL() override {
			alDeleteBuffers(1, &this->ALBuffer);
		}

		virtual void UploadSoundData(const void* SoundData, size_t SoundSize, ALenum SoundFormat, ALsizei SampleRate = 441000) override {
			alBufferData(this->ALBuffer, SoundFormat, SoundData, SoundSize, SampleRate);
		}
		virtual bool LoadFile(const char* Path, ALenum Format = AL_FORMAT_STEREO_FLOAT32) override {

			size_t AudioSize;
			char* PCM = Game::Sound::DecodeAudioFile(Path, &AudioSize);
			if (PCM == NULL) {
				return false;
			}

			alBufferData(this->ALBuffer, Format, PCM, AudioSize, 441000);

			std::free(PCM);

			return true;
		}
	};
}