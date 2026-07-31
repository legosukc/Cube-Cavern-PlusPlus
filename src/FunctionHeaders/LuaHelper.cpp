
#include "LuaHelper.hpp"

#include <cstdlib>
#include <iostream>

#include "File.hpp"

static int __readonly_metatable_newindex(lua_State* State) {
    luaL_error(State, "Cannot modify table because it is read-only.");
    return 0;
}

int LuaHelper::CompileToLuaFunction(lua_State* State, const char* SourcePath) {
    struct BytecodeCacheStruct {
        char Header[14] = "BYTECODECACHE";
        Uint32 SourceCRC;
        size_t SourceSize;
        char Bytecode[];
    };

    SDL_PathInfo PathInfo;

    Uint32 SourceCodeCRC;
    size_t SourceCodeSize;
    char* SourceCode =
        static_cast<char*>(SDL_LoadFile(SourcePath, &SourceCodeSize));
    if (SourceCode == NULL) {
        return LUA_ERRFILE;
    }
    /*
    bool DoCache;
    {
            if (Game::Lua::CacheLuaBytecode) {
                    SDL_GetPathInfo((Utils::File::PreferredPath +
    "LuaCache\\").c_str(), &PathInfo);

            }
    }*/

    std::string BytecodeCachePath;
    // TODO: fix this
    if (true) { //(Game::Lua::CacheLuaBytecode) {
        SourceCodeCRC = SDL_murmur3_32(SourceCode, SourceCodeSize, 420);

        BytecodeCachePath = Utils::File::PreferredPath;
        BytecodeCachePath.append("LuaCache\\");

        if (!SDL_GetPathInfo(BytecodeCachePath.c_str(), &PathInfo) ||
            PathInfo.type != SDL_PathType::SDL_PATHTYPE_DIRECTORY) {
            SDL_CreateDirectory(BytecodeCachePath.c_str());
            goto CheckForCacheBytecodeFail;
        }

        BytecodeCachePath.append(SourcePath);

        char* TempPath = NULL;
        size_t PathStringSize = BytecodeCachePath.size();
        for (; PathStringSize > 0; --PathStringSize) {
            if (TempPath[PathStringSize] == '\\') {
                TempPath = new char[PathStringSize + 1];
                break;
            }
        }

        if (TempPath == NULL) {
            goto CheckForCacheBytecodeFail;
        }
        memcpy(TempPath, BytecodeCachePath.c_str(), PathStringSize - 1);
        TempPath[PathStringSize] = '\0';
        /*
        TempPath =
        std::filesystem::path(BytecodeCachePath).remove_filename().u8string();*/
        std::cout << TempPath << std::endl;
        if (!SDL_GetPathInfo(TempPath, &PathInfo) ||
            PathInfo.type != SDL_PathType::SDL_PATHTYPE_FILE) {
            SDL_CreateDirectory(TempPath);
            delete[] TempPath;
            goto CheckForCacheBytecodeFail;
        }
        delete[] TempPath;

        size_t BytecodeCacheSize;
        BytecodeCacheStruct* BytecodeCache = static_cast<BytecodeCacheStruct*>(
            SDL_LoadFile(SourcePath, &BytecodeCacheSize));

        if (BytecodeCache == NULL) {
            goto CheckForCacheBytecodeFail;
        }

        if (strcmp(BytecodeCache->Header, "BYTECODECACHE") != 0 ||
            SourceCodeCRC != BytecodeCache->SourceCRC) {
            goto CheckForCacheBytecodeFail_FreeBytecodeCache;
        }
        SDL_free(SourceCode);

        luaL_loadbuffer(State, BytecodeCache->Bytecode,
                        BytecodeCacheSize - sizeof(Uint32), SourcePath);
        SDL_free(BytecodeCache);

        return LUA_OK;

    CheckForCacheBytecodeFail_FreeBytecodeCache:
        SDL_free(BytecodeCache);
    }
CheckForCacheBytecodeFail:

    // luaL_loadstring(State, SourceCode);

    // lua_getinfo
    const int CompileStatus = luaL_loadfile(State, SourcePath);
    if (CompileStatus != LUA_OK) {
        return CompileStatus;
    }

    SDL_free(SourceCode);

    // TODO: fix this
    if (true) {  //(Game::Lua::CacheLuaBytecode) {
        SDL_IOStream* BytecodeCacheIO =
            SDL_IOFromFile(BytecodeCachePath.c_str(), "wb");
        if (BytecodeCacheIO == NULL) {
            std::clog << "Failed to open lua bytecode cache file: "
                      << BytecodeCachePath << " for writing." << std::endl;
            return LUA_OK;
        }
        BytecodeCacheStruct CacheInfo;
        CacheInfo.SourceCRC = SourceCodeCRC;
        CacheInfo.SourceSize = SourceCodeSize;
        SDL_WriteIO(BytecodeCacheIO, &CacheInfo,
                    sizeof(CacheInfo.Header) + sizeof(CacheInfo.SourceCRC) +
                        sizeof(CacheInfo.SourceSize));
        SDL_SeekIO(BytecodeCacheIO,
                   sizeof(CacheInfo.Header) + sizeof(CacheInfo.SourceCRC) +
                       sizeof(CacheInfo.SourceSize),
                   SDL_IOWhence::SDL_IO_SEEK_SET);

        lua_dump(
            State,
            [](lua_State* State, const void* Chunk, size_t ChunkSize,
               void* BytecodeCacheIO) -> int {
                SDL_WriteIO(static_cast<SDL_IOStream*>(BytecodeCacheIO), Chunk,
                            ChunkSize);
                SDL_SeekIO(static_cast<SDL_IOStream*>(BytecodeCacheIO),
                           ChunkSize, SDL_IOWhence::SDL_IO_SEEK_CUR);
                return 0;
            },
            BytecodeCacheIO, false);

        SDL_CloseIO(BytecodeCacheIO);
    }

    return LUA_OK;
}

