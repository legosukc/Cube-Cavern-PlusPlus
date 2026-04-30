#pragma once

#include "../Statistics.hpp"

#include "SDLClasses/Window.hpp"


namespace Game {

	Game::Classes::Window Window;

	inline void Update();
	void Draw();
}

void Game::Update() {

	Game::Statistics::Update();

	Game::Lua::Update();
}

void Game::Draw() {

	Game::Window.Present();
}