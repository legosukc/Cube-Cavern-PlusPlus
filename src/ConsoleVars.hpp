#pragma once

#include <map>
#include <string>

#include <SDL3/SDL_stdinc.h>

namespace Game::ConsoleVars {

    namespace ReferenceVars {}

    class BaseCVar {
       protected:
        void RaiseRangeWarning(const char* CVarClassName) const;

       public:
        BaseCVar() noexcept;
        const char* const Name;

        virtual void SetWithString(const char* NewValue) = 0;
        virtual void SetValue(const void* PtrToCVarType) = 0;
        virtual std::string GetAsString() = 0;
        virtual void GetValue(void* DestinationForCVarType) = 0;
    };

    inline std::map<const char*, BaseCVar*> CVars;

    void RegisterNewCVar(BaseCVar* NewCVar);
}

namespace Game::ConsoleVars::CVarClasses {

    class FloatCVar : BaseCVar {
		float CVarValue = 0.f;
       public:
        virtual void SetWithString(const char* NewValue) override;
        virtual void SetValue(const void* PtrToCVarType) override;
        virtual std::string GetAsString() override;
        virtual void GetValue(void* DestinationForCVarType) override;

        using VarType = float;
    };

    class DoubleCVar : BaseCVar {
		double CVarValue = 0.0;
       public:
        virtual void SetWithString(const char* NewValue) override;
        virtual void SetValue(const void* PtrToCVarType) override;
        virtual std::string GetAsString() override;
        virtual void GetValue(void* DestinationForCVarType) override;

        using VarType = double;
    };

    template <typename IntType>
    class IntCVar : BaseCVar {
		IntType CVarValue = static_cast<IntType>(0);
       public:
        virtual void SetWithString(const char* NewValue) override;
        virtual void SetValue(const void* PtrToCVarType) override;
        virtual std::string GetAsString() override;
        virtual void GetValue(void* DestinationForCVarType) override;

        using VarType = IntType;
    };

    template <typename UintType>
    class UintCVar : BaseCVar {
		UintType CVarValue = static_cast<UintType>(0);
       public:
        virtual void SetWithString(const char* NewValue) override;
        virtual void SetValue(const void* PtrToCVarType) override;
        virtual std::string GetAsString() override;
        virtual void GetValue(void* DestinationForCVarType) override;

        using VarType = UintType;
    };

    using Int8CVar = IntCVar<Sint8>;
    using Int16CVar = IntCVar<Sint16>;
    using Int32CVar = IntCVar<Sint32>;
    using Int64CVar = IntCVar<Sint64>;

    using Uint8CVar = UintCVar<Uint8>;
    using Uint16CVar = UintCVar<Uint16>;
    using Uint32CVar = UintCVar<Uint32>;
    using Uint64CVar = UintCVar<Uint64>;

    class BoolCVar : BaseCVar {
		bool CVarValue = false;
       public:
        virtual void SetWithString(const char* NewValue) override;
        virtual void SetValue(const void* PtrToCVarType) override;
        virtual std::string GetAsString() override;
        virtual void GetValue(void* DestinationForCVarType) override;

        using VarType = bool;
    };

    class StringCVar : BaseCVar {
		char* CVarValue = NULL;
       public:
        virtual void SetWithString(const char* NewValue) override;
        virtual void SetValue(const void* PtrToCVarType) override;
        virtual std::string GetAsString() override;
        virtual void GetValue(void* DestinationForCVarType) override;

        using VarType = const char*;
    };
}