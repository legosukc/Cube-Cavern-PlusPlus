#ifndef FILE_HPP
#define FILE_HPP

#include <string>

namespace Utils::File {

    inline std::string PreferredPath;

    void Init(const char* OrganisationName, const char* AppName);
}

#endif