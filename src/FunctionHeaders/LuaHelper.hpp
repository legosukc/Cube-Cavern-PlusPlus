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
    UserdataType& GetUserdata(lua_State* State, int StackIndex) {
        return *static_cast<UserdataType*>(lua_touserdata(State, StackIndex));
    }

    void PushClosure(lua_State* State,
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
    void SetGlobal(lua_State* State, Value_T Value, const char* Name) {
        LuaHelper::Push<Value_T>(State, Value);
        lua_setglobal(State, Name);
    }

    template <typename Value_T, typename Index_T>
    void SetTable(lua_State* State,
                  int TableIndex,
                  Value_T Value,
                  Index_T Index) {
        LuaHelper::Push<Index_T>(State, Index);
        LuaHelper::Push<Value_T>(State, Value);
        lua_settable(State, TableIndex);
    }

    template <typename Value_T>
    void SetKey(lua_State* State,
                int TableIndex,
                Value_T Value,
                const char* Key) {
        LuaHelper::Push<Value_T>(State, Value);
        lua_setfield(State, TableIndex, Key);
    }

    template <typename Value_T>
    void SetIndex(lua_State* State,
                  int TableIndex,
                  Value_T Value,
                  lua_Integer Index) {
        lua_pushinteger(State, Index);
        LuaHelper::Push<Value_T>(State, Value);

        lua_settable(State, TableIndex);
    }

    template <typename GetType>
    [[nodiscard]]
    GetType GetGlobal(lua_State* State, const char* Name);

    template <typename GetType>
    [[nodiscard]]
    GetType GetTable(lua_State* State, int TableIndex);

    template <typename GetType>
    [[nodiscard]]
    GetType GetKey(lua_State* State, int TableIndex, const char* Key);

    template <typename GetType>
    [[nodiscard]]
    GetType GetIndex(lua_State* State, int TableIndex, lua_Integer Index);

    template <typename Index_T>
    void SetTableClosure(lua_State* State,
                         int TableIndex,
                         lua_CFunction Closure,
                         const char* ClosureName,
                         Index_T Index,
                         int Upvalues = 0) {
        LuaHelper::PushClosure(State, Closure, ClosureName, Upvalues);
        lua_settable(State, TableIndex);
    }

    void SetKeyClosure(lua_State* State,
                       int TableIndex,
                       lua_CFunction Closure,
                       const char* ClosureName,
                       const char* Key,
                       int Upvalues = 0);

    // Uses argument "Key" for the closure name.
    void SetKeyClosure(lua_State* State,
                       int TableIndex,
                       lua_CFunction Closure,
                       const char* Key,
                       int Upvalues = 0);

    void SetIndexClosure(lua_State* State,
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
        StackTableReference(lua_State* State);

        // Uses the top of the passed States' stack as the table reference
        // (TableIndex).
        StackTableReference(lua_State* State, const char* MetatableName);

        // Creates a new table and sets the table reference (TableIndex) to the
        // top of the stack.
        StackTableReference(lua_State* State,
                            int AllocateArrays,
                            int AllocateKeys);

        // Uses the passed 'Index' parameter as the table reference
        // (TableIndex).
        constexpr StackTableReference(int Index) noexcept;
        StackTableReference() noexcept;

        [[nodiscard]]
        constexpr int GetStackIndex() noexcept;

        void PushReference(lua_State* State);

        template <typename Value_T, typename Index_T>
        void SetTable(lua_State* State, Value_T Value, Index_T Index) {
            LuaHelper::SetTable<Value_T, Index_T>(State, this->TableIndex,
                                                  Value, Index);
        }

        template <typename Value_T>
        void SetKey(lua_State* State, Value_T Value, const char* Key) {
            LuaHelper::SetKey<Value_T>(State, this->TableIndex, Value, Key);
        }

        template <typename Value_T>
        void SetIndex(lua_State* State, Value_T Value, lua_Integer Index) {
            LuaHelper::SetIndex<Value_T>(State, this->TableIndex, Value, Index);
        }

        template <typename Index_T>
        void SetTableClosure(lua_State* State,
                             lua_CFunction Closure,
                             const char* ClosureName,
                             Index_T Index,
                             int Upvalues = 0) {
            LuaHelper::SetTableClosure<Index_T>(State, this->TableIndex,
                                                Closure, Index, Upvalues);
        }

        void SetKeyClosure(lua_State* State,
                           lua_CFunction Closure,
                           const char* ClosureName,
                           const char* Key,
                           int Upvalues = 0);

        // Uses argument "Key" for the name of the closure.
        void SetKeyClosure(lua_State* State,
                           lua_CFunction Closure,
                           const char* Key,
                           int Upvalues = 0);

        void SetIndexClosure(lua_State* State,
                             lua_CFunction Closure,
                             const char* ClosureName,
                             lua_Integer Index,
                             int Upvalues = 0);

        void PushKey(lua_State* State, const char* Key) const;
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

// TODO: seperate
#include "LuaHelper.cpp"

#endif