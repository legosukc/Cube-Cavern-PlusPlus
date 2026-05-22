#pragma once

#include "../../define.h"

#include "../../MathClasses/Vector2.hpp"

#include <chrono>
#include <thread>

#include <glad/glad.h>

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_stdinc.h>


namespace Game::Classes {

	class Window {

		mutable std::chrono::steady_clock::time_point LastFrameTime = std::chrono::steady_clock::now();

		enum class ScancodeStatesEnum : Uint8 {
			Released,
			Pressed,
			Held,
		};
		mutable ScancodeStatesEnum ScancodeStates[SDL_Scancode::SDL_SCANCODE_COUNT];

	public:
		void Create(const char* WindowTitle, int Width, int Height, SDL_WindowFlags Flags = 0);
		inline Window() = default;

		inline void Destroy();

		Math::Vector2 MouseDelta;
		Math::IVector2 Position, Size;

		bool HasMouseFocus = false;
		bool Focus = false;

		SDL_Window* SDLWindow;
		SDL_GLContext GLContext;

		Uint32 FrameMs = 16;
		Uint32 FrameMsBackground = 100;


		inline bool PollEvents();

		Math::IVector2 GetPosition() const;
		Math::IVector2 GetSize() const;

		inline bool ScancodePressed(SDL_Scancode Scancode) const;
		bool ScancodeHeld(SDL_Scancode Scancode) const;
		inline bool ScancodeReleased(SDL_Scancode Scancode) const;

		void Present() const;
	};
}


void Game::Classes::Window::Create(const char* WindowTitle, int Width, int Height, SDL_WindowFlags Flags) {

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	this->SDLWindow = SDL_CreateWindow(WindowTitle, Width, Height, SDL_WINDOW_OPENGL | Flags);

	unlikely_branch
		if (this->SDLWindow == NULL) {
			Exceptions::ThrowSDLError("Failed to create SDL_Window.");
		}


	this->GLContext = SDL_GL_CreateContext(this->SDLWindow);

	unlikely_branch
		if (this->GLContext == NULL) {
			Exceptions::ThrowSDLError("Failed to create SDL_GLContext.");
		}

	unlikely_branch
		if (!SDL_GL_MakeCurrent(this->SDLWindow, this->GLContext)) {
			Exceptions::ThrowSDLError("Error while calling SDL_GL_MakeCurrent.");
		}

	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	std::memset(&this->ScancodeStates, 0, sizeof(this->ScancodeStates));
}

void Game::Classes::Window::Destroy() {

	if (this->GLContext != NULL) {
		if (SDL_GL_DestroyContext(this->GLContext)) {
			this->GLContext = NULL;
		} else {
			std::clog << "Failed to destroy the OpenGL context." << std::endl;
		}
	}

	if (this->SDLWindow != NULL) {
		SDL_DestroyWindow(this->SDLWindow);
		this->SDLWindow = NULL;
	}
}



bool Game::Classes::Window::PollEvents() {

	this->MouseDelta = Math::Vector2(1.f, 0.f);

	SDL_Event Event;
	while (SDL_PollEvent(&Event)) {

		switch (Event.type) {
		case SDL_EventType::SDL_EVENT_QUIT:
			return true;

		case SDL_EventType::SDL_EVENT_WINDOW_RESIZED:
			//this->Size = Math::IVector2(Event.window.data1, Event.window.data2);
			this->Size = *reinterpret_cast<Math::IVector2*>(&Event.window.data1);
			break;
		case SDL_EventType::SDL_EVENT_WINDOW_MOVED:
			//this->Position = Math::IVector2(Event.window.data1, Event.window.data2);
			this->Position = *reinterpret_cast<Math::IVector2*>(&Event.window.data1);
			break;


		case SDL_EventType::SDL_EVENT_WINDOW_MOUSE_ENTER:
			this->HasMouseFocus = true;
		case SDL_EventType::SDL_EVENT_WINDOW_SHOWN:
			this->Focus = true;
			break;


		case SDL_EventType::SDL_EVENT_WINDOW_MOUSE_LEAVE:
			this->HasMouseFocus = false;
		case SDL_EventType::SDL_EVENT_WINDOW_HIDDEN:
			this->Focus = false;
			break;

		case SDL_EventType::SDL_EVENT_KEY_UP:
		case SDL_EventType::SDL_EVENT_KEY_DOWN:
			this->ScancodeStates[Event.key.scancode] = static_cast<ScancodeStatesEnum>(Event.key.down);
			break;

		case SDL_EventType::SDL_EVENT_MOUSE_MOTION:

			this->MouseDelta = *reinterpret_cast<Math::Vector2*>(&Event.motion.xrel);
			//this->MouseDelta.X = Event.motion.xrel;
			//this->MouseDelta.Y = Event.motion.yrel;
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
	return this->ScancodeStates[Scancode] == ScancodeStatesEnum::Pressed;
}

bool Game::Classes::Window::ScancodeHeld(SDL_Scancode Scancode) const {

	ScancodeStatesEnum& ScancodeState = this->ScancodeStates[Scancode];
	if (ScancodeState == ScancodeStatesEnum::Released) {
		return false;
	}
	ScancodeState = ScancodeStatesEnum::Held;

	return true;
}

bool Game::Classes::Window::ScancodeReleased(SDL_Scancode Scancode) const {
	return this->ScancodeStates[Scancode] == ScancodeStatesEnum::Released;
}


void Game::Classes::Window::Present() const {

	std::this_thread::sleep_until(this->LastFrameTime + std::chrono::milliseconds(this->Focus ? this->FrameMs : this->FrameMsBackground));

	SDL_GL_SwapWindow(this->SDLWindow);

	Game::DeltaTime = Math::Max<double>(static_cast<double>(
		std::chrono::duration_cast<std::chrono::milliseconds>(this->LastFrameTime - (this->LastFrameTime = std::chrono::steady_clock::now())
		).count()
		) / 1000.0,
		5e-324 // really really small non-zero double (make sure delta isn't 0 cause that could cause some wacky things to happen
	);
	//this->LastFrameTime = std::chrono::steady_clock::now();
}