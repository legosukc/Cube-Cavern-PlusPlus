#pragma once

#include <map>

#include <SDL3/SDL_stdinc.h>


#include "Memory.hpp"

#include "FunctionHeaders/Math.hpp"



namespace Game::ConsoleVars {

	namespace ReferenceVars {}


	class BaseCVar {
	protected:
		void RaiseRangeWarning(const char* CVarClassName) const {
			std::clog << "Range error while parsing argument to " << CVarClassName << ": '" << this->Name << "'" << std::endl;
		}

	public:
		const char* Name;
		void* ReferenceVar;

		virtual void Set(const char* Argument) {}
		virtual const char* Get() {
			return NULL;
		}
	};


	std::map<const char*, BaseCVar> CVars;

	template<class CVarClass>
	CVarClass& RegisterNewCVar(const char* CVarName) {
		static_assert(std::is_base_of_v<BaseCVar, CVarClass>, "Template class argument 'CVarClass' isn't derived from class 'BaseCVar'.");

		CVarClass NewCVar;
		NewCVar.Name = CVarName;

		return static_cast<CVarClass&>(Game::ConsoleVars::CVars[CVarName] = NewCVar);
	}
}

namespace Game::ConsoleVars::CVarClasses {

	struct FloatCVar : BaseCVar {

		virtual void Set(const char* Argument) override {
			errno = 0;
			const float Value = std::strtof(Argument, NULL);

			if (errno == ERANGE) {
				this->RaiseRangeWarning("FloatCVar");
				return;
			}

			*static_cast<float*>(this->ReferenceVar) = Value;
		}

		using VarType = float;
	};

	struct DoubleCVar : BaseCVar {

		virtual void Set(const char* Argument) override {
			errno = 0;
			const double Value = std::strtod(Argument, NULL);

			if (errno == ERANGE) {
				this->RaiseRangeWarning("DoubleCVar");
				return;
			}

			*static_cast<double*>(this->ReferenceVar) = Value;
		}

		using VarType = double;
	};


	template<typename IntType>
	struct IntCVar : BaseCVar {

		virtual void Set(const char* Argument) override {
			errno = 0;

			constexpr long long IntMinimumValue = ((2 ^ sizeof(IntType) * 8) / -2) + 1;

			IntType Value;
			if CONSTEXPR_IF(sizeof(IntType) <= sizeof(long)) {
				Value = static_cast<IntType>(Math::Clamp<long>(std::strtol(Argument, NULL, 10), ((2 ^ sizeof(IntType) * 8) / -2) + 1, ((2 ^ sizeof(IntType) * 8) / 2) + 1));
			} else {
				Value = static_cast<IntType>(Math::Clamp<long long>(std::strtoll(Argument, NULL, 10), ((2 ^ sizeof(IntType) * 8) / -2) + 1, ((2 ^ sizeof(IntType) * 8) / 2) + 1));
			}

			if (errno == ERANGE) {
				this->RaiseRangeWarning("FloatCVar");
				return;
			}

			*static_cast<Sint8*>(this->ReferenceVar) = static_cast<Sint8>(Math::Clamp<long>(Value, INT8_MIN, INT8_MAX));
		}

		using VarType = IntType;
	};

	template<typename UintType>
	struct UintCVar : BaseCVar {
		virtual void Set(const char* Argument) override;
		using VarType = UintType;
	};


	using Int8CVar  = IntCVar<Sint8>;
	using Int16CVar = IntCVar<Sint16>;
	using Int32CVar = IntCVar<Sint32>;
	using Int64CVar = IntCVar<Sint64>;

	using Uint8CVar  = UintCVar<Uint8>;
	using Uint16CVar = UintCVar<Uint16>;
	using Uint32CVar = UintCVar<Uint32>;
	using Uint64CVar = UintCVar<Uint64>;



	struct BoolCVar : BaseCVar {
		virtual void Set(const char* Argument) override {

			int i = 1;
			switch (*Argument) {
			case 't':

				while ("true"[i] != '\0') {

					if ("true"[i] != Argument[i]) {
						return;
					}
				}

				*static_cast<bool*>(this->ReferenceVar) = true;
				break;

			case 'f':

				while ("false"[i] != '\0') {

					if ("false"[i] != Argument[i]) {
						return;
					}
				}

				*static_cast<bool*>(this->ReferenceVar) = false;
				break;
			}
		}

		virtual const char* Get() override {
			return *static_cast<bool*>(this->ReferenceVar) ? "true" : "false";
		}

		using VarType = bool;
	};

	struct StringCVar : BaseCVar {
		virtual void Set(const char* Argument) override {

			const char* Index = Argument;
			while (true) {

				if (*Index == ' ') {
					
				}

				++Index;
			}
		}
		using VarType = const char*;
	};
}