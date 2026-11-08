#include "LuaHelper.hpp"

#include "../define.h"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_stdinc.h>

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

#include "../../include/Compiler/luacode.h"

#include "File.hpp"

const char* const _excludedFastcalls[] = {"print"};

// TODO: reminder to change this to non debugging settings
static lua_CompileOptions CompileOptions = {
    0, 2, 0, 2,

    // alternative global builtin to construct vectors, in addition to
    // default builtin 'vector.create'
    "Vector3", "Vector3.new",

    // alternative vector type name for type tables, in addition to default
    // type 'vector'
    "Vector3",

    // 0 - 32-bit float vector components
    // 1 - 64-bit double vector components
    // CompileOptions.vectorPrecision = 0;

    // null-terminated array of globals that are mutable; disables the
    // import optimization for fields accessed through these
    NULL,

    // null-terminated array of userdata types that will be included in the
    // type information
    NULL,

    // null-terminated array of globals which act as libraries and have
    // members with known type and/or constant value when an import of one
    // of these libraries is accessed, callbacks below will be called to
    // receive that information
    NULL, NULL, NULL,

    // null-terminated array of library functions that should not be
    // compiled into a built-in fastcall ("name" "lib.name")
    _excludedFastcalls};

static int __readonly_metatable_newindex(lua_State* State) {
    luaL_error(State, "Cannot modify table because it is read-only.");
    return 0;
}

lua_Status LuaHelper::CompileToLuaFunction(lua_State* State,
                                           const char* SourcePath) {
    static constexpr const char BytecodeCacheHeader[] = "bYTEcODEcACHE";
    struct BytecodeCacheInfoStruct {
        char Header[sizeof(BytecodeCacheHeader)];
        Uint32 SourceHash;
        size_t SourceSize;
    };

    SDL_PathInfo PathInfo;

    Uint32 SourceCodeCRC;
    size_t SourceCodeSize;

    char* Bytecode;
    size_t BytecodeSize;
    char* SourceCode =
        static_cast<char*>(SDL_LoadFile(SourcePath, &SourceCodeSize));
    if (SourceCode == NULL) {
        return LUA_ERRERR;
    }

    std::filesystem::path BytecodeCachePath;

    // load cache file
    if (true) {  //(Game::Lua::CacheLuaBytecode) {

        std::ifstream BytecodeCacheIO;

        BytecodeCacheInfoStruct BytecodeCacheInfo;
        SourceCodeCRC = SDL_murmur3_32(SourceCode, SourceCodeSize, 420);

        BytecodeCachePath = Utils::File::PreferredPath + "LuaCache";

        // check if LuaCache directory exists and create it if it doesn't
        if (!std::filesystem::is_directory(BytecodeCachePath)) {
            std::filesystem::create_directory(BytecodeCachePath);
            BytecodeCachePath.append(SourcePath);
            goto CheckForCacheBytecodeFail;
        }
        BytecodeCachePath.append(SourcePath);

        // add source file parent directory to cache if it doesn't exist
        if (const std::filesystem::path SourcePathDirectory =
                BytecodeCachePath.parent_path();
            !std::filesystem::is_directory(SourcePathDirectory)) {
            std::filesystem::create_directory(SourcePathDirectory);
            goto CheckForCacheBytecodeFail;
        }

        // check for cache file
        if (!std::filesystem::exists(BytecodeCachePath)) {
            std::clog << "No " << BytecodeCachePath << " cache file."
                      << std::endl;
            goto CheckForCacheBytecodeFail;
        }

        // now load cache
        BytecodeCacheIO.open(BytecodeCachePath);
        if (!BytecodeCacheIO.is_open()) {
            std::clog << "Failed to open cache file for reading: "
                      << BytecodeCachePath << std::endl;
            goto CheckForCacheBytecodeFail;
        }

        BytecodeCacheIO.read((char*)&BytecodeCacheInfo,
                             sizeof(BytecodeCacheInfoStruct));

        if (SourceCodeCRC != BytecodeCacheInfo.SourceHash ||
            SourceCodeSize != BytecodeCacheInfo.SourceSize) {
            std::clog << "Hash or source size doesn't match." << std::endl;
            goto CheckForCacheBytecodeFail;
        }

        if (strncmp(BytecodeCacheInfo.Header, BytecodeCacheHeader,
                    sizeof(BytecodeCacheHeader)) != 0) {
            std::clog << "Mismatched header." << std::endl;
            goto CheckForCacheBytecodeFail;
        }

        SDL_free(SourceCode);

        BytecodeCacheIO.seekg(0, std::ios::end);
        BytecodeSize =
            BytecodeCacheIO.tellg() -
            static_cast<std::streamsize>(sizeof(BytecodeCacheInfoStruct));
        BytecodeCacheIO.seekg(sizeof(BytecodeCacheInfoStruct), std::ios::beg);

        Bytecode = new char[BytecodeSize];
        BytecodeCacheIO.read(Bytecode, BytecodeSize);

        luau_load(State, SourcePath, Bytecode, BytecodeSize, 0);

        delete[] Bytecode;

        std::cout << "loaded cache file" << std::endl;
        return LUA_OK;
    }
CheckForCacheBytecodeFail:

    Bytecode = luau_compile(SourceCode, SourceCodeSize, &CompileOptions,
                            &BytecodeSize);
    SDL_free(SourceCode);

    const lua_Status CompileStatus =
        (lua_Status)luau_load(State, SourcePath, Bytecode, BytecodeSize, 0);

    if (CompileStatus != LUA_OK) {
        free(Bytecode);
        return CompileStatus;
    }

    // write cache file
    if (true) {  //(Game::Lua::CacheLuaBytecode) {

        if (const std::filesystem::path ParentPath =
                BytecodeCachePath.parent_path();
            !std::filesystem::exists(ParentPath)) {
            std::filesystem::create_directory(ParentPath);
        }

        std::ofstream BytecodeCacheIO(BytecodeCachePath);
        if (!BytecodeCacheIO.is_open()) {
            std::clog << "Failed to open lua bytecode cache file: "
                      << BytecodeCachePath << " for writing." << std::endl;
            goto WriteCacheFileFail;
        }

        BytecodeCacheInfoStruct CacheInfo;
        memcpy(CacheInfo.Header, BytecodeCacheHeader,
               sizeof(BytecodeCacheHeader));
        CacheInfo.SourceHash = SourceCodeCRC;
        CacheInfo.SourceSize = SourceCodeSize;

        BytecodeCacheIO.write((char*)&CacheInfo,
                              sizeof(BytecodeCacheInfoStruct));

        BytecodeCacheIO.seekp(sizeof(BytecodeCacheInfoStruct), std::ios::beg);

        BytecodeCacheIO.write(Bytecode, BytecodeSize);
    }
    WriteCacheFileFail:
    free(Bytecode);
    return LUA_OK;
}

