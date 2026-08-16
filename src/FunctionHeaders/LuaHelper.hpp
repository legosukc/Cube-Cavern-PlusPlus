#ifndef LUAHELPER_H
#define LUAHELPER_H

#include "../define.h"

#include <cstdlib>
#include <iostream>

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_iostream.h>

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

#include "../../include/Compiler/luacode.h"

#include "File.hpp"

namespace LuaHelper {

    lua_Status CompileToLuaFunction(lua_State* State, const char* SourcePath);

    lua_Status PCallLog(lua_State* State,
                        int ArgumentsPassed = 0,
                        int ReturnValues = 0);

    lua_Status LoadFileLog(lua_State* State, const char* Path);

    // Loads a lua file and runs it unprotected. Only automatically prints any
    // caught Lua errors from file compilation.
    lua_Status CallFileLog(lua_State* State,
                           const char* Path,
                           int ReturnValues = 1);

    // Loads a lua file and runs it with a pcall, automatically printing any
    // caught Lua errors.
    lua_Status PCallFileLog(lua_State* State,
                            const char* Path,
                            int ReturnValues = 1);

    template <typename T>
    constexpr void Push(lua_State* State, T Value) {
        if CONSTEXPR_IF (std::is_same_v<std::remove_cv_t<T>, bool>) {
            lua_pushboolean(State, static_cast<int>(Value));
        } else if CONSTEXPR_IF (std::is_integral_v<T>) {
            lua_pushinteger(State, static_cast<lua_Integer>(Value));
        } else if CONSTEXPR_IF (std::is_floating_point_v<T>) {
            lua_pushnumber(State, static_cast<lua_Number>(Value));
        } else if CONSTEXPR_IF (std::is_function_v<T>) {
            static_assert(
                std::is_same_v<std::remove_cv_t<T>, lua_CFunction>,
                "Template parameter 'T' is a function type that doesn't "
                "match function type 'lua_CFunction'.");
            lua_pushcfunction(State, Value, "rahh");
        } else if CONSTEXPR_IF (std::is_pointer_v<T>) {
            lua_pushlightuserdata(State, reinterpret_cast<void*>(Value));
        }
    }

    template <typename T>
    constexpr T ToTypename(lua_State* State, int StackIndex) {
        if CONSTEXPR_IF (std::is_same_v<std::remove_cv_t<T>, bool>) {
            return static_cast<T>(lua_toboolean(State, StackIndex));
        } else if CONSTEXPR_IF (std::is_integral_v<T>) {
            return static_cast<T>(lua_tointeger(State, StackIndex));
        } else if CONSTEXPR_IF (std::is_floating_point_v<T>) {
            return static_cast<T>(lua_tonumber(State, StackIndex));
        } else if CONSTEXPR_IF (std::is_function_v<T>) {
            static_assert(
                std::is_same_v<std::remove_cv_t<T>, lua_CFunction>,
                "Template parameter 'T' is a function type that doesn't "
                "match function type 'lua_CFunction'.");
            return lua_tocfunction(State, StackIndex);
        } else if CONSTEXPR_IF (std::is_pointer_v<T>) {
            return static_cast<T>(lua_touserdata(State, StackIndex));
        }
    }

    template <typename UserdataType>
    [[nodiscard]]
    inline UserdataType& GetUserdata(lua_State* State, int StackIndex) {
        return *static_cast<UserdataType*>(lua_touserdata(State, StackIndex));
    }

    inline void PushClosure(lua_State* State,
                            lua_CFunction Closure,
                            const char* Name,
                            int Upvalues = 0);

    template <typename Arg_T>
    constexpr void PushArgs(lua_State* State, Arg_T Value) {
        LuaHelper::Push<Arg_T>(State, Value);
    }

    template <typename Arg_T, typename... Args_T>
    constexpr void PushArgs(lua_State* State, Arg_T Arg, Args_T... Args) {
        LuaHelper::Push<Arg_T>(State, Arg);
        LuaHelper::PushArgs<Args_T...>(State, Args...);
    }

    template <typename Value_T>
    inline void SetGlobal(lua_State* State, Value_T Value, const char* Name) {
        LuaHelper::Push<Value_T>(State, Value);
        lua_setglobal(State, Name);
    }

