#pragma once

#include <iostream>


#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

#include <SDL3_mixer/SDL_mixer.h>

#include "../FunctionHeaders/Exceptions.hpp"

#include "../MathClasses/Vector3.hpp"


namespace Game::Sound {

	enum class AudioBackendEnum : Uint8 {
		OpenAL,
		SDL_Mixer,
		Cubeb,
		NoAudio,
	};

	namespace NoAudioClasses {
		class NoAudioSource;
		class NoAudioBuffer;
		struct NoAudioListener;
	}

	bool AudioEnabled = true;

	ALCdevice* Device = NULL;
	ALCcontext* Context = NULL;

	enum class Init_ErrorCodes : Uint8 {
		Success,
		OpenAudioDeviceError,
		CreateContextError,
		SetCurrentContextError
	};

	inline Init_ErrorCodes Init();
	inline void Destroy();

	char* DecodeAudioFile(const char* Path, size_t* AudioSize);

	struct Listener_Struct {

		virtual void SetPosition(const Math::Vector3& Position);
		virtual void SetVelocity(const Math::Vector3& Velocity);
		virtual void SetOrientation(const Math::Vector3& Look, const Math::Vector3& Up);
	};
	Listener_Struct Listener;

	class ALObjectBase {
	protected:
		ALuint ALName = 0;
	};

	class AudioChunk {

		char* Chunk = NULL;
		size_t ChunkSize = 0;

	public:
		constexpr AudioChunk() = default;

		// Loads the entire audio stream from the file at 'AudioPath'.
		inline AudioChunk(const char* AudioPath) {
			this->Chunk = Game::Sound::DecodeAudioFile(AudioPath, &this->ChunkSize);
		}

		~AudioChunk() {

			if (this->Chunk == NULL) {
				return;
			}
			delete[] this->Chunk;
			this->Chunk = NULL;
		}
	};

	class Source;
	class Buffer;

	Sound::Source CreateSource();
	Sound::Buffer CreateBuffer();

	void CreateSources(ALsizei Count, Sound::Source* Sources);
	void CreateBuffers(ALsizei Count, Sound::Buffer* Buffers);
}


char* Game::Sound::DecodeAudioFile(const char* Path, size_t* AudioSize) {
#warning "remember to change this function when SDL3_mixer decides to tell us the decoded size of an audio. what a shit library."

	int DecodeInfo;
	char* PCMData;
	SDL_AudioSpec AudioSpec;
	MIX_AudioDecoder* AudioDecoder;
	SDL_IOStream* AudioFile;

	AudioFile = SDL_IOFromFile(Path, "rb");
	if (AudioFile == NULL) {
		return NULL;
	}

	*AudioSize = static_cast<size_t>(SDL_GetIOSize(AudioFile));

	AudioDecoder = MIX_CreateAudioDecoder_IO(AudioFile, true, 0);

	AudioSpec.format = SDL_AudioFormat::SDL_AUDIO_F32;
	AudioSpec.channels = 1;
	AudioSpec.freq = 441000;
	
	while (true) {

		*AudioSize *= 2;
		PCMData = new char[*AudioSize];

		DecodeInfo = MIX_DecodeAudio(AudioDecoder, PCMData, *AudioSize, &AudioSpec);
		
		if (DecodeInfo == -1) {
			delete[] PCMData;
			MIX_DestroyAudioDecoder(AudioDecoder);
			Exceptions::ThrowExceptionExpression<Exceptions::IOError, const char*, const char*>("Unrecoverable error while decoding audio: ", Path);
		} else if (DecodeInfo == 0) {
			break;
		}

		delete[] PCMData;
	}

	MIX_DestroyAudioDecoder(AudioDecoder);

	return PCMData;
}




class Game::Sound::Source : public Game::Sound::ALObjectBase {
public:
	inline Source() = default;
	virtual ~Source();

	friend Source Game::Sound::CreateSource();
	//friend void Sound::CreateSources(ALsizei Count, Sound::Source* Sources);
};


class Game::Sound::Buffer : public Game::Sound::ALObjectBase {
public:
	inline Buffer() = default;
	virtual ~Buffer();

	virtual void UploadSoundData(const void* SoundData, size_t SoundSize, ALenum SoundFormat, ALsizei SampleRate = 441000);
	virtual bool LoadFile(const char* Path, ALenum Format = AL_FORMAT_STEREO_FLOAT32);

	friend Buffer Game::Sound::CreateBuffer();
	//friend void Sound::CreateBuffers(ALsizei Count, Sound::Buffer* Buffers);
};



struct Game::Sound::NoAudioClasses::NoAudioListener : Game::Sound::Listener_Struct {

	virtual void SetPosition(const Math::Vector3& Position) override {}
	virtual void SetVelocity(const Math::Vector3& Velocity) override {}
	virtual void SetOrientation(const Math::Vector3& Look, const Math::Vector3& Up) override {}
};


class Game::Sound::NoAudioClasses::NoAudioSource : public Game::Sound::Source {
	inline NoAudioSource() = default;
	virtual ~NoAudioSource() override {}

	friend Game::Sound::Source Game::Sound::CreateSource();
	friend void Game::Sound::CreateSources(ALsizei Count, Game::Sound::Source* Sources);
};

class Game::Sound::NoAudioClasses::NoAudioBuffer : public Game::Sound::Buffer {
	inline NoAudioBuffer() = default;
	virtual ~NoAudioBuffer() override {}

