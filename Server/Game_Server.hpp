#pragma once

#include <SDL2/SDL_events.h>


namespace Game {

	inline void Update();
	inline bool PollEvents();

	void Draw();
}


namespace {
	static SDL_Event Event;
}

bool Game::PollEvents() {

	bool Quit = false;
	while (SDL_PollEvent(&::Event)) {

		switch (::Event.type) {
		case SDL_QUIT:
			Quit = true;
			break;
		}
	}

	return Quit;
}

void Game::Draw() {

	
}