    template <typename Value_T, typename Index_T>
    inline void SetTable(lua_State* State,
                         int TableIndex,
                         Value_T Value,
                         Index_T Index) {
        LuaHelper::Push<Index_T>(State, Index);
        LuaHelper::Push<Value_T>(State, Value);
        lua_settable(State, TableIndex);
    }

    template <typename Value_T>
    inline void SetKey(lua_State* State,
                       int TableIndex,
                       Value_T Value,
                       const char* Key) {
        LuaHelper::Push<Value_T>(State, Value);
        lua_setfield(State, TableIndex, Key);
    }

    template <typename Value_T>
    inline void SetIndex(lua_State* State,
                         int TableIndex,
                         Value_T Value,
                         lua_Integer Index) {
        lua_pushinteger(State, Index);
        LuaHelper::Push<Value_T>(State, Value);

        lua_settable(State, TableIndex);
    }

    template <typename GetType>
    [[nodiscard]]
    inline GetType GetGlobal(lua_State* State, const char* Name);

    template <typename GetType>
    [[nodiscard]]
    inline GetType GetTable(lua_State* State, int TableIndex);

    template <typename GetType>
    [[nodiscard]]
    inline GetType GetKey(lua_State* State, int TableIndex, const char* Key);

    template <typename GetType>
    [[nodiscard]]
    inline GetType GetIndex(lua_State* State,
                            int TableIndex,
                            lua_Integer Index);

    template <typename Index_T>
    inline void SetTableClosure(lua_State* State,
                                int TableIndex,
                                lua_CFunction Closure,
                                const char* ClosureName,
                                Index_T Index,
                                int Upvalues = 0) {
        LuaHelper::PushClosure(State, Closure, ClosureName, Upvalues);
        lua_settable(State, TableIndex);
    }

    inline void SetKeyClosure(lua_State* State,
                              int TableIndex,
                              lua_CFunction Closure,
                              const char* ClosureName,
                              const char* Key,
                              int Upvalues = 0);

    // Uses argument "Key" for the closure name.
    inline void SetKeyClosure(lua_State* State,
                              int TableIndex,
                              lua_CFunction Closure,
                              const char* Key,
                              int Upvalues = 0);

    inline void SetIndexClosure(lua_State* State,
                                int TableIndex,
                                lua_CFunction Closure,
                                const char* ClosureName,
                                lua_Integer Index,
                                int Upvalues = 0);

    class StackTableReference {
        int TableIndex;

       public:
        // Uses the top of the passed States' stack as the table reference
        // (TableIndex).
        inline StackTableReference(lua_State* State);

        // Uses the top of the passed States' stack as the table reference
        // (TableIndex).
        inline StackTableReference(lua_State* State, const char* MetatableName);

        // Creates a new table and sets the table reference (TableIndex) to the
        // top of the stack.
        inline StackTableReference(lua_State* State,
                                   int AllocateArrays,
                                   int AllocateKeys);

        // Uses the passed 'Index' parameter as the table reference
        // (TableIndex).
        constexpr StackTableReference(int Index) noexcept;
        inline StackTableReference() noexcept;

        [[nodiscard]]
        constexpr inline int GetStackIndex() noexcept;

        inline void PushReference(lua_State* State);

        template <typename Value_T, typename Index_T>
        inline void SetTable(lua_State* State, Value_T Value, Index_T Index) {
            LuaHelper::SetTable<Value_T, Index_T>(State, this->TableIndex,
                                                  Value, Index);
        }

        template <typename Value_T>
        inline void SetKey(lua_State* State, Value_T Value, const char* Key) {
            LuaHelper::SetKey<Value_T>(State, this->TableIndex, Value, Key);
        }

        template <typename Value_T>
        inline void SetIndex(lua_State* State,
                             Value_T Value,
                             lua_Integer Index) {
            LuaHelper::SetIndex<Value_T>(State, this->TableIndex, Value, Index);
        }

        template <typename Index_T>
        inline void SetTableClosure(lua_State* State,
                                    lua_CFunction Closure,
                                    const char* ClosureName,
                                    Index_T Index,
                                    int Upvalues = 0) {
            LuaHelper::SetTableClosure<Index_T>(State, this->TableIndex,
                                                Closure, Index, Upvalues);
        }

