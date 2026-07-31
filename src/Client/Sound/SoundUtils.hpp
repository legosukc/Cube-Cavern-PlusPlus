#ifndef SOUND_UTILS_H
#define SOUND_UTILS_H

#include <iostream>
#include <cstddef>
#include <cstdlib>

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

#include <SDL3_mixer/SDL_mixer.h>

#include "../../FunctionHeaders/Exceptions.hpp"

namespace Game::Sound::Utils {

    // Free returned PCM data using "std::free".
    char* DecodeAudioFile(const char* Path, size_t* AudioSize);
}

char* Game::Sound::Utils::DecodeAudioFile(const char* Path, size_t* AudioSize) {
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

    return PCMData;
}

#endif