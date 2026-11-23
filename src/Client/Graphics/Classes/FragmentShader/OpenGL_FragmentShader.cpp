#include "OpenGL_FragmentShader.hpp"

bool Game::Graphics::Classes::FragmentShader_OpenGL::SetShaderSource(
    const char* Source) {
    /*
    std::string SourceString = Source;
    Uint16 CurrentOutputBinding = 0;
    size_t FragmentOutputIndex, OutputNameIndex;
    while (true) {
            FragmentOutputIndex = SourceString.find("out",
    FragmentOutputIndex); if (FragmentOutputIndex == SIZE_MAX) { break;
            }

            if
    (!StringHelper::IsWhitespace(SourceString[FragmentOutputIndex - 1])
    || !StringHelper::IsWhitespace(SourceString[FragmentOutputIndex +
    3])) {

            }

            OutputNameIndex = SourceString.find(';',
    FragmentOutputIndex); if (OutputNameIndex == SIZE_MAX) { std::cerr
    << "Missing ';' to close 'out'" << std::endl; return false;
            }

            --OutputNameIndex;
            for (; SourceString[OutputNameIndex] == '	' ||
    SourceString[OutputNameIndex] == ' '; --OutputNameIndex) {} for (;
    SourceString[OutputNameIndex] != '	' ||
    SourceString[OutputNameIndex] != ' '; --OutputNameIndex) {}

            //SourceString.replace
    }

    std::cout << SourceString << std::endl;*/

    Game::Graphics::OpenGLFunctions::glShaderSource(this->GLObject, 1, &Source,
                                                    NULL);
    return true;
}

bool Game::Graphics::Classes::FragmentShader_OpenGL::Compile() {
    Game::Graphics::OpenGLFunctions::glCompileShader(this->GLObject);
    return true;
}