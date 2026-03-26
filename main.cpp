#define SDL_MAIN_HANDLED

#include "define.h"

#include <iostream>
#include <filesystem>

#include <thread>
#include <chrono>

#include <string>
#include <vector>

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


#include <stdint.h>
typedef int8_t Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;


#include <SDL2/SDL.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <lua-5.4.2/lua.hpp>


#include "TypeHelper.hpp"

#include "pch.h"


#include "Window.hpp"

#include "Game.hpp"
#include "Sound.hpp"
#include "Lua.hpp"



namespace {

	static bool _Closed = false;
	static void _CloseInitialisedItems() try {

		if (::_Closed) {
			return;
		}
		::_Closed = true;


		Game::Lua::Destroy();

		Sound::Destroy();

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

	Sound::Init();

	Game::Window.Create("Cube Cavern++", 800, 600);

	std::thread LuaLoaderThread(Game::Lua::Init);
	
	LuaLoaderThread.join();


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