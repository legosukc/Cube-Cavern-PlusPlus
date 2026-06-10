#pragma once

#include <lua-5.5.0/lua.hpp>


namespace {
	
	static void* _PushExternalString_DefaultDeallocator(void* ud, void* ptr, size_t osize, size_t nsize) {

		std::free(ptr);
		Game::Statistics::Memory::EngineAllocatedBytes -= osize;
		++Game::Statistics::Memory::EngineDeallocationsPerFrame;
		return NULL;
	}

	static int __readonly_metatable_newindex(lua_State* State) {
		luaL_error(State, "Cannot modify table because it is read-only.");
		return 0;
	}
}


namespace LuaHelper {

	int PCallLog(lua_State* State, int ArgumentsPassed = 0, int ReturnValues = 0);

	int LoadFileLog(lua_State* State, const char* Path);


	// Loads a lua file and runs it unprotected. Only automatically prints any caught Lua errors from file compilation.
	int CallFileLog(lua_State* State, const char* Path, int ReturnValues = 1);

	// Loads a lua file and runs it with a pcall, automatically printing any caught Lua errors.
	int PCallFileLog(lua_State* State, const char* Path, int ReturnValues = 1);


	template<typename T>
	constexpr void Push(lua_State* State, T Value);

	inline void PushExternalString(lua_State* State, const char* String, size_t StringLength, lua_Alloc FreeString) {

#if LUA_VERSION_MAJOR_N >= 5 && LUA_VERSION_MINOR_N >= 5
		lua_pushexternalstring(State, String, StringLength, FreeString, NULL);
#else
#warning "Project is being compiled with a older version of Lua (older than 5.5.0) that doesn't support 'lua_pushexternalstring', compatability problems may arise. (parameter 'FreeString' will never be called.)"
		lua_pushlstring(State, String, StringLength);
#endif
	}
	inline void PushExternalString(lua_State* State, const char* String, size_t StringLength) {
		PushExternalString(State, String, StringLength, ::_PushExternalString_DefaultDeallocator);
	}
	inline char* AllocateExternalString(size_t StringLength) {
		return static_cast<char*>(std::malloc(StringLength));
	}

	template<typename T>
	constexpr T ToTypename(lua_State* State, int StackIndex);

	template<typename UserdataType>
	[[nodiscard]]
	inline UserdataType& GetUserdata(lua_State* State, int StackIndex);

	inline void PushClosure(lua_State* State, lua_CFunction Closure, int Upvalues = 0);


	template<typename Arg_T>
	constexpr void PushArgs(lua_State* State, Arg_T Value) {
		LuaHelper::Push<Arg_T>(State, Value);
	}

	template<typename Arg_T, typename... Args_T>
	constexpr void PushArgs(lua_State* State, Arg_T Arg, Args_T... Args) {
		LuaHelper::Push<Arg_T>(State, Arg);
		LuaHelper::PushArgs<Args_T...>(State, Args...);
	}



	template<typename Value_T>
	inline void SetGlobal(lua_State* State, Value_T Value, const char* Name);


	template<typename Value_T, typename Index_T>
	inline void SetTable(lua_State* State, int TableIndex, Value_T Value, Index_T Index);

	template<typename Value_T>
	inline void SetKey(lua_State* State, int TableIndex, Value_T Value, const char* Key);

	template<typename Value_T>
	inline void SetIndex(lua_State* State, int TableIndex, Value_T Value, lua_Integer Index);


	template<typename GetType>
	[[nodiscard]]
	inline GetType GetGlobal(lua_State* State, const char* Name);


	template<typename GetType>
	[[nodiscard]]
	inline GetType GetTable(lua_State* State, int TableIndex);

	template<typename GetType>
	[[nodiscard]]
	inline GetType GetKey(lua_State* State, int TableIndex, const char* Key);

	template<typename GetType>
	[[nodiscard]]
	inline GetType GetIndex(lua_State* State, int TableIndex, lua_Integer Index);



	template<typename Index_T>
	inline void SetTableClosure(lua_State* State, int TableIndex, lua_CFunction Closure, Index_T Index, int Upvalues = 0);

	inline void SetKeyClosure(lua_State* State, int TableIndex, lua_CFunction Closure, const char* Key, int Upvalues = 0);
	inline void SetIndexClosure(lua_State* State, int TableIndex, lua_CFunction Closure, lua_Integer Index, int Upvalues = 0);



	class StackTableReference {
		int TableIndex;
	public:
		// Uses the top of the passed States' stack as the table reference (TableIndex).
		inline StackTableReference(lua_State* State) : TableIndex(lua_gettop(State)) {}

		// Uses the top of the passed States' stack as the table reference (TableIndex).
		inline StackTableReference(lua_State* State, const char* MetatableName) {
			luaL_newmetatable(State, MetatableName);
			this->TableIndex = lua_gettop(State);
		}

