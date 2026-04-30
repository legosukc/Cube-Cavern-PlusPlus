#pragma once

#include <lua-5.4.2/lua.hpp>

#include "../FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua {

	void* LuaAllocationFunc(void*, void* ptr, size_t, size_t nsize);

	inline void Init();
	inline void Destroy();

	inline void Update();

	lua_State* State = NULL;

	LuaHelper::StackTableReference GameTable;
}


#include "../LuaLibraries/LuaTask.hpp"
#include "../LuaLibraries/LuaTypeHelper.hpp"
#include "../LuaLibraries/LuaVector.hpp"
#include "../LuaLibraries/LuaBuffer.hpp"

#include "LuaLibraries/LuaNetwork_Client.hpp"
#include "LuaLibraries/LuaSound_Client.hpp"
#include "LuaLibraries/LuaGraphics.hpp"

#ifndef BUILD_SERVER
#include "LuaLibraries/LuaInput.hpp"
#endif

namespace {

	static void _LoadLuaFileInTable(const std::filesystem::directory_entry& Entry, int TableIDX) {

		const std::filesystem::path& Path = Entry.path();
		const std::string StringPath = Path.generic_u8string();

		unlikely_branch
		if (Path.extension() != ".lua") {
			std::clog << Path << " doesn't have a .lua extension, skipping" << std::endl;
			goto _Cleanup;
		}

		lua_pushstring(Game::Lua::State, StringPath.c_str());
		//lua_pushvalue(Game::Lua::State, -1);

		unlikely_branch
		if (lua_rawget(Game::Lua::State, TableIDX) != LUA_TNIL) {
			std::clog << Path << " already has an entry, skipping" << std::endl;
			goto _Cleanup;
		}

		unlikely_branch
		if (LuaHelper::LoadFileLog(Game::Lua::State, StringPath.c_str()) != LUA_OK) {
			std::clog << "Failed to load " << Path << std::endl;
			goto _Cleanup;
		}

		//lua_pcall(Game::Lua::State, 0, 1, 0);
		
		unlikely_branch
		if (LuaHelper::PCallLog(Game::Lua::State, 0, -1) != LUA_OK) {
			goto _Cleanup;
		}
	
		if (!lua_istable(Game::Lua::State, -1)) {
			std::clog << Path
				<< " returned typename '" << luaL_typename(Game::Lua::State, -1)
				<< "' instead of typename 'table', is this intended behaviour? (Ignore this warning if it is)" << std::endl;
		}

		lua_setfield(Game::Lua::State, TableIDX, StringPath.c_str());
		//lua_settable(Game::Lua::State, TableIDX);

		_Cleanup:
		lua_settop(Game::Lua::State, TableIDX);
	}

	static void _LoadLuaAssetDirectoryInTable(const char* DirectoryPath) {

		std::filesystem::path FilesystemPath;
		int TableIDX;
		
		FilesystemPath = DirectoryPath;

		unlikely_branch
		if (!std::filesystem::exists(FilesystemPath)) {
			std::cerr << "Directory " << FilesystemPath.generic_string() << " doesn't exist.\nWorking directory: "
				<< std::filesystem::current_path().generic_string().c_str() << std::endl;

			Exceptions::ThrowExceptionExpression<Exceptions::IOError>("Directory ", FilesystemPath.generic_string(), " doesn't exist.\nWorking directory: ",
				std::filesystem::current_path().generic_string().c_str()
			);
		}

		unlikely_branch
		if (!std::filesystem::is_directory(FilesystemPath)) {
			std::cerr << "Path " << FilesystemPath.generic_string() << " isn't a directory.\nWorking directory: "
				<< std::filesystem::current_path().generic_string().c_str() << std::endl;

			Exceptions::ThrowExceptionExpression<Exceptions::IOError>("Path ", FilesystemPath.generic_string(), " isn't a directory.\nWorking directory: ",
				std::filesystem::current_path().generic_string().c_str()
			);
		}
		TableIDX = lua_gettop(Game::Lua::State);

		for (const auto& Entry : std::filesystem::directory_iterator(FilesystemPath)) {
			::_LoadLuaFileInTable(Entry, TableIDX);
		}
	}


	struct _LuaDirectoryLibrary {
		inline _LuaDirectoryLibrary() = default;
		_LuaDirectoryLibrary(const char* _Name, int _ParentTableIDX);

		int TableReferenceIndex, ParentTableIndex;
		std::string Path;

		void Load();

		static int __index(lua_State* State);
	};

	_LuaDirectoryLibrary::_LuaDirectoryLibrary(const char* _Name, int _ParentTableIDX) : ParentTableIndex(_ParentTableIDX) {

		lua_createtable(Game::Lua::State, 0, 0);
		this->TableReferenceIndex = lua_gettop(Game::Lua::State);

		lua_pushvalue(Game::Lua::State, this->TableReferenceIndex);
		lua_setfield(Game::Lua::State, _ParentTableIDX, _Name);


		// METATABLE
		lua_createtable(Game::Lua::State, 0, 1);

		this->Path = StringHelper::Combine(_Name, '\\');

		lua_pushstring(Game::Lua::State, this->Path.c_str());
		lua_pushcclosure(Game::Lua::State, _LuaDirectoryLibrary::__index, 1);
		lua_setfield(Game::Lua::State, -2, "__index");

		lua_setmetatable(Game::Lua::State, this->TableReferenceIndex);

		lua_settop(Game::Lua::State, this->TableReferenceIndex);
	}