lua_Status LuaHelper::PCallLog(lua_State* State,
                               int ArgumentsPassed,
                               int ReturnValues) {
    const lua_Status ErrorCode =
        (lua_Status)lua_pcall(State, ArgumentsPassed, ReturnValues, 0);

    const char* ErrorTypeString;
    switch (ErrorCode) {
    likely_branch case lua_Status::LUA_OK:
        return LUA_OK;

    case lua_Status::LUA_ERRRUN:  // runtime error
        ErrorTypeString = "runtime";
        break;

        case lua_Status::LUA_ERRMEM:  // memory allocation error. For such
                                      // errors, Lua does not call the message
                                      // handler.
            ErrorTypeString = "memory allocation";
            break;

        case lua_Status::LUA_ERRERR:  // error while running the message
                                      // handler.
            ErrorTypeString = "message handler";
            break;

        // TODO: handle
        case lua_Status::LUA_BREAK:
            break;

        // TODO: handle
        case lua_Status::LUA_YIELD:
            break;

        // should never be executed, here just to shush a warning about not
        // handling every case.
        case lua_Status::LUA_ERRSYNTAX:
            return ErrorCode;
    }

    std::clog << "Caught a " << ErrorTypeString << " error from PCall.\n";

    if (lua_type(State, -1) != LUA_TSTRING) {
        std::clog << "PCall error object has type: "
                  << luaL_typename(State, -1);
    } else {
        std::clog << lua_tostring(State, -1);
    }
    std::clog << '\n' << std::endl;
    return ErrorCode;
}