		// Creates a new table and sets the table reference (TableIndex) to the top of the stack.
		inline StackTableReference(lua_State* State, int AllocateArrays, int AllocateKeys) {
			lua_createtable(State, AllocateArrays, AllocateKeys);
			this->TableIndex = lua_gettop(State);
		}

		// Uses the passed 'Index' parameter as the table reference (TableIndex).
		constexpr StackTableReference(int Index) noexcept : TableIndex(Index) {}
		inline StackTableReference() noexcept = default;

		[[nodiscard]]
		constexpr inline int GetStackIndex() noexcept {
			return this->TableIndex;
		}

		inline void PushReference(lua_State* State) {
			lua_pushvalue(State, this->TableIndex);
		}


		template<typename Value_T, typename Index_T>
		inline void SetTable(lua_State* State, Value_T Value, Index_T Index) {
			LuaHelper::SetTable<Value_T, Index_T>(State, this->TableIndex, Value, Index)
		}

		template<typename Value_T>
		inline void SetKey(lua_State* State, Value_T Value, const char* Key) {
			LuaHelper::SetKey<Value_T>(State, this->TableIndex, Value, Key);
		}

		template<typename Value_T>
		inline void SetIndex(lua_State* State, Value_T Value, lua_Integer Index) {
			LuaHelper::SetIndex<Value_T>(State, this->TableIndex, Value, Index);
		}

		template<typename Index_T>
		inline void SetTableClosure(lua_State* State, lua_CFunction Closure, Index_T Index, int Upvalues = 0) {
			LuaHelper::SetTableClosure<Index_T>(State, this->TableIndex, Closure, Index, Upvalues);
		}

		inline void SetKeyClosure(lua_State* State, lua_CFunction Closure, const char* Key, int Upvalues = 0) {
			LuaHelper::SetKeyClosure(State, this->TableIndex, Closure, Key, Upvalues);
		}

		inline void SetIndexClosure(lua_State* State, lua_CFunction Closure, lua_Integer Index, int Upvalues = 0) {
			LuaHelper::SetIndexClosure(State, this->TableIndex, Closure, Index, Upvalues);
		}

		inline void PushKey(lua_State* State, const char* Key) const {
			lua_getfield(State, this->TableIndex, Key);
		}
	};


	void* CheckMetatable(lua_State* State, int UserdataIndex, int MetatableIndex) {
		
		const int StackTop = lua_gettop(State);

		unlikely_branch
		if (lua_getmetatable(State, UserdataIndex) == 0) {
			goto Fail;
		}

		unlikely_branch
		if (lua_rawequal(State, -1, MetatableIndex) == 0) {
			goto Fail;
		}

		lua_settop(State, StackTop);

		return lua_touserdata(State, UserdataIndex);

		Fail:
		lua_getfield(State, MetatableIndex, "__name");
		luaL_typeerror(State, UserdataIndex, lua_tostring(State, -1));
		return NULL;
	}

	[[nodiscard]]
	void* TestMetatable(lua_State* State, int UserdataIndex, int MetatableIndex) {

		void* Userdata;
		int StartingStack;

		StartingStack = lua_gettop(State);

		unlikely_branch
		if (!lua_getmetatable(State, UserdataIndex) || !lua_rawequal(State, -1, MetatableIndex)) {
			Userdata = NULL;
		} else {
			Userdata = lua_touserdata(State, UserdataIndex);
		}
		lua_settop(State, StartingStack);

		return Userdata;
	}


	void LockTable(lua_State* State, int TableIndex) {

		lua_createtable(State, 0, 4);

		if (lua_getmetatable(State, TableIndex)) {

			lua_pushvalue(State, -1);
			lua_setfield(State, -3, "__metatable");

			lua_setmetatable(State, -2);
		}
		
		LuaHelper::SetKeyClosure(State, -2, ::__readonly_metatable_newindex, "__newindex");

		lua_setmetatable(State, TableIndex);
	}
}



int LuaHelper::PCallLog(lua_State* State, int ArgumentsPassed, int ReturnValues) {

	const int ErrorCode = lua_pcall(State, ArgumentsPassed, ReturnValues, 0);

	likely_branch
		if (ErrorCode == LUA_OK) {
			return LUA_OK;
		}

	std::clog << "Caught a ";

	switch (ErrorCode) {
	case LUA_ERRRUN:    // runtime error
		std::clog << "runtime";
		break;

	case LUA_ERRMEM:    // memory allocation error. For such errors, Lua does not call the message handler.
		std::clog << "memory allocation";
		break;

	case LUA_ERRERR:    // error while running the message handler.
		std::clog << "message handler";
		break;
		//case LUA_ERRGCMM: // error while running a __gc metamethod. For such errors, Lua does not call the message handler (as this kind of error typically has no relation with the function being called).
	}

	std::clog << " error from PCall:\n" << lua_tostring(State, -1) << std::endl;
	return ErrorCode;
}

