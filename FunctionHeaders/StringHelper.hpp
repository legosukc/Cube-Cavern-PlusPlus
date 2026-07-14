#pragma once

#include "../define.h"

#include <string>
#include <cstring>
#include <cmath>

#include "TypeHelper.hpp"
#include "BitOp.hpp"

namespace StringHelper {

	constexpr bool IsWhitespace(char Character) {

		constexpr char WhitespaceCharacters[] = {
			' ', '	', '\n', '\0'
		};

		for (char WhitepspaceChar : WhitespaceCharacters) {
			if (WhitepspaceChar == Character) {
				return true;
			}
		}
		return false;
	}

	template<typename T>
	constexpr size_t Length(const T& X) {

		if CONSTEXPR_IF(TypeHelper::is_c_string_v<T>) {
			return strlen(X) + 1;

		} else if CONSTEXPR_IF(std::is_same_v<T, std::string>) {
			return X.size() + 1;

		} else if CONSTEXPR_IF(std::is_same_v<T, char>) {
			return 2;

		} else if CONSTEXPR_IF(std::is_integral_v<T>) {


			if CONSTEXPR_IF(std::is_signed_v<T>) {
				if (X == static_cast<T>(0)) {
					return 2;
				}

				return static_cast<size_t>(log10(std::abs(X)))
					+ static_cast<size_t>(
						BitOp::CheckBit<T>(X, (sizeof(T) * 8) - 1) // if number is negative, add 1 for negative symbol
						) + 2;
			} else if CONSTEXPR_IF(std::is_unsigned_v<T>) {

				if (X < static_cast<T>(10)) {
					return 2;
				}
				return static_cast<size_t>(log10(X)) + 2;
			}
		}

		return sizeof(T);
	}
}


namespace {

	template<typename T>
	constexpr static size_t _GetStrTotalSize(size_t Size, const T& arg) {

		static_assert(
			std::is_integral_v<T>
			|| TypeHelper::is_c_string_v<T>
			|| std::is_same_v<TypeHelper::RemoveTypeQualifiers<T>, std::string>,

			"Invalid Type"
		);

		return Size + StringHelper::Length<T>(arg);
	}

	template<typename T, typename... Args_T>
	constexpr static size_t _GetStrTotalSize(size_t Size, const T& arg, const Args_T&... Strs) {

		return _GetStrTotalSize<Args_T...>(_GetStrTotalSize<T>(Size, arg), Strs...);
	}



	template<typename T>
	constexpr static void _AppendString(std::string& Buffer, const T& Appendant) {

		static_assert(
			std::is_integral_v<T> || std::is_floating_point_v<T>
			|| TypeHelper::is_c_string_v<T>
			|| std::is_same_v<TypeHelper::RemoveTypeQualifiers<T>, std::string>,
			"Invalid Type"
			);

		if CONSTEXPR_IF(TypeHelper::is_c_string_v<T> || std::is_same_v<T, std::string>) {
			Buffer.append(Appendant);
		} else if CONSTEXPR_IF (std::is_same_v<T, char>) {
			Buffer.push_back(Appendant);
		} else if CONSTEXPR_IF (std::is_floating_point_v<T> || std::is_integral_v<T>) {
			Buffer.append(std::to_string(Appendant));
		}
	}

	template<typename T, typename ...Appendants_T>
	constexpr static inline void _AppendString(std::string& Buffer, const T& Appendant, const Appendants_T&... Appendants) {
		/*
		if constexpr (std::is_same_v<Appendant_T, char*> || std::is_same_v<Appendant_T, const char*>) {
			Buffer = strcpy(Buffer, Appendant);

		} else {
			Buffer = strcpy(Buffer, std::to_string(Appendant).c_str());
		}*/
		_AppendString<T>(Buffer, Appendant);
		_AppendString<Appendants_T...>(Buffer, Appendants...);
	}
}


namespace StringHelper {


	template<typename ...Appendants_T>
	inline std::string Combine(const Appendants_T&... Appendants) {
	
		std::string Buffer;
		Buffer.reserve(_GetStrTotalSize<Appendants_T...>(0, Appendants...));

		_AppendString<Appendants_T...>(Buffer, Appendants...);

		return Buffer;
	}
}