#pragma once

#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_keyboard.h>

#include "../MathClasses/Vector2.hpp"


namespace {
	static Uint32 _MouseButtonMask;
	static Math::IVector2 _MousePosition;
}


namespace Game {
	namespace Input {

		inline void Update();


		namespace Keyboard {

			inline bool KeyPressed(SDL_Scancode Key);
		}

		namespace Mouse {

			inline bool GetLeftPressed();
			inline bool GetRightPressed();
			inline bool GetMiddlePressed();

			inline const Math::IVector2& GetPosition();
		}

		namespace Binding {

			void CreateBinding();
		}
	}
}


void Game::Input::Update() {
	::_MouseButtonMask = SDL_GetMouseState(&::_MousePosition.X, &::_MousePosition.Y);
}


bool Game::Input::Keyboard::KeyPressed(SDL_Scancode Key) {
	return static_cast<bool>(SDL_GetKeyboardState(nullptr)[Key]);
}


inline bool Game::Input::Mouse::GetLeftPressed() {
	return SDL_BUTTON(::_MouseButtonMask, SDL_BUTTON_LEFT);
}
inline bool Game::Input::Mouse::GetRightPressed() {
	return SDL_BUTTON(::_MouseButtonMask, SDL_BUTTON_RIGHT);
}
inline bool Game::Input::Mouse::GetMiddlePressed() {
	return SDL_BUTTON(::_MouseButtonMask, SDL_BUTTON_MIDDLE);
}

const Math::IVector2& Game::Input::Mouse::GetPosition() {
	return ::_MousePosition;
}