	void _LuaDirectoryLibrary::Load() {
		lua_pushvalue(Game::Lua::State, this->TableReferenceIndex);
		::_LoadLuaAssetDirectoryInTable(this->Path.c_str());
		lua_pop(Game::Lua::State, 1);
	}

	int _LuaDirectoryLibrary::__index(lua_State* State) {

		lua_pushvalue(State, 2);
		if (lua_rawget(State, 1) == LUA_TNIL) {
			
			const std::string Path = StringHelper::Combine(lua_tostring(State, lua_upvalueindex(1)), lua_tostring(State, 2), ".lua");

			unlikely_branch
			if (!std::filesystem::exists(Path)) {
				luaL_error(State, "Attempted to index a non-existant entry '", lua_tostring(State, 2), "' in table ", lua_tostring(State, lua_upvalueindex(1)));
			}

			unlikely_branch
			if (luaL_loadfile(State, Path.c_str()) != LUA_OK) {
				lua_error(State);
			}

			lua_call(State, 0, 1);
			lua_pushvalue(State, 2);
			lua_pushvalue(State, -2);
			lua_rawset(State, 1);
		}

		lua_rotate(State, 0, 1);
		return 1;
	}
}



void* Game::Lua::LuaAllocationFunc(void*, void* ptr, size_t, size_t nsize) {
	using namespace Game::Statistics;

	const static thread_local std::chrono::high_resolution_clock::time_point StartTime = std::chrono::high_resolution_clock::now();

	if (nsize == 0) {
		std::free(ptr);
		ptr = NULL;

		Statistics::Memory::UpdateLuaStatsDeallocation(static_cast<Uint64>(nsize));

	} else {
		ptr = std::realloc(ptr, nsize);

		Statistics::Memory::UpdateLuaStatsAllocation(static_cast<Uint64>(nsize));
	}

	Statistics::Memory::LuaMicrosecondsSpentOnHeapPerFrame += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - StartTime).count();

	return ptr;
}


void Game::Lua::Init() {

	auto Start = std::chrono::high_resolution_clock::now();
	std::cout << "Initializing Lua.\n";

	Game::Lua::State = luaL_newstate(); //lua_newstate(Game::Lua::LuaAllocationFunc, NULL);
	if (Game::Lua::State == NULL) {
		std::cerr << "Failed to create the Lua state." << std::endl;
		Exceptions::ThrowException<Exceptions::RuntimeError>("Failed to create the Lua state.");
	}

	luaopen_base(Game::Lua::State);

	luaopen_math(Game::Lua::State);

	luaopen_string(Game::Lua::State);
	luaopen_utf8(Game::Lua::State);

	luaopen_table(Game::Lua::State);
	luaopen_coroutine(Game::Lua::State);

	Lua::CLibraries::Task::Init(Game::Lua::State);

	Lua::CLibraries::TypeHelper::Init(Game::Lua::State);

	Lua::CLibraries::Vector::Init(Game::Lua::State);
	Lua::CLibraries::Buffer::Init(Game::Lua::State);

#ifndef BUILD_SERVER
	Lua::CLibraries::Graphics::Init(Game::Lua::State);
	Lua::CLibraries::Input::Init(Game::Lua::State);
#endif


	// Create global tables
	Game::Lua::GameTable = LuaHelper::StackTableReference(Game::Lua::State, 0, 4);

	lua_pushvalue(Game::Lua::State, -1);
	lua_setglobal(Game::Lua::State, "Game");

	::_LoadLuaAssetDirectoryInTable("GameClasses\\");

	lua_createtable(Game::Lua::State, 0, 4); // Assets

	{
		int ParentTableIDX;
		::_LuaDirectoryLibrary ItemClasses, EnemyClasses, MiscClasses, BaseClasses;

		ParentTableIDX = lua_gettop(Game::Lua::State);
		lua_pushvalue(Game::Lua::State, ParentTableIDX);
		lua_setglobal(Game::Lua::State, "Assets");

		ItemClasses = ::_LuaDirectoryLibrary("ItemClasses", ParentTableIDX);
		EnemyClasses = ::_LuaDirectoryLibrary("EnemyClasses", ParentTableIDX);
		MiscClasses = ::_LuaDirectoryLibrary("MiscClasses", ParentTableIDX);
		BaseClasses = ::_LuaDirectoryLibrary("BaseClasses", ParentTableIDX);

		BaseClasses.Load();
		MiscClasses.Load();
		EnemyClasses.Load();
		ItemClasses.Load();
	}


	std::cout << "Initalized Lua system, took "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - Start).count()
		<< " milliseconds." << std::endl;
}

void Game::Lua::Update() {
	CLibraries::Task::Update(Game::Lua::State);
}


void Game::Lua::Destroy() {

	if (Game::Lua::State != NULL) {
		lua_close(Game::Lua::State);
		Game::Lua::State = NULL;
	}
}