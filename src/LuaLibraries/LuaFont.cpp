#include "LuaFont.hpp"

#include <SDL3/SDL.h>

#include <SDL3/SDL_gpu.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <SDL3_ttf/SDL_ttf.h>

void Game::Lua::CLibraries::Font::Init() {
    SDL_GPUDevice* GPU =
        SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, NULL);

    // SDL_CreateText
    TTF_TextEngine* TextEngine = TTF_CreateGPUTextEngine(GPU);
}