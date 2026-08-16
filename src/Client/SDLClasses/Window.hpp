#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>

#include "../../FunctionHeaders/Exceptions.hpp"
#include "../../MathClasses/Vector2.hpp"

#include "../../Time.hpp"

#include "../../FunctionHeaders/Event.hpp"

namespace Game::Classes {

    class Window {
        mutable Uint64 LastNS = SDL_GetTicksNS();

        enum class ScancodeStatesEnum : Uint8 {
            Released,
            Pressed,
            Held,
        };
        mutable ScancodeStatesEnum
            ScancodeStates[SDL_Scancode::SDL_SCANCODE_COUNT];

       public:
        void Create(const char* WindowTitle,
                    int Width,
                    int Height,
                    SDL_WindowFlags Flags = 0);
        inline Window();

        inline void Destroy();

        Math::Vector2 MouseDelta;
        Math::IVector2 Position, Size;

        Utils::Event<const Math::IVector2&> WindowResizedEvent;

        bool HasMouseFocus = false;
        bool Focus = false;

        SDL_Window* SDLWindow;
        SDL_GLContext GLContext;

        Uint64 FrameNSFocused = 16 * 1000000;
        Uint64 FrameNSUnfocused = 100 * 1000000;
        Uint64 FrameNS = FrameNSFocused;

        inline bool PollEvents();

        Math::IVector2 GetPosition() const;
        Math::IVector2 GetSize() const;

        inline bool ScancodePressed(SDL_Scancode Scancode) const;
        bool ScancodeHeld(SDL_Scancode Scancode) const;
        inline bool ScancodeReleased(SDL_Scancode Scancode) const;

        void Present() const;
    };
}

void Game::Classes::Window::Create(const char* WindowTitle,
                                   int Width,
                                   int Height,
                                   SDL_WindowFlags Flags) {
#ifdef SDL_PLATFORM_PSVITA
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
#endif

    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
    // SDL_GL_CONTEXT_PROFILE_ES);

    this->SDLWindow =
        SDL_CreateWindow(WindowTitle, Width, Height, SDL_WINDOW_OPENGL | Flags);
    if (this->SDLWindow == NULL) {
        Exceptions::ThrowSDLError("Failed to create SDL_Window.");
    }
    this->Size = Math::IVector2(Width, Height);

    this->GLContext = SDL_GL_CreateContext(this->SDLWindow);
    if (this->GLContext == NULL) {
        Exceptions::ThrowSDLError("Failed to create SDL_GLContext.");
    }

    if (!SDL_GL_MakeCurrent(this->SDLWindow, this->GLContext)) {
        Exceptions::ThrowSDLError("Error while calling SDL_GL_MakeCurrent.");
    }

    SDL_SetWindowRelativeMouseMode(this->SDLWindow, true);

    std::memset(&this->ScancodeStates, 0, sizeof(this->ScancodeStates));
}
Game::Classes::Window::Window() = default;

void Game::Classes::Window::Destroy() {
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
}

bool Game::Classes::Window::PollEvents() {
    this->MouseDelta = Math::Vector2(0.f, 0.f);

    SDL_Event Event;
    while (SDL_PollEvent(&Event)) {
        switch (Event.type) {
            case SDL_EventType::SDL_EVENT_QUIT:
                return true;

            case SDL_EventType::SDL_EVENT_WINDOW_RESIZED:
            case SDL_EventType::SDL_EVENT_WINDOW_MAXIMIZED:
            case SDL_EventType::SDL_EVENT_WINDOW_MINIMIZED:
                
                SDL_GetWindowSize(this->SDLWindow, &this->Size.X, &this->Size.Y);
                this->WindowResizedEvent.Fire(this->Size);
                break;
            
            case SDL_EventType::SDL_EVENT_WINDOW_MOVED:
                // this->Position = Math::IVector2(Event.window.data1,
                // Event.window.data2);
                this->Position =
                    *reinterpret_cast<Math::IVector2*>(&Event.window.data1);
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

                // make sure to remove laterrr
                // exists so i don't accidentally alt-f4 vscode
                // elegant way of exitting (makes sure everything is cleaned up)
                if (Event.key.scancode == SDL_SCANCODE_ESCAPE) {
                    return true;
                }

                std::cout << "scancode "
                          << SDL_GetScancodeName(Event.key.scancode)
                          << " state " << std::boolalpha << Event.key.down
                          << std::endl;
                this->ScancodeStates[Event.key.scancode] =
                    static_cast<ScancodeStatesEnum>(Event.key.down);
                break;

            case SDL_EventType::SDL_EVENT_MOUSE_MOTION:
                this->MouseDelta.X = Event.motion.xrel;
                this->MouseDelta.Y = Event.motion.yrel;

                break;
        }
    }

    return false;
}

Math::IVector2 Game::Classes::Window::GetPosition() const {
    Math::IVector2 Position;
    SDL_GetWindowPosition(this->SDLWindow, &Position.X, &Position.Y);
    return Position;
}

Math::IVector2 Game::Classes::Window::GetSize() const {
    Math::IVector2 Size;
    SDL_GetWindowSize(this->SDLWindow, &Size.X, &Size.Y);
    return Size;
}

bool Game::Classes::Window::ScancodePressed(SDL_Scancode Scancode) const {
    // return this->ScancodeStates[Scancode] == ScancodeStatesEnum::Pressed;
    return SDL_GetKeyboardState(NULL)[Scancode];
}

bool Game::Classes::Window::ScancodeHeld(SDL_Scancode Scancode) const {
    // return SDL_GetKeyboardState(NULL)[Scancode];

    ScancodeStatesEnum& ScancodeState = this->ScancodeStates[Scancode];
    if (ScancodeState == ScancodeStatesEnum::Released) {
        return false;
    }
    ScancodeState = ScancodeStatesEnum::Held;

    return true;
}

bool Game::Classes::Window::ScancodeReleased(SDL_Scancode Scancode) const {
    // return this->ScancodeStates[Scancode] == ScancodeStatesEnum::Released;
    return !SDL_GetKeyboardState(NULL)[Scancode];
}

void Game::Classes::Window::Present() const {
    SDL_DelayPrecise(this->FrameNS - (SDL_GetTicksNS() - this->LastNS));

    SDL_GL_SwapWindow(this->SDLWindow);

    const Uint64 TicksNS = SDL_GetTicksNS();
    Game::Time::DeltaTime = static_cast<double>(TicksNS - this->LastNS) / 1e+9;
    this->LastNS = TicksNS;
}

#endif