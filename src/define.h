#pragma once

#ifndef DEFINE_H
#define DEFINE_H 1

// for linting
#if !defined(BUILD_CLIENT) && !defined(BUILD_SERVER)
#define BUILD_CLIENT
#endif

#define GL_GLES_PROTOTYPES 0

#define SDL_OPENGL_1_NO_PROTOTYPES 0
#define SDL_OPENGL_1_FUNCTION_TYPEDEFS 0

#define GL_APIENTRYP *


#if defined(DEBUG) && !defined(_DEBUG)
#define _DEBUG

#elif defined(_DEBUG) && !defined(DEBUG)
#define DEBUG

#endif


#ifdef _MSVC_LANG

// MSVC-specific macro
#define CPP_VERSION _MSVC_LANG

#else

// Standard macro
#define CPP_VERSION __cplusplus

#endif

#if CPP_VERSION >= 199711L
#define CPP_98_SUPPORTED

#if CPP_VERSION >= 201103L
#define CPP_11_SUPPORTED

#if CPP_VERSION >= 201402L
#define CPP_14_SUPPORTED

#if CPP_VERSION >= 201703L
#define CPP_17_SUPPORTED

#if CPP_VERSION >= 202002L
#define CPP_20_SUPPORTED

#if CPP_VERSION >= 202302L
#define CPP_23_SUPPORTED

// (same as C++23 until finalized)
#if CPP_VERSION >= 202302
#define CPP_26_DRAFT_SUPPORTED

#endif
#endif
#endif
#endif
#endif
#endif
#endif


#if defined(_MSVC_LANG) || defined(__clang__)
#define restrict __restrict
#else
#define restrict
#endif

/*
Determines whether to use SIMD intrinsics to speed up certain functions.
Comment out to use regular math operations. (The compiler can slot in SIMD instructions if it sees fit.)
*/
//#define USE_SIMD_INTRINSICS

#ifdef USE_SIMD_INTRINSICS

#ifdef __clang__
#define SSE3_FUNCTION __attribute__((target("sse3")))

#else
#define SSE3_FUNCTION

#endif

#else

#define SSE3_FUNCTION

#endif





#ifdef CPP_20_SUPPORTED

#define likely_branch [[likely]]
#define unlikely_branch [[unlikely]]

#else

#define likely_branch
#define unlikely_branch

#endif

#ifdef CPP_17_SUPPORTED

#define CONSTEXPR_IF constexpr
#define INLINE_MEMBER_VARIABLE inline

#else

#define CONSTEXPR_IF
#define INLINE_MEMBER_VARIABLE

#endif

#ifndef LOAD_OPENGL_FUNCTION

#define LOAD_OPENGL_FUNCTION(FunctionName)                         \
    (Game::Graphics::OpenGLFunctions::FunctionName =               \
         (decltype(Game::Graphics::OpenGLFunctions::FunctionName)) \
             SDL_GL_GetProcAddress(#FunctionName))

#endif

#endif