int LuaHelper::PCallLog(lua_State* State,
                        int ArgumentsPassed,
                        int ReturnValues) {
    const int ErrorCode = lua_pcall(State, ArgumentsPassed, ReturnValues, 0);

    likely_branch if (ErrorCode == LUA_OK) {
        return LUA_OK;
    }

    std::clog << "Caught a ";

    switch (ErrorCode) {
        case LUA_ERRRUN:  // runtime error
            std::clog << "runtime";
            break;

        case LUA_ERRMEM:  // memory allocation error. For such errors, Lua does
                          // not call the message handler.
            std::clog << "memory allocation";
            break;

        case LUA_ERRERR:  // error while running the message handler.
            std::clog << "message handler";
            break;
            // case LUA_ERRGCMM: // error while running a __gc metamethod. For
            // such errors, Lua does not call the message handler (as this kind
            // of error typically has no relation with the function being
            // called).
    }

    std::clog << " error from PCall:\n" << lua_tostring(State, -1) << std::endl;
    return ErrorCode;
}

int LuaHelper::LoadFileLog(lua_State* State, const char* Path) {
    const int ErrorCode = LuaHelper::CompileToLuaFunction(
        State, Path);  // luaL_loadfile(State, Path);
    if (ErrorCode != LUA_OK) {
        std::cerr << lua_tostring(State, -1) << std::endl;
        lua_pop(State, 1);
    }
    return ErrorCode;

    /*
    const int ErrorCode = luaL_loadfile(State, Path);
    switch (ErrorCode) {

            likely_branch
    case LUA_OK:
            return LUA_OK;

    case LUA_ERRSYNTAX:
    default:
            std::cerr << lua_tostring(State, -1) << std::endl;
    }

    return ErrorCode;*/
}

// Loads a lua file and runs it unprotected. Only automatically prints any
// caught Lua errors from file compilation.
int LuaHelper::CallFileLog(lua_State* State,
                           const char* Path,
                           int ReturnValues) {
    int Error = LuaHelper::LoadFileLog(State, Path);

    likely_branch if (Error == LUA_OK) {
        lua_call(State, 0, ReturnValues);
    }
    return Error;
}

// Loads a lua file and runs it with a pcall, automatically printing any caught
// Lua errors.
int LuaHelper::PCallFileLog(lua_State* State,
                            const char* Path,
                            int ReturnValues) {
    int Error = LuaHelper::LoadFileLog(State, Path);

    likely_branch if (Error == LUA_OK) {
        Error = LuaHelper::PCallLog(State, 0, ReturnValues);
    }
    return Error;
}

