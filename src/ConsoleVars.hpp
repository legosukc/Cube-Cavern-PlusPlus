#pragma once

#include <map>

#include <SDL3/SDL_stdinc.h>

namespace Game::ConsoleVars {

    namespace ReferenceVars {}

    class BaseCVar {
       protected:
        void RaiseRangeWarning(const char* CVarClassName) const;

       public:
        BaseCVar() noexcept;
        const char* const Name;
        void* ReferenceVar;

        virtual void SetWithString(const char* NewValue) = 0;
        virtual void SetValue(const void* PtrToCVarType) = 0;
        virtual void GetValue(void* DestinationForCVarType) = 0;
        virtual const char* GetAsString() = 0;
    };

    inline std::map<const char*, BaseCVar*> CVars;

    void RegisterNewCVar(BaseCVar* NewCVar);
}

namespace Game::ConsoleVars::CVarClasses {

    struct FloatCVar : BaseCVar {
        virtual void SetWithString(const char* NewValue) override;
        virtual void SetValue(const void* PtrToCVarType) override;
        virtual void GetValue(void* DestinationForCVarType) override;
        virtual const char* GetAsString() override;

        using VarType = float;
    };

    struct DoubleCVar : BaseCVar {
        virtual void SetWithString(const char* NewValue) override;
        virtual void SetValue(const void* PtrToCVarType) override;
        virtual void GetValue(void* DestinationForCVarType) override;
        virtual const char* GetAsString() override;

        using VarType = double;
    };

    template <typename IntType>
    struct IntCVar : BaseCVar {
        virtual void SetWithString(const char* NewValue) override;
        virtual void SetValue(const void* PtrToCVarType) override;
        virtual void GetValue(void* DestinationForCVarType) override;
        virtual const char* GetAsString() override;

        using VarType = IntType;
    };

    template <typename UintType>
    struct UintCVar : BaseCVar {
        virtual void SetWithString(const char* NewValue) override;
        virtual void SetValue(const void* PtrToCVarType) override;
        virtual void GetValue(void* DestinationForCVarType) override;
        virtual const char* GetAsString() override;

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

    struct BoolCVar : BaseCVar {
        virtual void SetWithString(const char* NewValue) override;
        virtual void SetValue(const void* PtrToCVarType) override;
        virtual void GetValue(void* DestinationForCVarType) override;
        virtual const char* GetAsString() override;

        using VarType = bool;
    };

    struct StringCVar : BaseCVar {
        virtual void SetWithString(const char* NewValue) override;
        virtual void SetValue(const void* PtrToCVarType) override;
        virtual void GetValue(void* DestinationForCVarType) override;
        virtual const char* GetAsString() override;

        using VarType = const char*;
    };
}