#ifndef WINDOW_HPP
#define WINDOW_HPP 1

#include "../../define.h"

#if defined(USE_SIMD_INTRINSICS) && !defined(SDL_PLATFORM_VITA)
#include <emmintrin.h>
#include <immintrin.h>
#endif

#include <iostream>

// #include <SDL3/SDL_egl.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>

#ifdef SDL_PLATFORM_VITA
#include <vitaGL.h>
#endif

#include "../../FunctionHeaders/Exceptions.hpp"
#include "../../MathClasses/Vector2.hpp"

#include "../../Time.hpp"

#include "../../FunctionHeaders/Event.hpp"

namespace Game::Classes {

    class Window {
        mutable Uint64 LastNS = SDL_GetTicksNS();

        enum class ScancodeStatesEnum : Uint8 {
            NotHeld,
            Released,
            Pressed,
            Held,
        };

        struct ScancodeStateStruct {
            ScancodeStatesEnum CurrentState = ScancodeStatesEnum::NotHeld;
            float StateChangeTimestamp;  // in secs
        };

        mutable ScancodeStateStruct
            ScancodeStates[SDL_Scancode::SDL_SCANCODE_COUNT];

#ifndef SDL_PLATFORM_VITA
        SDL_Window* SDLWindow;
        SDL_GLContext GLContext;
#endif

        Math::IVector2 Position, Size;

       public:
        void Create(const char* WindowTitle,
                    int Width,
                    int Height,
                    SDL_WindowFlags Flags = 0);
        inline Window();

        inline void Destroy();

        Math::Vector2 MouseDelta;

        Utils::Event<const Math::IVector2&> WindowResizedEvent;

        bool HasMouseFocus = false;
        bool Focus = false;

        ScancodeStatesEnum MouseButtonStates[5];

        Uint64 FrameNSFocused = 16 * 1000000;
        Uint64 FrameNSUnfocused = FrameNSFocused * 2;
        Uint64 FrameNS = FrameNSFocused;

        void HandleEvent(const SDL_Event& Event);

        inline const Math::IVector2& GetPosition() const;
        inline const Math::IVector2& GetSize() const;

        bool ScancodePressed(SDL_Scancode Scancode,
                             float BufferTime = 0.f) const;
        bool ScancodeHeld(SDL_Scancode Scancode, float BufferTime = 0.f) const;
        bool ScancodeReleased(SDL_Scancode Scancode,
                              float BufferTime = 0.f) const;

        void Present();
    };
}

void Game::Classes::Window::Create(const char* WindowTitle,
                                   int Width,
                                   int Height,
                                   SDL_WindowFlags Flags) {
#ifdef SDL_PLATFORM_VITA
    std::cout << "version 2.0 es" << std::endl;

    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
    // SDL_GL_CONTEXT_PROFILE_ES);

    Width = 960;
    Height = 544;
#else
    std::cout << "version 3.30 core" << std::endl;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
#endif

#ifndef SDL_PLATFORM_VITA
    this->SDLWindow =
        SDL_CreateWindow(WindowTitle, Width, Height, SDL_WINDOW_OPENGL | Flags);
    if (this->SDLWindow == NULL) {
        Exceptions::ThrowSDLError("Failed to create SDL_Window.");
    }
#endif

    this->Size = Math::IVector2(Width, Height);

#ifndef SDL_PLATFORM_VITA
    this->GLContext = SDL_GL_CreateContext(this->SDLWindow);
    if (this->GLContext == NULL) {
        Exceptions::ThrowSDLError("Failed to create SDL_GLContext.");
    }

    if (!SDL_GL_MakeCurrent(this->SDLWindow, this->GLContext)) {
        Exceptions::ThrowSDLError("Error while calling SDL_GL_MakeCurrent.");
    }

    SDL_SetWindowRelativeMouseMode(this->SDLWindow, true);
#endif

    std::memset(&this->ScancodeStates, 0, sizeof(this->ScancodeStates));
}
Game::Classes::Window::Window() = default;

void Game::Classes::Window::Destroy() {
#ifndef SDL_PLATFORM_VITA
    if (this->SDLWindow != NULL) {
        SDL_GL_MakeCurrent(this->SDLWindow, NULL);
        SDL_DestroyWindow(this->SDLWindow);
        this->SDLWindow = NULL;
    }

    if (this->GLContext != NULL) {
        if (SDL_GL_DestroyContext(this->GLContext)) {
            this->GLContext = NULL;
        } else {
            std::clog << "Failed to destroy the OpenGL context." << std::endl;
        }
    }
#endif
}

