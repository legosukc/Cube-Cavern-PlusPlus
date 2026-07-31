#define SDL_MAIN_HANDLED

#include "../define.h"

#include <iostream>

#include <string>

#ifdef USE_SIMD_INTRINSICS
#include <xmmintrin.h>
#include <pmmintrin.h>
#include <smmintrin.h>
#include <mmintrin.h>
#endif
/*
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>*/

#include <SDL3/SDL.h>
//#include <SDL3_net/SDL_net.h>
//#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3/SDL_main.h>

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_opengles2_gl2ext.h>


#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>


#include "../FunctionHeaders/File.hpp"
#include "../FunctionHeaders/ConfigHandler.hpp"

#include "../pch.h"


#include "SDLClasses/Window.hpp"

#include "../Game.hpp"

#include "Sound/Sound.hpp"
#include "Network_Client.hpp"
#include "Graphics_Client.hpp"

#include "../Lua.hpp"

void Game::Update() {
    Game::Statistics::Update();

    Game::Lua::Update();
}

void Game::Draw() {
    Game::Graphics::SetClearColor(0.f, 0.f, 0.f, 1.f);
    Game::Graphics::ClearBitfields(Game::Graphics::BufferBitfields.ColorBit |
                                   Game::Graphics::BufferBitfields.DepthBit |
                                   Game::Graphics::BufferBitfields.StencilBit);

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

#ifdef SDL_PLATFORM_VITA

	SDL_SetHint(SDL_HINT_VITA_ENABLE_BACK_TOUCH, "0");

	SDL_SetHint(SDL_HINT_VITA_PVR_OPENGL, "1");
	SDL_SetHint(SDL_HINT_VITA_RESOLUTION, "720");
#endif

	if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK)) {
		Exceptions::ThrowSDLError("Failed to initalize SDL.");
	}
	
	Utils::File::Init("DribbleCo", "CubeCavernPlus");

	Game::Config::Init();
#ifdef BUILD_CLIENT
	Game::Config::OpenConfig("ClientCfg.json", "defaultClientCfg.json");
#else
	Game::Config::OpenConfig("ServerCfg.json", "defaultServerCfg.json");
#endif

	Game::Sound::Init();
	Game::Network::Init();
	
	Game::Window.Create("Cube Cavern++", 800, 600);

	Game::Graphics::Init();
	Game::Lua::Init();

	Game::Sound::AudioSource* AudioSource = Game::Sound::CreateAudioSource();
	Game::Sound::Speaker* Speaker = Game::Sound::CreateSpeaker();

	AudioSource->LoadFile("SFX\\Kick.ogg");
	Speaker->SetAudioSource(AudioSource);
	Speaker->Play();

	while (true) {

		unlikely_branch
		if (Game::Window.PollEvents()) {
			break;
		}

		Game::Update();
		
		Game::Draw();

		Game::Window.Present();
	}

	delete AudioSource;
	delete Speaker;

	::_CloseInitialisedItems();

	return -1;
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