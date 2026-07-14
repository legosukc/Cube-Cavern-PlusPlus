#pragma once

#include "../define.h"

#include <type_traits>
#include <tuple>
#include <algorithm>

#ifndef DEPRECATE

#if defined(__GNUC__)
#define DEPRECATE(foo, msg) foo __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#define DEPRECATE(foo, msg) __declspec(deprecated(msg)) foo
#else
#error This compiler is not supported
#endif

#else
#warning Macro DEPRECATE isnt defined by FunctionHeaders\TypeHelper.hpp.
#endif

#ifndef PP_CAT
#define PP_CAT(x,y) PP_CAT1(x,y)
#else
#warning Macro PP_CAT isnt defined by FunctionHeaders\TypeHelper.hpp.
#endif

#ifndef PP_CAT1
#define PP_CAT1(x,y) x##y
#else
#warning Macro PP_CAT1 isnt defined by FunctionHeaders\TypeHelper.hpp.
#endif


namespace TypeHelper::detail {
    template<int test>
    struct converter : public std::true_type {};

    template<>
    struct converter<0> : public std::false_type {};
}

#ifndef STATIC_WARNING
#define STATIC_WARNING(cond, msg) \
struct PP_CAT(static_warning,__LINE__) { \
  DEPRECATE(void _(std::false_type const& ),msg) {}; \
  void _(std::true_type const& ) {}; \
  PP_CAT(static_warning,__LINE__)() {_(TypeHelper::detail::converter<(cond)>());} \
}
#else
#warning Macro STATIC_WARNING isnt defined by FunctionHeaders\TypeHelper.hpp.
#endif

#ifndef STATIC_WARNING_TEMPLATE
// Note: using STATIC_WARNING_TEMPLATE changes the meaning of a program in a small way.
// It introduces a member/variable declaration.  This means at least one byte of space
// in each structure/class instantiation.  STATIC_WARNING should be preferred in any 
// non-template situation.
//  'token' must be a program-wide unique identifier.
#define STATIC_WARNING_TEMPLATE(token, cond, msg) \
    STATIC_WARNING(cond, msg) PP_CAT(PP_CAT(_localvar_, token),__LINE__)
#else
#warning Macro STATIC_WARNING_TEMPLATE isnt defined by FunctionHeaders\TypeHelper.hpp.
#endif


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
    constexpr bool is_c_string_v = is_c_string<std::remove_reference_t<RemoveTypeQualifiers<T>>>::value;


    template <typename T>
    constexpr bool is_literal_v = is_literal<T>::value;

    
    template<typename T>
    using GetSimpleTypeSpecifier = std::remove_cv_t<std::remove_pointer_t<std::remove_all_extents_t<T>>>;

    template<typename... T>
    struct sizeofTypes;
    
    template<>
    struct sizeofTypes<> {
        constexpr static size_t size = 0;
    };

    template<typename T, typename... Rest>
    struct sizeofTypes<T, Rest...> {
        constexpr static size_t size = sizeofTypes<Rest...>::size + sizeof(T);
    };

    template<typename... T>
    constexpr size_t sizeofTypes_v = sizeofTypes<T...>::size;
}