int LuaHelper::LoadFileLog(lua_State* State, const char* Path) {

	const int ErrorCode = luaL_loadfile(State, Path);
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


// Loads a lua file and runs it unprotected. Only automatically prints any caught Lua errors from file compilation.
int LuaHelper::CallFileLog(lua_State* State, const char* Path, int ReturnValues) {

	int Error = LuaHelper::LoadFileLog(State, Path);

	likely_branch
		if (Error == LUA_OK) {
			lua_call(State, 0, ReturnValues);
		}
	return Error;
}

// Loads a lua file and runs it with a pcall, automatically printing any caught Lua errors.
int LuaHelper::PCallFileLog(lua_State* State, const char* Path, int ReturnValues) {

	int Error = LuaHelper::LoadFileLog(State, Path);

	likely_branch
		if (Error == LUA_OK) {
			Error = LuaHelper::PCallLog(State, 0, ReturnValues);
		}
	return Error;
}


template<typename T>
constexpr void LuaHelper::Push(lua_State* State, T Value) {

	if CONSTEXPR_IF(std::is_same_v<std::remove_cv_t<T>, bool>) {
		lua_pushboolean(State, static_cast<int>(Value));

	} else if CONSTEXPR_IF(std::is_integral_v<T>) {
		lua_pushinteger(State, static_cast<lua_Integer>(Value));

	} else if CONSTEXPR_IF(std::is_floating_point_v<T>) {
		lua_pushnumber(State, static_cast<lua_Number>(Value));

	} else if CONSTEXPR_IF(std::is_function_v<T>) {
		static_assert(std::is_same_v<std::remove_cv_t<T>, lua_CFunction>, "Template parameter 'T' is a function type that doesn't match function type 'lua_CFunction'.");
		lua_pushcfunction(State, Value);
	
	} else if CONSTEXPR_IF(std::is_pointer_v<T>) {
		lua_pushlightuserdata(State, reinterpret_cast<void*>(Value));
	}
}

template<typename T>
constexpr T LuaHelper::ToTypename(lua_State* State, int StackIndex) {

	if CONSTEXPR_IF(std::is_same_v<std::remove_cv_t<T>, bool>) {
		return static_cast<T>(lua_toboolean(State, StackIndex));

	} else if CONSTEXPR_IF(std::is_integral_v<T>) {
		return static_cast<T>(lua_tointeger(State, StackIndex));

	} else if CONSTEXPR_IF(std::is_floating_point_v<T>) {
		return static_cast<T>(lua_tonumber(State, StackIndex));

	} else if CONSTEXPR_IF(std::is_function_v<T>) {
		static_assert(std::is_same_v<std::remove_cv_t<T>, lua_CFunction>, "Template parameter 'T' is a function type that doesn't match function type 'lua_CFunction'.");
		return lua_tocfunction(State, StackIndex);

	} else if CONSTEXPR_IF(std::is_pointer_v<T>) {
		return static_cast<T>(lua_touserdata(State, StackIndex));
	}
}

template<typename UserdataType>
UserdataType& LuaHelper::GetUserdata(lua_State* State, int StackIndex) {
	return *static_cast<UserdataType*>(lua_touserdata(State, StackIndex));
}



void LuaHelper::PushClosure(lua_State* State, lua_CFunction Closure, int Upvalues) {
	lua_pushcclosure(State, Closure, Upvalues);
}


template<typename Value_T>
void LuaHelper::SetGlobal(lua_State* State, Value_T Value, const char* Name) {
	LuaHelper::Push<Value_T>(State, Value);
	lua_setglobal(State, Name);
}



template<typename Value_T, typename Index_T>
void LuaHelper::SetTable(lua_State* State, int TableIndex, Value_T Value, Index_T Index) {
	LuaHelper::Push<Index_T>(State, Index);
	LuaHelper::Push<Value_T>(State, Value);
	lua_settable(State, TableIndex);
}

template<typename Value_T>
void LuaHelper::SetKey(lua_State* State, int TableIndex, Value_T Value, const char* Key) {
	LuaHelper::Push<Value_T>(State, Value);
	lua_setfield(State, TableIndex, Key);
}

template<typename Value_T>
void LuaHelper::SetIndex(lua_State* State, int TableIndex, Value_T Value, lua_Integer Index) {
	LuaHelper::Push<Value_T>(State, Value);
	lua_seti(State, TableIndex, Index);
}


template<typename Index_T>
void LuaHelper::SetTableClosure(lua_State* State, int TableIndex, lua_CFunction Closure, Index_T Index, int Upvalues) {
	LuaHelper::PushClosure(State, Closure, Upvalues);
	lua_settable(State, TableIndex);
}

void LuaHelper::SetKeyClosure(lua_State* State, int TableIndex, lua_CFunction Closure, const char* Key, int Upvalues) {
	LuaHelper::PushClosure(State, Closure, Upvalues);
	lua_setfield(State, TableIndex, Key);
}

void LuaHelper::SetIndexClosure(lua_State* State, int TableIndex, lua_CFunction Closure, lua_Integer Index, int Upvalues) {
	LuaHelper::PushClosure(State, Closure, Upvalues);
	lua_seti(State, TableIndex, Index);
}