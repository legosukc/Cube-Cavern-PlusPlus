#define SDL_MAIN_USE_CALLBACKS 1

#include "../define.h"

#include "../MathClasses/Vector2.hpp"
#include "../MathClasses/Vector3.hpp"
#include "../MathClasses/Vector4.hpp"
#include "../MathClasses/Matrix.hpp"

#include <iostream>

#include <string>

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>

#ifdef SDL_PLATFORM_VITA
#include <psp2/kernel/processmgr.h>
#include <vitaGL.h>
#include <vitasdk.h>
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_opengles2_gl2ext.h>
#endif

//#include "../Collisions.hpp"


#include "../FunctionHeaders/ConfigHandler.hpp"
#include "../FunctionHeaders/File.hpp"

#include "../pch.h"

#include "SDLClasses/Window.hpp"

#include "../Game.hpp"

#include "Graphics_Client.hpp"
#include "Network_Client.hpp"

#ifndef SDL_PLATFORM_VITA
#include "Sound/Sound.hpp"
#endif

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

        #ifndef SDL_PLATFORM_VITA
        Game::Sound::Destroy();
        #endif

        Game::Window.Destroy();
        SDL_Quit();

    } catch (const Exceptions::Exception& Exception) {
        std::clog << "Caught a unhandled exception while closing libraries. "
                     "Exception message:\n"
                  << Exception.what() << std::endl;
    } catch (...) {
        std::clog
            << "Caught a unknown unhandled exception while closing libraries."
            << std::endl;
    }
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
#ifndef DEBUG_BUILD
    try
#endif
{

#ifdef SDL_PLATFORM_VITA

    //vglInitExtended(0, 960, 544, 0x1800000, SCE_GXM_MULTISAMPLE_NONE);

    //SDL_SetHint(SDL_HINT_VITA_ENABLE_BACK_TOUCH, "0");

    //SDL_SetHint(SDL_HINT_VITA_PVR_INIT, "1");
    //SDL_SetHint(SDL_HINT_VITA_RESOLUTION, "720");
    //SDL_SetHint(SDL_HINT_VITA_PVR_OPENGL, "1");
    if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_AUDIO |
                  SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK)) {
        Exceptions::ThrowSDLError("Failed to initalize SDL.");
    }
#else
if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO |
                  SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK)) {
        Exceptions::ThrowSDLError("Failed to initalize SDL.");
    }
#endif

    Utils::File::Init("DribbleCo", "CubeCavernPlus");

    Game::Config::Init();
#ifdef BUILD_CLIENT
    Game::Config::OpenConfig("ClientCfg.json", "defaultClientCfg.json");
#else
    Game::Config::OpenConfig("ServerCfg.json", "defaultServerCfg.json");
#endif

#ifndef SDL_PLATFORM_VITA
    Game::Sound::Init();
#endif

    Game::Network::Init();

    Game::Window.Create("Cube Cavern++", 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN);
    SDL_Log("created window ya slut");

#ifdef SDL_PLATFORM_VITA
    sceKernelExitProcess(-1);
#endif

    Game::Graphics::Init();
    Game::Lua::Init();

    // Game::Sound::AudioSource* AudioSource =
    // Game::Sound::CreateAudioSource(); Game::Sound::Speaker* Speaker =
    // Game::Sound::CreateSpeaker();

    // AudioSource->LoadFile("SFX/Kick.ogg");
    // Speaker->SetAudioSource(AudioSource);
    // Speaker->Play();

    return SDL_APP_CONTINUE;
}
#ifndef DEBUG_BUILD
catch (const Exceptions::Exception& Exception) {

    std::cerr << "::FATAL:: ::UNHANDLED EXCEPTION:: " << Exception.what()
              << std::endl;
    ::_CloseInitialisedItems();

    if (!SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR, "Unhandled Exception",

            StringHelper::Combine(
                "An unhandled exception occurred, causing the program to "
                "shit "
                "itself. We're NOT sorry!\nException message:\n",
                Exception.what())
                .c_str(),

            NULL)) {
        std::cerr << "An error occurred when calling "
                     "'SDL_ShowSimpleMessageBox'.\nSDL Error: "
                  << SDL_GetError() << std::endl;
    }

    return SDL_APP_FAILURE;

} catch (...) {
    std::cerr << "::FATAL:: ::UNHANDLED UNKNOWN EXCEPTION::" << std::endl;
    ::_CloseInitialisedItems();

    if (!SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                  "Unhandled Unknown Exception",
                                  "An unhandled unknown exception "
                                  "occurred, causing the program to "
                                  "shit itself. We're NOT sorry!",
                                  NULL)) {
        std::cerr << "An error occurred when calling "
                     "'SDL_ShowSimpleMessageBox'.\nSDL Error: "
                  << SDL_GetError() << std::endl;
    }

    return SDL_APP_FAILURE;
}
#endif

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* Event) {

    Game::Window.HandleEvent(*Event);

    switch (Event->type) {
        case SDL_EventType::SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;

        case SDL_EventType::SDL_EVENT_KEY_DOWN:

            // make sure to remove laterrr
            // exists so i don't accidentally alt-f4 vscode
            // elegant way of exitting (makes sure everything is cleaned up)
            if (Event->key.scancode == SDL_SCANCODE_ESCAPE) {
                return SDL_APP_SUCCESS;
            }
            break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    Game::Update();

    Game::Draw();

    //Game::Window.MouseDelta = Math::Vector2(0.f);
    Game::Window.Present();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    ::_CloseInitialisedItems();

    #ifdef SDL_PLATFORM_VITA
    sceKernelExitProcess(-1);
    #endif
}