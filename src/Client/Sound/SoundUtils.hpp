#ifndef SOUND_UTILS_H
#define SOUND_UTILS_H 1

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

// #include <SDL3_mixer/SDL_mixer.h>

//#include "../../../include/stb_vorbis.h"

#include "../../FunctionHeaders/Exceptions.hpp"

namespace Game::Sound::Utils {

    class RawAudioBuffer {
        void(*AudioChannelDataDeallocater)(void*);
    public:
        ~RawAudioBuffer() {
            this->AudioChannelDataDeallocater(this->AudioChannelData);
        }
        void** AudioChannelData;
        Uint32 AudioSize;
        Uint32 SampleRate;
        Uint8 Channels;

        friend RawAudioBuffer DecodeAudioFile(const char* Path,
                                              size_t* AudioSize);
    };

    // Free returned PCM data using "std::free".
    RawAudioBuffer DecodeAudioFile(const char* Path, size_t* AudioSize);
}

Game::Sound::Utils::RawAudioBuffer Game::Sound::Utils::DecodeAudioFile(
    const char* Path,
    size_t* AudioSize) {
    // constexpr size_t DecodeIncrement = sizeof(float) * 441000;

    if (!SDL_GetPathInfo(Path, NULL)) {
        Exceptions::ThrowException<Exceptions::IOError>(StringHelper::Combine(
            "DecodeAudioFile error: '", Path, "' doesn't exist!"));
    }

    Game::Sound::Utils::RawAudioBuffer Result;

    const char* FileExtension;
    for (FileExtension = Path + strlen(Path); *FileExtension != '.';
         --FileExtension)
        ;

    if (strcmp(FileExtension, "ogg") == 0) {
        /*int sample_rate, channels;
        const int result = stb_vorbis_decode_filename(
            Path, &channels, &sample_rate, (short**)&Result.AudioChannelData);
        std::cout << "ogg file size (?): " << result << std::endl;
        Result.SampleRate = sample_rate;
        Result.Channels = channels;
        Result.AudioSize = sample_rate * channels;*/

    } else if (strcmp(FileExtension, "wav") == 0) {
        SDL_AudioSpec Spec;

        if (!SDL_LoadWAV(Path, &Spec, (Uint8**)Result.AudioChannelData,
                         &Result.AudioSize)) {
            const std::string ErrorMessage =
                StringHelper::Combine("Failed to load wav file", Path);
            std::cerr << ErrorMessage << std::endl;
            Exceptions::ThrowException<Exceptions::IOError>(ErrorMessage);
        }
        Result.AudioChannelDataDeallocater = SDL_free;
    } else {
        const std::string ErrorMessage =
            StringHelper::Combine("DecodeAudioFile error: '", Path,
                                  "' doesn't have a supported file "
                                  "extension.\nSupported extensions are: "
                                  "ogg, wav");
        std::cerr << ErrorMessage << std::endl;
        Exceptions::ThrowException<Exceptions::IOError>(ErrorMessage);
    }
    return Result;

    /*
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
        DecodedNumBytes = MIX_DecodeAudio(AudioDecoder, DecodePtr,
                                          TotalDecodedSize, &AudioSpec);

        if (DecodedNumBytes == -1) {
            std::free(PCMData);
            MIX_DestroyAudioDecoder(AudioDecoder);
            Exceptions::ThrowExceptionExpression<Exceptions::IOError>(
                "Unrecoverable error while decoding audio: ", Path,
                " SDL_Error: ", SDL_GetError());
        } else if (DecodedNumBytes < DecodeIncrement) {
            TotalDecodedSize += DecodedNumBytes;
            std::cout << "fully decoded" << std::endl;
            break;
        }

        PCMData = static_cast<char*>(
            std::realloc(PCMData, TotalDecodedSize + DecodeIncrement));
        DecodePtr = PCMData + TotalDecodedSize;
        TotalDecodedSize += DecodeIncrement;
    }
    *AudioSize = TotalDecodedSize;

    MIX_DestroyAudioDecoder(AudioDecoder);

    return PCMData;*/
}

#endif