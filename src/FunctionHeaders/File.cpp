#include "File.hpp"

#include <string>

namespace Utils::File {
    std::string PreferredPath;
}

char* Utils::File::GetPathInAssetsChar(const char* Path, size_t PathLen) {
#ifdef SDL_PLATFORM_VITA
    constexpr static char Prefix[] = "app0:";
    char* NewPath = new char[sizeof(Prefix) + PathLen];
    memcpy(NewPath, Prefix, sizeof(Prefix) - 1);
    memcpy(NewPath + sizeof(Prefix), Path, PathLen);
    NewPath[PathLen + sizeof(Prefix)] = '\0';
#else
    char* NewPath = new char[PathLen + 1];
    memcpy(NewPath, Path, PathLen);
    NewPath[PathLen] = '\0';
#endif
    return NewPath;
}

char* Utils::File::GetPathInAssetsChar(const char* Path) {
    return Utils::File::GetPathInAssetsChar(Path, strlen(Path));
}

char* Utils::File::GetPathInDataChar(const char* Path, size_t PathLen) {
    const size_t PreferredPathSize = Utils::File::PreferredPath.size();
    char* NewPath = new char[PreferredPathSize + PathLen + 1];
    memcpy(NewPath, Utils::File::PreferredPath.c_str(), PreferredPathSize - 1);
    memcpy(NewPath + PreferredPathSize, Path, PathLen);
    NewPath[PathLen + PreferredPathSize] = '\0';
    return NewPath;
}

char* Utils::File::GetPathInDataChar(const char* Path) {
    return Utils::File::GetPathInDataChar(Path, strlen(Path));
}