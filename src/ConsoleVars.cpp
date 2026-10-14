#include "ConsoleVars.hpp"

#include <iostream>
#include <map>
#include <string>

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

        this->CVarValue = Value;
    }

    void FloatCVar::SetValue(const void* PtrToCVarType) {
        this->CVarValue = *static_cast<const float*>(PtrToCVarType);
    }

    std::string FloatCVar::GetAsString() {
        return std::to_string(this->CVarValue);
    }

    void FloatCVar::GetValue(void* DestinationForCVarType) {
        *static_cast<float*>(DestinationForCVarType) = this->CVarValue;
    }

    void DoubleCVar::SetWithString(const char* NewValue) {
        errno = 0;
        const double Value = std::strtod(NewValue, NULL);

        if (errno == ERANGE) {
            this->RaiseRangeWarning("DoubleCVar");
            return;
        }

        this->CVarValue = Value;
    }

    void DoubleCVar::SetValue(const void* PtrToCVarType) {
        this->CVarValue = *static_cast<const double*>(PtrToCVarType);
    }

    std::string DoubleCVar::GetAsString() {
        return std::to_string(this->CVarValue);
    }

    void DoubleCVar::GetValue(void* DestinationForCVarType) {
        *static_cast<double*>(DestinationForCVarType) = this->CVarValue;
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

        this->CVarValue =
            static_cast<Sint8>(Math::Clamp<long>(Value, INT8_MIN, INT8_MAX));
    }

    template <typename IntType>
    void IntCVar<IntType>::SetValue(const void* PtrToCVarType) {
        this->CVarValue = *static_cast<const IntType*>(PtrToCVarType);
    }

    template <typename IntType>
    std::string IntCVar<IntType>::GetAsString() {
        return std::to_string(this->CVarValue);
    }

    template <typename IntType>
    void IntCVar<IntType>::GetValue(void* DestinationForCVarType) {
        *static_cast<IntType*>(DestinationForCVarType) = this->CVarValue;
    }

    template class IntCVar<Sint8>;
    template class IntCVar<Sint16>;
    template class IntCVar<Sint32>;
    template class IntCVar<Sint64>;

    template <typename UintType>
    void UintCVar<UintType>::SetWithString(const char* NewValue) {
        // TODO: implement
    }

    template <typename UintType>
    void UintCVar<UintType>::SetValue(const void* PtrToCVarType) {
        this->CVarValue = *static_cast<const UintType*>(PtrToCVarType);
    }

    template <typename UintType>
    std::string UintCVar<UintType>::GetAsString() {
        return std::to_string(this->CVarValue);
    }

    template<typename UintType>
    void UintCVar<UintType>::GetValue(void* DestinationForCVarType) {
        *static_cast<UintType*>(DestinationForCVarType) = this->CVarValue;
    }

    template class UintCVar<Uint8>;
    template class UintCVar<Uint16>;
    template class UintCVar<Uint32>;
    template class UintCVar<Uint64>;

    void BoolCVar::SetWithString(const char* NewValue) {
        int i = 1;
        switch (NewValue[0]) {
            case 't':

                while ("true"[i] != '\0') {
                    if ("true"[i] != NewValue[i]) {
                        return;
                    }
                }

                this->CVarValue = true;
                break;

            case 'f':

                while ("false"[i] != '\0') {
                    if ("false"[i] != NewValue[i]) {
                        return;
                    }
                }

                this->CVarValue = false;
                break;
        }
    }

    void BoolCVar::SetValue(const void* PtrToCVarType) {
        this->CVarValue = *static_cast<const bool*>(PtrToCVarType);
    }

    std::string BoolCVar::GetAsString() {
        return std::to_string(this->CVarValue);
    }

    void BoolCVar::GetValue(void* DestinationForCVarType) {
        *static_cast<bool*>(DestinationForCVarType) = this->CVarValue;
    }

    void StringCVar::SetWithString(const char* NewValue) {
        this->SetValue(NewValue);
    }

    void StringCVar::SetValue(const void* PtrToCVarType) {
        const size_t NewValueLen =
            strlen(static_cast<const char*>(PtrToCVarType)) + 1;
        delete[] this->CVarValue;
        this->CVarValue = new char[NewValueLen];
        memcpy(this->CVarValue, static_cast<const char*>(PtrToCVarType),
               NewValueLen);
    }

    std::string StringCVar::GetAsString() {
        return this->CVarValue;
    }

    void StringCVar::GetValue(void* DestinationForCVarType) {
        *static_cast<char**>(DestinationForCVarType) = this->CVarValue;
    }
}