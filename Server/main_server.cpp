#define SDL_MAIN_HANDLED

#define BUILD_SERVER

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
#include <SDL3_net/SDL_net.h>

#include <lua-5.5.0/lua.hpp>


#include "../FunctionHeaders/TypeHelper.hpp"

#include "../pch.h"


#include "Network_Server.hpp"
#include "Sound_Server.hpp"

#include "Game_Server.hpp"
#include "../Lua.hpp"



int main() {

	if (!SDL_Init(SDL_INIT_EVENTS)) {
		Exceptions::ThrowSDLError("Failed to initialize SDL.");
	}
	
	Game::Network::Init();
	Game::Lua::Init();

	std::chrono::high_resolution_clock::time_point Start;
	std::chrono::milliseconds Delta;
	while (true) {
		Start = std::chrono::high_resolution_clock::now();

		Game::Lua::Update();
		Game::Network::SendPacket();

		Delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - Start);

		if (Delta > std::chrono::milliseconds(0) && Delta < std::chrono::milliseconds(16)) {
			std::this_thread::sleep_for(Delta);
		}
	}

	return -1;
}