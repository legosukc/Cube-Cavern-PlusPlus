#pragma once

#include <SDL3/SDL_events.h>


namespace Game {
	inline bool PollEvents();
}


bool Game::PollEvents() {

	SDL_Event Event;
	while (SDL_PollEvent(&Event)) {

		switch (Event.type) {
		case SDL_EVENT_QUIT:
			return true;
		}
	}

	return false;
}