#pragma once

#include <tuple>
#include <algorithm>

namespace TypeHelper {

    template<std::size_t N>
    struct fixed_string {
        constexpr fixed_string(const char(&str)[N]) {
            std::copy_n(str, N, data);
        }
        char data[N];
    };

    // Deduction guide
    template<std::size_t N>
    fixed_string(const char(&)[N]) -> fixed_string<N - 1>; // Drop null terminator


    template<typename T>
    struct FunctionTraits;

    template<typename R, typename... Args>
    struct FunctionTraits<R(Args...)> {

        constexpr static size_t ArgumentCount = sizeof...(Args);
        using ArgumentTypes = std::tuple<Args...>;

        template<size_t Index>
        using IndexArgType = std::tuple_element_t<Index, ArgumentTypes>;

        using ReturnType = R;
    };

    template<typename R, typename Class, typename... Args>
    struct FunctionTraits<R(Class::*)(Args...)> : public FunctionTraits<R(Args...)> {
        using ClassType = Class;
    };


    template<typename T>
    using RemoveTypeQualifiers = std::remove_cv_t<T>;


    // Primary template: defaults to false
    template <typename T>
    struct is_c_string : std::false_type {};

    // Specializations for char pointers
    template <>
    struct is_c_string<char*> : std::true_type {};

    template <>
    struct is_c_string<const char*> : std::true_type {};

    // Specializations for char arrays
    template <std::size_t N>
    struct is_c_string<char[N]> : std::true_type {};

    template <std::size_t N>
    struct is_c_string<const char[N]> : std::true_type {};


    // Primary template: defaults to false
    template<typename T>
    struct is_literal : std::false_type {};

    template <std::size_t N>
    struct is_literal<const char[N]> : std::true_type {};


    // Helper variable template (C++14+)
    template <typename T>
    constexpr INLINE_MEMBER_VARIABLE bool is_c_string_v = is_c_string<std::remove_reference_t<RemoveTypeQualifiers<T>>>::value;


    template <typename T>
    constexpr INLINE_MEMBER_VARIABLE bool is_literal_v = is_literal<T>::value;

    
    template<typename T>
    using GetSimpleTypeSpecifier = std::remove_cv_t<std::remove_pointer_t<std::remove_all_extents_t<T>>>;

    template<typename... T>
    struct sizeofTypes;
    
    template<>
    struct sizeofTypes<> {
        constexpr static INLINE_MEMBER_VARIABLE size_t size = 0;
    };

    template<typename T, typename... Rest>
    struct sizeofTypes<T, Rest...> {
        constexpr static INLINE_MEMBER_VARIABLE size_t size = sizeofTypes<Rest...>::size + sizeof(T);
    };

    template<typename... T>
    constexpr static INLINE_MEMBER_VARIABLE size_t sizeofTypes_v = sizeofTypes<T...>::size;
}