lua_Status LuaHelper::LoadFileLog(lua_State* State, const char* Path) {
    const lua_Status ErrorCode = LuaHelper::CompileToLuaFunction(
        State, Path);  // luaL_loadfile(State, Path);

    switch (ErrorCode) {
        case LUA_OK:
            return LUA_OK;
        case LUA_BREAK:
            std::cerr << "Encountered a debug break while compiling '" << Path
                      << "'. (what the hell?)";
            break;
        case LUA_ERRSYNTAX:
            std::cerr << "Syntax error whilst compiling '" << Path << "'.";
            break;
        case LUA_YIELD:
            std::cerr << "Yielded while compiling '" << Path
                      << "'. (what the hell?)\nkilling process bc something "
                         "bad might've happened";
            exit(EXIT_FAILURE);
    }
    std::cerr << '\n'
              << "ErrorString: " << lua_tostring(State, -1) << std::endl;
    lua_pop(State, 1);
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
lua_Status LuaHelper::CallFileLog(lua_State* State,
                                  const char* Path,
                                  int ReturnValues) {
    const lua_Status Error = LuaHelper::LoadFileLog(State, Path);

    likely_branch if (Error == LUA_OK) {
        lua_call(State, 0, ReturnValues);
    }
    return Error;
}

// Loads a lua file and runs it with a pcall, automatically printing any caught
// Lua errors.
lua_Status LuaHelper::PCallFileLog(lua_State* State,
                                   const char* Path,
                                   int ReturnValues) {
    lua_Status Error = LuaHelper::LoadFileLog(State, Path);

    likely_branch if (Error == LUA_OK) {
        return LuaHelper::PCallLog(State, 0, ReturnValues);
    }
    return Error;
}

void LuaHelper::PushClosure(lua_State* State,
                            lua_CFunction Closure,
                            const char* Name,
                            int Upvalues) {
    lua_pushcclosure(State, Closure, Name, Upvalues);
}

void LuaHelper::SetKeyClosure(lua_State* State,
                              int TableIndex,
                              lua_CFunction Closure,
                              const char* ClosureName,
                              const char* Key,
                              int Upvalues) {
    LuaHelper::PushClosure(State, Closure, ClosureName, Upvalues);
    lua_setfield(State, TableIndex, Key);
}

void LuaHelper::SetKeyClosure(lua_State* State,
                              int TableIndex,
                              lua_CFunction Closure,
                              const char* Key,
                              int Upvalues) {
    LuaHelper::PushClosure(State, Closure, Key, Upvalues);
    lua_setfield(State, TableIndex, Key);
}

void LuaHelper::SetIndexClosure(lua_State* State,
                                int TableIndex,
                                lua_CFunction Closure,
                                const char* ClosureName,
                                lua_Integer Index,
                                int Upvalues) {
    lua_pushinteger(State, Index);
    LuaHelper::PushClosure(State, Closure, ClosureName, Upvalues);
    lua_settable(State, TableIndex);
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

void LuaHelper::StackTableReference::PushReference(lua_State* State) {
    lua_pushvalue(State, this->TableIndex);
}

void LuaHelper::StackTableReference::SetKeyClosure(lua_State* State,
                                                   lua_CFunction Closure,
                                                   const char* ClosureName,
                                                   const char* Key,
                                                   int Upvalues) {
    LuaHelper::SetKeyClosure(State, this->TableIndex, Closure, ClosureName, Key,
                             Upvalues);
}

void LuaHelper::StackTableReference::SetKeyClosure(lua_State* State,
                                                   lua_CFunction Closure,
                                                   const char* Key,
                                                   int Upvalues) {
    LuaHelper::SetKeyClosure(State, this->TableIndex, Closure, Key, Key,
                             Upvalues);
}

void LuaHelper::StackTableReference::SetIndexClosure(lua_State* State,
                                                     lua_CFunction Closure,
                                                     const char* ClosureName,
                                                     lua_Integer Index,
                                                     int Upvalues) {
    LuaHelper::SetIndexClosure(State, this->TableIndex, Closure, ClosureName,
                               Index, Upvalues);
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

    luaL_typeerrorL(State, UserdataIndex, lua_tostring(State, -1));
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

void* LuaHelper::TestMetatable(lua_State* State,
                               int UserdataIndex,
                               const char* MetatableName) {
    const int InitialStackTop = lua_gettop(State);
    void* ReturnValue;

    lua_getmetatable(State, UserdataIndex);
    luaL_getmetatable(State, MetatableName);

    if (lua_equal(State, -1, -2)) {
        ReturnValue = lua_touserdata(State, UserdataIndex);
    } else {
        ReturnValue = NULL;
    }
    lua_settop(State, InitialStackTop);
    return ReturnValue;
}

void LuaHelper::LockTable(lua_State* State, int TableIndex) {
    lua_createtable(State, 0, 4);

    if (lua_getmetatable(State, TableIndex)) {
        lua_pushvalue(State, -1);
        lua_setfield(State, -3, "__metatable");

        lua_setmetatable(State, -2);
    }

    LuaHelper::SetKeyClosure(State, -2, ::__readonly_metatable_newindex,
                             "readonly_metatable_error_yeller", "__newindex");

    lua_setmetatable(State, lua_absindex(State, TableIndex));
}