template <typename T>
constexpr void LuaHelper::Push(lua_State* State, T Value) {
    if CONSTEXPR_IF (std::is_same_v<std::remove_cv_t<T>, bool>) {
        lua_pushboolean(State, static_cast<int>(Value));
    } else if CONSTEXPR_IF (std::is_integral_v<T>) {
        lua_pushinteger(State, static_cast<lua_Integer>(Value));
    } else if CONSTEXPR_IF (std::is_floating_point_v<T>) {
        lua_pushnumber(State, static_cast<lua_Number>(Value));
    } else if CONSTEXPR_IF (std::is_function_v<T>) {
        static_assert(std::is_same_v<std::remove_cv_t<T>, lua_CFunction>,
                      "Template parameter 'T' is a function type that doesn't "
                      "match function type 'lua_CFunction'.");
        lua_pushcfunction(State, Value);
    } else if CONSTEXPR_IF (std::is_pointer_v<T>) {
        lua_pushlightuserdata(State, reinterpret_cast<void*>(Value));
    }
}

template <typename T>
constexpr T LuaHelper::ToTypename(lua_State* State, int StackIndex) {
    if CONSTEXPR_IF (std::is_same_v<std::remove_cv_t<T>, bool>) {
        return static_cast<T>(lua_toboolean(State, StackIndex));
    } else if CONSTEXPR_IF (std::is_integral_v<T>) {
        return static_cast<T>(lua_tointeger(State, StackIndex));
    } else if CONSTEXPR_IF (std::is_floating_point_v<T>) {
        return static_cast<T>(lua_tonumber(State, StackIndex));
    } else if CONSTEXPR_IF (std::is_function_v<T>) {
        static_assert(std::is_same_v<std::remove_cv_t<T>, lua_CFunction>,
                      "Template parameter 'T' is a function type that doesn't "
                      "match function type 'lua_CFunction'.");
        return lua_tocfunction(State, StackIndex);
    } else if CONSTEXPR_IF (std::is_pointer_v<T>) {
        return static_cast<T>(lua_touserdata(State, StackIndex));
    }
}

template <typename UserdataType>
UserdataType& LuaHelper::GetUserdata(lua_State* State, int StackIndex) {
    return *static_cast<UserdataType*>(lua_touserdata(State, StackIndex));
}

void LuaHelper::PushClosure(lua_State* State,
                            lua_CFunction Closure,
                            int Upvalues) {
    lua_pushcclosure(State, Closure, Upvalues);
}

template <typename Value_T>
void LuaHelper::SetGlobal(lua_State* State, Value_T Value, const char* Name) {
    LuaHelper::Push<Value_T>(State, Value);
    lua_setglobal(State, Name);
}

template <typename Value_T, typename Index_T>
void LuaHelper::SetTable(lua_State* State,
                         int TableIndex,
                         Value_T Value,
                         Index_T Index) {
    LuaHelper::Push<Index_T>(State, Index);
    LuaHelper::Push<Value_T>(State, Value);
    lua_settable(State, TableIndex);
}

template <typename Value_T>
void LuaHelper::SetKey(lua_State* State,
                       int TableIndex,
                       Value_T Value,
                       const char* Key) {
    LuaHelper::Push<Value_T>(State, Value);
    lua_setfield(State, TableIndex, Key);
}

template <typename Value_T>
void LuaHelper::SetIndex(lua_State* State,
                         int TableIndex,
                         Value_T Value,
                         lua_Integer Index) {
    LuaHelper::Push<Value_T>(State, Value);
    lua_seti(State, TableIndex, Index);
}

template <typename Index_T>
void LuaHelper::SetTableClosure(lua_State* State,
                                int TableIndex,
                                lua_CFunction Closure,
                                Index_T Index,
                                int Upvalues) {
    LuaHelper::PushClosure(State, Closure, Upvalues);
    lua_settable(State, TableIndex);
}

void LuaHelper::SetKeyClosure(lua_State* State,
                              int TableIndex,
                              lua_CFunction Closure,
                              const char* Key,
                              int Upvalues) {
    LuaHelper::PushClosure(State, Closure, Upvalues);
    lua_setfield(State, TableIndex, Key);
}

void LuaHelper::SetIndexClosure(lua_State* State,
                                int TableIndex,
                                lua_CFunction Closure,
                                lua_Integer Index,
                                int Upvalues) {
    LuaHelper::PushClosure(State, Closure, Upvalues);
    lua_seti(State, TableIndex, Index);
}

// Uses the top of the passed States' stack as the table reference
// (TableIndex).
LuaHelper::StackTableReference::StackTableReference(lua_State* State,
                                                    const char* MetatableName) {
    luaL_newmetatable(State, MetatableName);
    this->TableIndex = lua_gettop(State);
}

