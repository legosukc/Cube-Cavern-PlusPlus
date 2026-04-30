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

#include <sockpp/tcp6_acceptor.h>
#include <sockpp/tcp6_connector.h>

#include <sockpp/udp6_socket.h>

#ifdef USE_SIMD_INTRINSICS
#include <xmmintrin.h>
#include <pmmintrin.h>
#include <smmintrin.h>
#include <mmintrin.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <lua-5.4.2/lua.hpp>


#include "../FunctionHeaders/TypeHelper.hpp"

#include "../pch.h"


#include "SDLClasses/Window.hpp"

#include "Sound_Client.hpp"
#include "Network_Client.hpp"

#include "Lua_Client.hpp"
#include "Game_Client.hpp"



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
	unlikely_branch
	if (const int ErrorCode = SDL_Init(SDL_INIT_VIDEO); ErrorCode != 0) {
		Exceptions::ThrowSDLError("Failed to initalize SDL2. ErrorCode: ", ErrorCode);
	}

	Game::Sound::Init();
	//Game::Network::Init();
	

	Game::Window.Create("Cube Cavern++", 800, 600);

	Game::Lua::Init();

	//std::thread LuaLoaderThread(Game::Lua::Init);
	
	//LuaLoaderThread.join();


	while (true) {

		unlikely_branch
		if (Game::Window.PollEvents()) {
			break;
		}

		Game::Update();

		Game::Window.Present();
	}

	::_CloseInitialisedItems();

	return EXIT_SUCCESS;
}

#ifndef DEBUG_BUILD
catch (const Exceptions::Exception& Exception) {

	std::cerr << "::FATAL:: ::UNHANDLED EXCEPTION:: " << Exception.what() << std::endl;
	::_CloseInitialisedItems();

	const int ErrorCode = SDL_ShowSimpleMessageBox(
		SDL_MESSAGEBOX_ERROR,
		"Unhandled Exception",

		StringHelper::Combine(
			"An unhandled exception occurred, causing the program to shit itself. We're NOT sorry!\nException message:\n",
			Exception.what()
		).c_str(),

		NULL
	);
	
	if (ErrorCode != 0) {
		std::cerr << "An error occurred when calling 'SDL_ShowSimpleMessageBox'. Error Code: " << ErrorCode
			<< "\nSDL Error: " << SDL_GetError() << std::endl;
	}

	return EXIT_FAILURE;

} catch (...) {

	std::cerr << "::FATAL:: ::UNHANDLED UNKNOWN EXCEPTION::" << std::endl;
	::_CloseInitialisedItems();

	const int ErrorCode = SDL_ShowSimpleMessageBox(
		SDL_MESSAGEBOX_ERROR,
		"Unhandled Unknown Exception",
		"An unhandled unknown exception occurred, causing the program to shit itself. We're NOT sorry!",
		NULL
	);

	if (ErrorCode != 0) {
		std::cerr << "An error occurred when calling 'SDL_ShowSimpleMessageBox'. Error Code: " << ErrorCode
			<< "\nSDL Error: " << SDL_GetError() << std::endl;
	}

	return EXIT_FAILURE;
}
#endif