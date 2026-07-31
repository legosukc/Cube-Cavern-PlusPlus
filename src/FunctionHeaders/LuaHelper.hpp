#pragma once

#include "../define.h"

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_iostream.h>

#include <lua.hpp>

namespace LuaHelper {

    int CompileToLuaFunction(lua_State* State, const char* SourcePath);

    int PCallLog(lua_State* State,
                 int ArgumentsPassed = 0,
                 int ReturnValues = 0);

    int LoadFileLog(lua_State* State, const char* Path);

    // Loads a lua file and runs it unprotected. Only automatically prints any
    // caught Lua errors from file compilation.
    int CallFileLog(lua_State* State, const char* Path, int ReturnValues = 1);

    // Loads a lua file and runs it with a pcall, automatically printing any
    // caught Lua errors.
    int PCallFileLog(lua_State* State, const char* Path, int ReturnValues = 1);

    template <typename T>
    constexpr void Push(lua_State* State, T Value);

    template <typename T>
    constexpr T ToTypename(lua_State* State, int StackIndex);

    template <typename UserdataType>
    [[nodiscard]]
    inline UserdataType& GetUserdata(lua_State* State, int StackIndex);

    inline void PushClosure(lua_State* State,
                            lua_CFunction Closure,
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
    inline void SetGlobal(lua_State* State, Value_T Value, const char* Name);

    template <typename Value_T, typename Index_T>
    inline void SetTable(lua_State* State,
                         int TableIndex,
                         Value_T Value,
                         Index_T Index);

    template <typename Value_T>
    inline void SetKey(lua_State* State,
                       int TableIndex,
                       Value_T Value,
                       const char* Key);

    template <typename Value_T>
    inline void SetIndex(lua_State* State,
                         int TableIndex,
                         Value_T Value,
                         lua_Integer Index);

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
                                Index_T Index,
                                int Upvalues = 0);

    inline void SetKeyClosure(lua_State* State,
                              int TableIndex,
                              lua_CFunction Closure,
                              const char* Key,
                              int Upvalues = 0);
    inline void SetIndexClosure(lua_State* State,
                                int TableIndex,
                                lua_CFunction Closure,
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
        inline void SetTable(lua_State* State, Value_T Value, Index_T Index);

        template <typename Value_T>
        inline void SetKey(lua_State* State, Value_T Value, const char* Key);

        template <typename Value_T>
        inline void SetIndex(lua_State* State,
                             Value_T Value,
                             lua_Integer Index);

        template <typename Index_T>
        inline void SetTableClosure(lua_State* State,
                                    lua_CFunction Closure,
                                    Index_T Index,
                                    int Upvalues = 0);
        inline void SetKeyClosure(lua_State* State,
                                  lua_CFunction Closure,
                                  const char* Key,
                                  int Upvalues = 0);
        inline void SetIndexClosure(lua_State* State,
                                    lua_CFunction Closure,
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

    void LockTable(lua_State* State, int TableIndex);
}  // namespace LuaHelper