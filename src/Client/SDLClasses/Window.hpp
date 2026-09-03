#ifndef WINDOW_HPP
#define WINDOW_HPP 1

#include "../../define.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>

#include "../../MathClasses/Vector2.hpp"

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
            double StateChangeTimestamp;  // in secs
        };

        mutable ScancodeStateStruct
            ScancodeStates[SDL_Scancode::SDL_SCANCODE_COUNT];

//#ifndef SDL_PLATFORM_VITA
        SDL_Window* SDLWindow;
        SDL_GLContext GLContext;
//#endif

        Math::IVector2 Position, Size;

       public:
        void Create(const char* WindowTitle,
                    int Width,
                    int Height,
                    SDL_WindowFlags Flags = 0);
        Window();

        void Destroy();

        Math::Vector2 MouseDelta;

        Utils::Event<const Math::IVector2&> WindowResizedEvent;

        bool HasMouseFocus = false;
        bool Focus = false;

        ScancodeStatesEnum MouseButtonStates[5];

        Uint64 FrameNSFocused = 16 * 1000000;
        Uint64 FrameNSUnfocused = FrameNSFocused * 2;
        Uint64 FrameNS = FrameNSFocused;

        void HandleEvent(const SDL_Event& Event);

        const Math::IVector2& GetPosition() const;
        const Math::IVector2& GetSize() const;

        bool ScancodePressed(SDL_Scancode Scancode,
                             float BufferTime = 0.f) const;
        bool ScancodeHeld(SDL_Scancode Scancode, float BufferTime = 0.f) const;
        bool ScancodeReleased(SDL_Scancode Scancode,
                              float BufferTime = 0.f) const;

        void Present();
    };
}

#endif