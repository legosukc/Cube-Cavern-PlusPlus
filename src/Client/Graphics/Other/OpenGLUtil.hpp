#pragma once

namespace Game::Graphics::OpenGL::Util {
    bool ExtensionSupported(const char* Name);
    void (*GetProcAddress(const char* FunctionName))();
}