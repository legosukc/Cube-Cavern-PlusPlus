#include "ConsoleVars.hpp"

#include <iostream>
#include <map>

#include <SDL3/SDL_stdinc.h>

#include "FunctionHeaders/Math.hpp"

namespace Game::ConsoleVars {

    void BaseCVar::RaiseRangeWarning(const char* CVarClassName) const {
        std::clog << "Range error while parsing argument to " << CVarClassName
                  << ": '" << this->Name << "'" << std::endl;
    }

    void RegisterNewCVar(BaseCVar* NewCVar) {
        Game::ConsoleVars::CVars[NewCVar->Name] = NewCVar;
    }
}

namespace Game::ConsoleVars::CVarClasses {

    void FloatCVar::SetWithString(const char* NewValue) {
        errno = 0;
        const float Value = std::strtof(NewValue, NULL);

        if (errno == ERANGE) {
            this->RaiseRangeWarning("FloatCVar");
            return;
        }

        *static_cast<float*>(this->ReferenceVar) = Value;
    }

    void DoubleCVar::SetWithString(const char* NewValue) {
        errno = 0;
        const double Value = std::strtod(NewValue, NULL);

        if (errno == ERANGE) {
            this->RaiseRangeWarning("DoubleCVar");
            return;
        }

        *static_cast<double*>(this->ReferenceVar) = Value;
    }

    template <typename IntType>
    void IntCVar<IntType>::SetWithString(const char* NewValue) {
        errno = 0;

        constexpr long long IntMinimumValue =
            ((2 ^ sizeof(IntType) * 8) / -2) + 1;

        IntType Value;
        if CONSTEXPR_IF (sizeof(IntType) <= sizeof(long)) {
            Value = static_cast<IntType>(
                Math::Clamp<long>(std::strtol(NewValue, NULL, 10),
                                  ((2 ^ sizeof(IntType) * 8) / -2) + 1,
                                  ((2 ^ sizeof(IntType) * 8) / 2) + 1));
        } else {
            Value = static_cast<IntType>(
                Math::Clamp<long long>(std::strtoll(NewValue, NULL, 10),
                                       ((2 ^ sizeof(IntType) * 8) / -2) + 1,
                                       ((2 ^ sizeof(IntType) * 8) / 2) + 1));
        }

        if (errno == ERANGE) {
            this->RaiseRangeWarning("FloatCVar");
            return;
        }

        *static_cast<Sint8*>(this->ReferenceVar) =
            static_cast<Sint8>(Math::Clamp<long>(Value, INT8_MIN, INT8_MAX));
    }

    template <typename UintType>
    void UintCVar<UintType>::SetWithString(const char* NewValue) {}

    void BoolCVar::SetWithString(const char* NewValue) {
        int i = 1;
        switch (NewValue[0]) {
            case 't':

                while ("true"[i] != '\0') {
                    if ("true"[i] != NewValue[i]) {
                        return;
                    }
                }

                *static_cast<bool*>(this->ReferenceVar) = true;
                break;

            case 'f':

                while ("false"[i] != '\0') {
                    if ("false"[i] != NewValue[i]) {
                        return;
                    }
                }

                *static_cast<bool*>(this->ReferenceVar) = false;
                break;
        }
    }

    void StringCVar::SetWithString(const char* NewValue) {
        const char* Index = NewValue;
        while (true) {
            if (*Index == ' ') {
            }

            ++Index;
        }
    }
}