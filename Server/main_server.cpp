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

#include <lua-5.4.2/lua.hpp>


#include "../FunctionHeaders/TypeHelper.hpp"

#include "../pch.h"


#include "Network_Server.hpp"
#include "Sound_Server.hpp"

#include "Lua_Server.hpp"
#include "Game_Server.hpp"



int main() {

	if (const int ErrorCode = SDL_InitSubSystem(SDL_INIT_EVENTS); ErrorCode != NULL) {

	}
	
	Game::Network::Init();
	Game::Lua::Init();

	std::chrono::high_resolution_clock::time_point Start;
	std::chrono::milliseconds Delta;
	while (true) {
		Start = std::chrono::high_resolution_clock::now();

		Game::Update();
		Game::Lua::Update();
		Game::Network::Update();

		Delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - Start);

		if (Delta > std::chrono::milliseconds(0) && Delta < std::chrono::milliseconds(16)) {
			std::this_thread::sleep_for(Delta);
		}
	}

	return -1;
}