        inline void SetKeyClosure(lua_State* State,
                                  lua_CFunction Closure,
                                  const char* ClosureName,
                                  const char* Key,
                                  int Upvalues = 0);

        // Uses argument "Key" for the name of the closure.
        inline void SetKeyClosure(lua_State* State,
                                  lua_CFunction Closure,
                                  const char* Key,
                                  int Upvalues = 0);

        inline void SetIndexClosure(lua_State* State,
                                    lua_CFunction Closure,
                                    const char* ClosureName,
                                    lua_Integer Index,
                                    int Upvalues = 0);

        inline void PushKey(lua_State* State, const char* Key) const;
    };

    void* CheckMetatable(lua_State* State,
                         int UserdataIndex,
                         int MetatableIndex);

    [[nodiscard]]
    void* TestMetatable(lua_State* State,
                        int UserdataIndex,
                        int MetatableIndex);

    [[nodiscard]]
    void* TestMetatable(lua_State*,
                        int UserdataIndex,
                        const char* MetatableName);

    void LockTable(lua_State* State, int TableIndex);

}

static int __readonly_metatable_newindex(lua_State* State) {
    luaL_error(State, "Cannot modify table because it is read-only.");
    return 0;
}

inline lua_Status LuaHelper::CompileToLuaFunction(lua_State* State,
                                                  const char* SourcePath) {
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
        return LUA_ERRERR;
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
    // TODO: fix this and also load and save proper chunk names rather than just
    // using the source path.
    if (false) {  //(Game::Lua::CacheLuaBytecode) {
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

        if (strncmp(BytecodeCache->Header, "BYTECODECACHE",
                    sizeof("BYTECODECACHE")) != 0 ||
            SourceCodeCRC != BytecodeCache->SourceCRC) {
            goto CheckForCacheBytecodeFail_FreeBytecodeCache;
        }
        SDL_free(SourceCode);

        luau_load(State, SourcePath, BytecodeCache->Bytecode,
                  BytecodeCacheSize - sizeof(Uint32), 0);
        /*
luaL_loadbuffer(State, BytecodeCache->Bytecode,
              BytecodeCacheSize - sizeof(Uint32), SourcePath);*/
        SDL_free(BytecodeCache);

        return LUA_OK;

    CheckForCacheBytecodeFail_FreeBytecodeCache:
        SDL_free(BytecodeCache);
    }
CheckForCacheBytecodeFail:

    // luaL_loadstring(State, SourceCode);

    size_t LuaSourceSize;
    char* LuaSource =
        static_cast<char*>(SDL_LoadFile(SourcePath, &LuaSourceSize));

    size_t BytecodeSize;
    char* Bytecode =
        luau_compile(LuaSource, LuaSourceSize, NULL, &BytecodeSize);
    SDL_free(LuaSource);

    const lua_Status CompileStatus =
        (lua_Status)luau_load(State, SourcePath, Bytecode, BytecodeSize, 0);
    free(Bytecode);

    // lua_getinfo(lua_State *L, int level, const char *what, lua_Debug *ar)

    // const int CompileStatus = luaL_loadfile(State, SourcePath);
    if (CompileStatus != LUA_OK) {
        return CompileStatus;
    }

    SDL_free(SourceCode);

    /*
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



                   //lua_getinfo(lua_State *L, int level, const char *what,
    lua_Debug *ar); lua_dump( State,
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
    }*/

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

void LuaHelper::StackTableReference::SetKeyClosure(lua_State* State,
                                                   lua_CFunction Closure,
                                                   const char* ClosureName,
                                                   const char* Key,
                                                   int Upvalues) {
    LuaHelper::SetKeyClosure(State, this->TableIndex, Closure, ClosureName, Key, Upvalues);
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
    LuaHelper::SetIndexClosure(State, this->TableIndex, Closure, ClosureName, Index,
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

    luaL_typeerrorL(State, UserdataIndex, lua_tostring(State, -1));
    return NULL;
}

inline void* LuaHelper::TestMetatable(lua_State* State,
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

    LuaHelper::SetKeyClosure(State, -2, ::__readonly_metatable_newindex, "readonly_metatable_error_yeller", "__newindex");

    lua_setmetatable(State, TableIndex);
}

#endif