// Creates a new table and sets the table reference (TableIndex) to the top of
// the stack.
LuaHelper::StackTableReference::StackTableReference(lua_State* State,
                                                    int AllocateArrays,
                                                    int AllocateKeys) {
    lua_createtable(State, AllocateArrays, AllocateKeys);
    this->TableIndex = lua_gettop(State);
}

// Uses the passed 'Index' parameter as the table reference (TableIndex).
constexpr LuaHelper::StackTableReference::StackTableReference(
    int Index) noexcept
    : TableIndex(Index) {}
LuaHelper::StackTableReference::StackTableReference() noexcept = default;

constexpr int LuaHelper::StackTableReference::GetStackIndex() noexcept {
    return this->TableIndex;
}

void LuaHelper::StackTableReference::PushReference(lua_State* State) {
    lua_pushvalue(State, this->TableIndex);
}

template <typename Value_T, typename Index_T>
void LuaHelper::StackTableReference::SetTable(lua_State* State,
                                              Value_T Value,
                                              Index_T Index) {
    LuaHelper::SetTable<Value_T, Index_T>(State, this->TableIndex, Value,
                                          Index);
}

template <typename Value_T>
void LuaHelper::StackTableReference::SetKey(lua_State* State,
                                            Value_T Value,
                                            const char* Key) {
    LuaHelper::SetKey<Value_T>(State, this->TableIndex, Value, Key);
}

template <typename Value_T>
void LuaHelper::StackTableReference::SetIndex(lua_State* State,
                                              Value_T Value,
                                              lua_Integer Index) {
    LuaHelper::SetIndex<Value_T>(State, this->TableIndex, Value, Index);
}

template <typename Index_T>
void LuaHelper::StackTableReference::SetTableClosure(lua_State* State,
                                                     lua_CFunction Closure,
                                                     Index_T Index,
                                                     int Upvalues) {
    LuaHelper::SetTableClosure<Index_T>(State, this->TableIndex, Closure, Index,
                                        Upvalues);
}

void LuaHelper::StackTableReference::SetKeyClosure(lua_State* State,
                                                   lua_CFunction Closure,
                                                   const char* Key,
                                                   int Upvalues) {
    LuaHelper::SetKeyClosure(State, this->TableIndex, Closure, Key, Upvalues);
}

void LuaHelper::StackTableReference::SetIndexClosure(lua_State* State,
                                                     lua_CFunction Closure,
                                                     lua_Integer Index,
                                                     int Upvalues) {
    LuaHelper::SetIndexClosure(State, this->TableIndex, Closure, Index,
                               Upvalues);
}

void LuaHelper::StackTableReference::PushKey(lua_State* State,
                                             const char* Key) const {
    lua_getfield(State, this->TableIndex, Key);
}

void* LuaHelper::CheckMetatable(lua_State* State,
                                int UserdataIndex,
                                int MetatableIndex) {
    const int StackTop = lua_gettop(State);

    unlikely_branch if (lua_getmetatable(State, UserdataIndex) == 0) {
        goto Fail;
    }

    unlikely_branch if (lua_rawequal(State, -1, MetatableIndex) == 0) {
        goto Fail;
    }

    lua_settop(State, StackTop);

    return lua_touserdata(State, UserdataIndex);

Fail:
    lua_getfield(State, MetatableIndex, "__name");
    luaL_typeerror(State, UserdataIndex, lua_tostring(State, -1));
    return NULL;
}

void* LuaHelper::TestMetatable(lua_State* State,
                               int UserdataIndex,
                               int MetatableIndex) {
    void* Userdata;
    int StartingStack;

    StartingStack = lua_gettop(State);

    unlikely_branch if (!lua_getmetatable(State, UserdataIndex) ||
                        !lua_rawequal(State, -1, MetatableIndex)) {
        Userdata = NULL;
    }
    else {
        Userdata = lua_touserdata(State, UserdataIndex);
    }
    lua_settop(State, StartingStack);

    return Userdata;
}

void LuaHelper::LockTable(lua_State* State, int TableIndex) {
    lua_createtable(State, 0, 4);

    if (lua_getmetatable(State, TableIndex)) {
        lua_pushvalue(State, -1);
        lua_setfield(State, -3, "__metatable");

        lua_setmetatable(State, -2);
    }

    LuaHelper::SetKeyClosure(State, -2, ::__readonly_metatable_newindex,
                             "__newindex");

    lua_setmetatable(State, TableIndex);
}