void Game::Classes::Window::HandleEvent(const SDL_Event& Event) {
    int numkeys;
    const bool* KeyboardState = SDL_GetKeyboardState(&numkeys);
    /*
    #ifndef USE_SIMD_INTRINSICS
        for (const bool* i = KeyboardState;
             i < KeyboardState + std::min((int)SDL_SCANCODE_COUNT, numkeys);
             i += sizeof(__m128i)) {
            __m128i KeyStatesVector = _mm_loadrs_epi8(KeyboardState);
            _mm_min_epi8(KeyStatesVector, _mm_set1_epi8(1))
            if (KeyboardState) {
                this->ScancodeStates[i] = this->ScancodeStates[i]
            }
        }
    #else
    #endif
    */
    switch (Event.type) {
        case SDL_EventType::SDL_EVENT_WINDOW_RESIZED:
        case SDL_EventType::SDL_EVENT_WINDOW_MAXIMIZED:
        case SDL_EventType::SDL_EVENT_WINDOW_MINIMIZED:
            this->Size.X = Event.window.data1;
            this->Size.Y = Event.window.data2;
            this->WindowResizedEvent.Fire(this->Size);
            break;

        case SDL_EventType::SDL_EVENT_WINDOW_MOVED:
            this->Position.X = Event.window.data1;
            this->Position.Y = Event.window.data2;
            break;

        case SDL_EventType::SDL_EVENT_WINDOW_MOUSE_ENTER:
            this->HasMouseFocus = true;
        case SDL_EventType::SDL_EVENT_WINDOW_SHOWN:
            this->Focus = true;
            this->FrameNS = this->FrameNSFocused;
            break;

        case SDL_EventType::SDL_EVENT_WINDOW_MOUSE_LEAVE:
            this->HasMouseFocus = false;
        case SDL_EventType::SDL_EVENT_WINDOW_HIDDEN:
            this->Focus = false;
            this->FrameNS = this->FrameNSUnfocused;
            break;

        case SDL_EventType::SDL_EVENT_KEY_UP:
        case SDL_EventType::SDL_EVENT_KEY_DOWN:

            std::cout << "scancode " << SDL_GetScancodeName(Event.key.scancode)
                      << " state " << std::boolalpha << Event.key.down
                      << std::endl;
            this->ScancodeStates[Event.key.scancode].CurrentState =
                static_cast<ScancodeStatesEnum>(Event.key.down);
            break;

        case SDL_EventType::SDL_EVENT_MOUSE_MOTION:
            this->MouseDelta.X = Event.motion.xrel;
            this->MouseDelta.Y = Event.motion.yrel;
            break;
    }
}

const Math::IVector2& Game::Classes::Window::GetPosition() const {
    return this->Position;
}

const Math::IVector2& Game::Classes::Window::GetSize() const {
    return this->Size;
}

bool Game::Classes::Window::ScancodePressed(SDL_Scancode Scancode,
                                            float BufferTime) const {
    // return this->ScancodeStates[Scancode] == ScancodeStatesEnum::Pressed;
    return SDL_GetKeyboardState(NULL)[Scancode];
}

bool Game::Classes::Window::ScancodeHeld(SDL_Scancode Scancode,
                                         float BufferTime) const {
    return SDL_GetKeyboardState(NULL)[Scancode];

    ScancodeStateStruct& ScancodeState = this->ScancodeStates[Scancode];
    if (ScancodeState.CurrentState == ScancodeStatesEnum::Released) {
        return false;
    }
    ScancodeState.CurrentState = ScancodeStatesEnum::Held;

    return ScancodeState.CurrentState == ScancodeStatesEnum::Released;
}

bool Game::Classes::Window::ScancodeReleased(SDL_Scancode Scancode,
                                             float BufferTime) const {
    // return this->ScancodeStates[Scancode] == ScancodeStatesEnum::Released;
    return !SDL_GetKeyboardState(NULL)[Scancode];
}

void Game::Classes::Window::Present() {
    this->MouseDelta = Math::Vector2(0.f, 0.f);

    SDL_DelayPrecise(this->FrameNS - (SDL_GetTicksNS() - this->LastNS));

#ifdef SDL_PLATFORM_VITA
// vglSwa
#else

    SDL_GL_SwapWindow(this->SDLWindow);
#endif

    const Uint64 TicksNS = SDL_GetTicksNS();
    Game::Time::DeltaTime = static_cast<double>(TicksNS - this->LastNS) / 1e+9;
    this->LastNS = TicksNS;
}

#endif