	friend Game::Sound::Buffer Game::Sound::CreateBuffer();
	friend void Game::Sound::CreateBuffers(ALsizei Count, Sound::Buffer* Buffers);
};



Game::Sound::Source Game::Sound::CreateSource() {

	if (!Game::Sound::AudioEnabled) {
		return Game::Sound::NoAudioClasses::NoAudioSource();
	}

	Game::Sound::Source NewSource;
	alGenSources(1, &NewSource.ALName);
	return NewSource;
}

void Game::Sound::CreateSources(ALsizei Count, Sound::Source* Sources) {

	if (!Game::Sound::AudioEnabled) {
		for (; Count > 0; --Count) {
			Sources[Count] = Game::Sound::NoAudioClasses::NoAudioSource();
		}
		return;
	}

	alGenSources(Count, reinterpret_cast<ALuint*>(Sources));
}


Game::Sound::Buffer Game::Sound::CreateBuffer() {

	if (!Game::Sound::AudioEnabled) {
		return Game::Sound::NoAudioClasses::NoAudioBuffer();
	}

	Game::Sound::Buffer NewBuffer;
	alGenBuffers(1, &NewBuffer.ALName);
	return NewBuffer;
}

void Game::Sound::CreateBuffers(ALsizei Count, Game::Sound::Buffer* Buffers) {

	if (!Game::Sound::AudioEnabled) {
		for (; Count > 0; --Count) {
			Buffers[Count] = Game::Sound::NoAudioClasses::NoAudioBuffer();
		}
		return;
	}

	alGenBuffers(Count, reinterpret_cast<ALuint*>(Buffers));
}


void Game::Sound::Listener_Struct::SetPosition(const Math::Vector3& Position) {
	alListenerfv(AL_POSITION, reinterpret_cast<const ALfloat*>(&Position));
}

void Game::Sound::Listener_Struct::SetVelocity(const Math::Vector3& Velocity) {
	alListenerfv(AL_VELOCITY, reinterpret_cast<const ALfloat*>(&Velocity));
}

void Game::Sound::Listener_Struct::SetOrientation(const Math::Vector3& Look, const Math::Vector3& Up) {
	const ALfloat OrientationBuffer[6] = { Look.X, Look.Y, Look.Z, Up.X, Up.Y, Up.Z };
	alListenerfv(AL_ORIENTATION, OrientationBuffer);
}



Game::Sound::Buffer::~Buffer() {
	alDeleteBuffers(1, &this->ALName);
}


void Game::Sound::Buffer::UploadSoundData(const void* SoundData, size_t SoundSize, ALenum SoundFormat, ALsizei SampleRate) {
	alBufferData(this->ALName, SoundFormat, SoundData, SoundSize, SampleRate);
}

bool Game::Sound::Buffer::LoadFile(const char* Path, ALenum Format) {

	size_t DecodedSize;
	void* Audio = Game::Sound::DecodeAudioFile(Path, &DecodedSize);

	unlikely_branch
	if (Audio == NULL) {
		return false;
	}
	this->UploadSoundData(Audio, DecodedSize, Format);

	return true;
}


Game::Sound::Source::~Source() {
	alDeleteSources(1, &this->ALName);
}





Game::Sound::Init_ErrorCodes Game::Sound::Init() {

	std::cout << "Initalizing OpenAL." << std::endl;
	const Uint64 StartNS = SDL_GetTicksNS();

	Game::Sound::Init_ErrorCodes ErrorCode;

	//alcGetString(NULL, ALC_DEVICE_SPECIFIER);
	Game::Sound::Device = alcOpenDevice(NULL); // open default device

	if (Game::Sound::Device == NULL) {

		std::clog << "Failed to open OpenAL audio device";
		ErrorCode = Game::Sound::Init_ErrorCodes::OpenAudioDeviceError;
		goto _Failure;
	}

	Game::Sound::Context = alcCreateContext(Game::Sound::Device, NULL);

	if (Game::Sound::Context == NULL) {

		std::clog << "Failed to create OpenAL context";
		ErrorCode = Game::Sound::Init_ErrorCodes::CreateContextError;
		goto _Failure;
	}

	if (!alcMakeContextCurrent(Game::Sound::Context)) {

		std::clog << "Failed to set the current OpenAL context";
		ErrorCode = Game::Sound::Init_ErrorCodes::SetCurrentContextError;
		goto _Failure;
	}

	std::cout << "Initialized OpenAL, took " << static_cast<double>(SDL_GetTicks() - StartNS) / 1000.0 << " milliseconds." << std::endl;

	return Game::Sound::Init_ErrorCodes::Success;

_Failure:
	Game::Sound::AudioEnabled = false;
	Game::Sound::Destroy();
	std::clog << ".\nFailed to initalize OpenAL, audio has been disabled. Took " << static_cast<double>(SDL_GetTicks() - StartNS) / 1000.0 << " milliseconds." << std::endl;

	return ErrorCode;
}

void Game::Sound::Destroy() {

	if (Game::Sound::Context != NULL) {
		alcDestroyContext(Game::Sound::Context);
		Game::Sound::Context = NULL;
	}

	if (Game::Sound::Device == NULL) {
		return;
	}

	if (!alcCloseDevice(Game::Sound::Device)) {
		std::clog << "Failed to close OpenAL audio device." << std::endl;
	} else {
		Game::Sound::Device = NULL;
	}
}