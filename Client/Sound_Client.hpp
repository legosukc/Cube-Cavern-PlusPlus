#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_mixer.h>

#include "../Vector3.hpp"


namespace Game::Sound {

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

	Mix_Chunk* DecodeAudioFile(const char* Path);

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

		Mix_Chunk* Chunk = NULL;

	public:
		constexpr AudioChunk() = default;

		// Loads the entire audio stream from the file at 'AudioPath'.
		inline AudioChunk(const char* AudioPath) {
			this->Chunk = Game::Sound::DecodeAudioFile(AudioPath);
		}

		~AudioChunk() {

			if (this->Chunk == NULL) {
				return;
			}
			Mix_FreeChunk(this->Chunk);
		}
	};

	class Source;
	class Buffer;

	Sound::Source CreateSource();
	Sound::Buffer CreateBuffer();

	void CreateSources(ALsizei Count, Sound::Source* Sources);
	void CreateBuffers(ALsizei Count, Sound::Buffer* Buffers);
}


Mix_Chunk* Game::Sound::DecodeAudioFile(const char* Path) {

	Mix_Chunk* const Chunk = Mix_LoadWAV(Path);

	unlikely_branch
	if (Chunk == NULL) {
		std::clog << "Failed to find audio: " << Path << "\nMix_Error: " << Mix_GetError() << std::endl;
	}
	return Chunk;
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

	Mix_Chunk* Chunk = Game::Sound::DecodeAudioFile(Path);

	unlikely_branch
	if (Chunk == NULL) {
		return false;
	}
	this->UploadSoundData(Chunk->abuf, Chunk->alen, Format);

	return true;
}


Game::Sound::Source::~Source() {
	alDeleteSources(1, &this->ALName);
}





Game::Sound::Init_ErrorCodes Game::Sound::Init() {

	std::cout << "Initalizing OpenAL.\n";
	const auto Start = std::chrono::high_resolution_clock::now();

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

	std::cout << "Initialized OpenAL, took " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - Start).count() << " milliseconds." << std::endl;

	return Game::Sound::Init_ErrorCodes::Success;

_Failure:
	Game::Sound::AudioEnabled = false;
	Game::Sound::Destroy();
	std::clog << ".\nFailed to initalize OpenAL, audio has been disabled. Took " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - Start).count() << " milliseconds." << std::endl;

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