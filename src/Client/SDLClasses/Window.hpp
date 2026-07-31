#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>

#include "../../MathClasses/Vector2.hpp"

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