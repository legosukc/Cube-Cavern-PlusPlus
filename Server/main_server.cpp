#define SDL_MAIN_HANDLED

#define BUILD_SERVER

#include "../define.h"

#include <iostream>

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


namespace Game {
	double DeltaTime = 1.0;
}

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

	Uint64 Elasped, Start;
	while (true) {
		
		Start = SDL_GetTicksNS();

		Game::Lua::Update();
		Game::Network::SendPacket();

		Elasped = SDL_GetTicksNS() - Start;
		Game::DeltaTime = static_cast<double>(Elasped) / 1e+9;

		if (Elasped < 16 * 1000) {
			SDL_DelayNS(Elasped);
		}
	}

	return -1;
}