#define SDL_MAIN_HANDLED

#define BUILD_CLIENT

#include "../define.h"

#include <iostream>
#include <filesystem>

#include <thread>
#include <chrono>

#include <string>
#include <vector>
#include <map>

#include <type_traits>
#include <typeinfo>
#include <tuple>
#include <utility>

#include <cmath>

#ifdef USE_SIMD_INTRINSICS
#include <xmmintrin.h>
#include <pmmintrin.h>
#include <smmintrin.h>
#include <mmintrin.h>
#endif

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <SDL3_net/SDL_net.h>
#include <SDL3_mixer/SDL_mixer.h>


#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <lua-5.5.0/lua.hpp>

#include <glad/glad.h>
#include <glad/glad.c>


#include "../Memory.hpp"

#include "../FunctionHeaders/TypeHelper.hpp"

#include "../pch.h"


namespace Game {
	double DeltaTime = 1.0;
}

#include "SDLClasses/Window.hpp"
namespace Game {
	Game::Classes::Window Window;

	inline void Update();
	void Draw();
}

#include "Sound_Client.hpp"
#include "Network_Client.hpp"
#include "Graphics_Client.hpp"

#include "../Lua.hpp"





void Game::Update() {

	Game::Statistics::Update();

	Game::Lua::Update();
}

void Game::Draw() {

	Game::Graphics::SetClearColor(0.f, 0.f, 0.f, 1.f);
	Game::Graphics::ClearBitfields(Game::Graphics::BufferBitfields.ColorBit | Game::Graphics::BufferBitfields.DepthBit | Game::Graphics::BufferBitfields.StencilBit);

	Game::Graphics::Draw();
	Game::Lua::Draw();
}



namespace {

	static bool _Closed = false;
	static void _CloseInitialisedItems() try {

		if (::_Closed) {
			return;
		}
		::_Closed = true;


		Game::Lua::Destroy();
		Game::Sound::Destroy();

		Game::Window.Destroy();
		SDL_Quit();

	} catch (const Exceptions::Exception& Exception) {
		std::clog << "Caught a unhandled exception while closing libraries. Exception message:\n" << Exception.what() << std::endl;
	} catch (...) {
		std::clog << "Caught a unknown unhandled exception while closing libraries." << std::endl;
	}
}


int main()

#ifndef DEBUG_BUILD
try
#endif
{
	if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK)) {
		Exceptions::ThrowSDLError("Failed to initalize SDL.");
	}
	
	
	Game::Sound::Init();
	Game::Network::Init();
	
	Game::Window.Create("Cube Cavern++", 800, 600);

	Game::Graphics::Init();
	Game::Lua::Init();


	while (true) {

		unlikely_branch
		if (Game::Window.PollEvents()) {
			break;
		}

		Game::Update();
		
		Game::Draw();

		Game::Window.Present();
	}

	::_CloseInitialisedItems();

	return EXIT_SUCCESS;
}

#ifndef DEBUG_BUILD
catch (const Exceptions::Exception& Exception) {

	std::cerr << "::FATAL:: ::UNHANDLED EXCEPTION:: " << Exception.what() << std::endl;
	::_CloseInitialisedItems();
	
	if (!SDL_ShowSimpleMessageBox(
		SDL_MESSAGEBOX_ERROR,
		"Unhandled Exception",

		StringHelper::Combine(
			"An unhandled exception occurred, causing the program to shit itself. We're NOT sorry!\nException message:\n",
			Exception.what()
		).c_str(),

		NULL
	)) {
		std::cerr << "An error occurred when calling 'SDL_ShowSimpleMessageBox'.\nSDL Error: " << SDL_GetError() << std::endl;
	}

	return EXIT_FAILURE;

} catch (...) {

	std::cerr << "::FATAL:: ::UNHANDLED UNKNOWN EXCEPTION::" << std::endl;
	::_CloseInitialisedItems();

	if (!SDL_ShowSimpleMessageBox(
		SDL_MESSAGEBOX_ERROR,
		"Unhandled Unknown Exception",
		"An unhandled unknown exception occurred, causing the program to shit itself. We're NOT sorry!",
		NULL
	)) {
		std::cerr << "An error occurred when calling 'SDL_ShowSimpleMessageBox'.\nSDL Error: " << SDL_GetError() << std::endl;
	}

	return EXIT_FAILURE;
}
#endif