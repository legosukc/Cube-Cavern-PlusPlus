#pragma once

#include <random>

#include <fstream>


#include <lua-5.5.0/lua.hpp>

#include "FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua {

	struct LuaThreadInfo {
		LuaThreadInfo() {
			
#ifndef BUILD_SERVER
			GLint MaxBoundTextures;
			glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MaxBoundTextures);
			
			this->ActiveTextures = static_cast<GLuint*>(std::calloc(0, MaxBoundTextures * sizeof(GLuint)));

			//this->BoundVertexArray = glGetVertexArrayiv()
			
			this->DepthTest = glIsEnabled(GL_DEPTH_TEST);
			this->StencilTest = glIsEnabled(GL_STENCIL_TEST);
#endif
		}

#ifndef BUILD_SERVER
		bool isMainState;
		GLuint BoundVertexArray;
		GLuint BoundVertexBuffer;
		GLuint BoundElementBuffer;
		GLuint BoundUniformBuffer;
		GLint BoundTexture = 0;
		GLuint* ActiveTextures;

		GLuint BoundProgram = 0;

		bool DepthTest, StencilTest;
#endif
	};

	void* LuaAllocationFunc(void*, void* ptr, size_t, size_t nsize);

	inline void Init();
	inline void Destroy();

	inline void Update();
#ifndef BUILD_SERVER
	void Draw();
#endif

	lua_State* State = NULL;

	LuaHelper::StackTableReference GameTable;
}

#include "LuaLibraries/LuaTable.hpp"
#include "LuaLibraries/LuaEnums.hpp"
#include "LuaLibraries/LuaTask.hpp"
#include "LuaLibraries/LuaTypeHelper.hpp"

#include "LuaLibraries/LuaVector.hpp"
#include "LuaLibraries/LuaMatrix.hpp"

#include "LuaLibraries/LuaBuffer.hpp"

#include "LuaLibraries/LuaGraphics.hpp"
#include "LuaLibraries/LuaInput.hpp"

#ifndef BUILD_SERVER

#include "LuaLibraries/LuaNetwork_Client.hpp"
#include "LuaLibraries/LuaSound_Client.hpp"
#else

#include "LuaLibraries/LuaNetwork_Server.hpp"
#include "LuaLibraries/LuaSound_Server.hpp"
#endif

namespace {

	static void _LoadLuaFileInTable(const std::filesystem::directory_entry& Entry, int TableIDX) {

		const std::filesystem::path& Path = Entry.path();
		const std::string StringPath = Path.generic_u8string();

		unlikely_branch
		if (Path.extension() != ".lua") {
			std::clog << Path << " doesn't have a .lua extension, skipping" << std::endl;
			return;
		}

		lua_pushstring(Game::Lua::State, Path.stem().generic_string().c_str());
		//lua_pushvalue(Game::Lua::State, -1);

		unlikely_branch
		if (lua_rawget(Game::Lua::State, TableIDX) != LUA_TNIL) {
			std::clog << Path << " already has an entry, skipping" << std::endl;
			lua_pop(Game::Lua::State, 1);
			return;
		}
		lua_pop(Game::Lua::State, 1);

		std::cout << "Loading " << Path.stem().generic_string().c_str() << std::endl;

		unlikely_branch
		if (LuaHelper::LoadFileLog(Game::Lua::State, StringPath.c_str()) != LUA_OK) {
			std::clog << "Failed to load " << Path << std::endl;
			return;
		}

		unlikely_branch
		if (LuaHelper::PCallLog(Game::Lua::State, 0, 1) != LUA_OK) {
			lua_pop(Game::Lua::State, 1);
			return;
		}
	
		if (!lua_istable(Game::Lua::State, -1)) {
			std::clog << Path
				<< " returned type '" << luaL_typename(Game::Lua::State, -1)
				<< "' instead of type 'table', is this intended behaviour? (Ignore this warning if it is)" << std::endl;
		}

		lua_setfield(Game::Lua::State, TableIDX, Path.stem().generic_string().c_str());
	}

	static void _LoadLuaAssetDirectoryInTable(const char* DirectoryPath, int TableIDX) {

		std::filesystem::path FilesystemPath;
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

		lua_pushlstring(Game::Lua::State, this->Path.c_str(), this->Path.size());
		lua_pushcclosure(Game::Lua::State, _LuaDirectoryLibrary::__index, 1);
		lua_setfield(Game::Lua::State, -2, "__index");

		lua_setmetatable(Game::Lua::State, this->TableReferenceIndex);

		lua_settop(Game::Lua::State, this->TableReferenceIndex);
	}

	void _LuaDirectoryLibrary::Load() {
		//lua_pushvalue(Game::Lua::State, this->TableReferenceIndex);
		::_LoadLuaAssetDirectoryInTable(this->Path.c_str(), this->TableReferenceIndex);
		//lua_pop(Game::Lua::State, 1);
	}

	int _LuaDirectoryLibrary::__index(lua_State* State) {

		lua_pushvalue(State, 2);

		unlikely_branch
		if (lua_rawget(State, 1) == LUA_TNIL) {
			
			const std::string Path = StringHelper::Combine(lua_tostring(State, lua_upvalueindex(1)), luaL_checkstring(State, 2), ".lua");

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

		lua_rotate(State, 1, 1);
		return 1;
	}
}


//static std::ofstream _AllocLogFile;
void* Game::Lua::LuaAllocationFunc(void*, void* ptr, size_t, size_t nsize) {
	using namespace Game::Statistics;

	const auto StartTime = std::chrono::high_resolution_clock::now();

	if (nsize == 0) {

		if (ptr != NULL) {
			ptr = static_cast<char*>(ptr) - sizeof(size_t);
			//::_AllocLogFile << "free: " << reinterpret_cast<void*>(static_cast<char*>(ptr) + sizeof(size_t)) << ", " << ptr << std::endl;
			//std::cout << "free: " << reinterpret_cast<void*>(static_cast<char*>(ptr) + sizeof(size_t)) << ", " << ptr << std::endl;

			Statistics::Memory::UpdateLuaStatsDeallocation(static_cast<Uint64>(*static_cast<size_t*>(ptr)));
			std::free(ptr);
		}

		Statistics::Memory::LuaMicrosecondsSpentOnHeapPerFrame += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - StartTime).count();
		return NULL;
	}

	nsize += sizeof(size_t);
	if (ptr == NULL) {
		ptr = std::malloc(nsize);
	} else {
		ptr = std::realloc(static_cast<char*>(ptr) - sizeof(size_t), nsize);
	}
	*static_cast<size_t*>(ptr) = nsize;

	//::_AllocLogFile << "alloc: " << reinterpret_cast<void*>(static_cast<char*>(ptr) + sizeof(size_t)) << ", " << ptr << std::endl;

	Statistics::Memory::UpdateLuaStatsAllocation(static_cast<Uint64>(nsize));

	Statistics::Memory::LuaMicrosecondsSpentOnHeapPerFrame += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - StartTime).count();

	return static_cast<char*>(ptr) + sizeof(size_t);
}


void Game::Lua::Init() {

	auto Start = std::chrono::high_resolution_clock::now();
	std::cout << "Initializing Lua." << std::endl;

	//::_AllocLogFile.open("alloclog.txt");

	Game::Lua::State = lua_newstate(Game::Lua::LuaAllocationFunc, NULL, std::random_device()());
	if (Game::Lua::State == NULL) {
		std::cerr << "Failed to create the Lua state." << std::endl;
		Exceptions::ThrowException<Exceptions::RuntimeError>("Failed to create the Lua state.");
	}

	Game::Lua::LuaThreadInfo* StateInfo = new Game::Lua::LuaThreadInfo;

#ifndef BUILD_SERVER
	StateInfo->isMainState = true;
#endif
	
	(*static_cast<Game::Lua::LuaThreadInfo**>(lua_getextraspace(Game::Lua::State))) = StateInfo;


	luaopen_base(Game::Lua::State);

	luaopen_math(Game::Lua::State);
	lua_setglobal(Game::Lua::State, LUA_MATHLIBNAME);

	luaopen_string(Game::Lua::State);
	lua_setglobal(Game::Lua::State, LUA_STRLIBNAME);

	luaopen_utf8(Game::Lua::State);
	lua_setglobal(Game::Lua::State, LUA_UTF8LIBNAME);

	luaopen_coroutine(Game::Lua::State);
	lua_setglobal(Game::Lua::State, LUA_COLIBNAME);

	


	// Create global tables
	Game::Lua::GameTable = LuaHelper::StackTableReference(Game::Lua::State, 0, 4);
	
	Game::Lua::GameTable.PushReference(Game::Lua::State);
	lua_setglobal(Game::Lua::State, "Game");


	Lua::CLibraries::table::Init(Game::Lua::State);

	Lua::CLibraries::Enums::Init(Game::Lua::State);

	Lua::CLibraries::Task::Init(Game::Lua::State);

	Lua::CLibraries::TypeHelper::Init(Game::Lua::State);

	Lua::CLibraries::Vector::Init(Game::Lua::State);
	Lua::CLibraries::Matrix::Init(Game::Lua::State);

	Lua::CLibraries::Buffer::Init(Game::Lua::State);

	Lua::CLibraries::Graphics::Init(Game::Lua::State);
	Lua::CLibraries::Input::Init(Game::Lua::State);


	::_LoadLuaAssetDirectoryInTable("GameClasses\\", Game::Lua::GameTable.GetStackIndex());

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

		MiscClasses.Load();
		BaseClasses.Load();
		EnemyClasses.Load();
		ItemClasses.Load();
	}


	std::cout << "Initalized Lua system, took "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - Start).count()
		<< " milliseconds." << std::endl;
}


void Game::Lua::Update() {

	Game::Lua::GameTable.SetKey(Game::Lua::State, Game::DeltaTime, "DeltaTime");

	CLibraries::Task::Update(Game::Lua::State);

	lua_pushnil(Game::Lua::State);
	const int StackTop = lua_gettop(Game::Lua::State);
	while (lua_next(Game::Lua::State, Game::Lua::GameTable.GetStackIndex()) != 0) {

		if (lua_getfield(Game::Lua::State, -1, "Update") == LUA_TFUNCTION) {
			lua_pushvalue(Game::Lua::State, -2);
			LuaHelper::PCallLog(Game::Lua::State, 1, 0);
		}

		lua_settop(Game::Lua::State, StackTop);
	}
	lua_settop(Game::Lua::State, StackTop - 1);
}

#ifndef BUILD_SERVER
void Game::Lua::Draw() {

	lua_getglobal(Game::Lua::State, "Game");
	lua_pushnil(Game::Lua::State);

	const int StackTop = lua_gettop(Game::Lua::State);
	while (lua_next(Game::Lua::State, -2) != 0) {

		if (lua_getfield(Game::Lua::State, -1, "Draw") == LUA_TFUNCTION) {
			lua_pushvalue(Game::Lua::State, -2);
			LuaHelper::PCallLog(Game::Lua::State, 1, 0);
		}

		lua_settop(Game::Lua::State, StackTop);
	}
	lua_settop(Game::Lua::State, StackTop - 2);
	/*
	lua_getglobal(Game::Lua::State, "Game");
	lua_getfield(Game::Lua::State, -1, "Camera");
	lua_getfield(Game::Lua::State, -1, "Draw");
	lua_pushvalue(Game::Lua::State, -2);

	LuaHelper::PCallLog(Game::Lua::State, 1, 0);
	lua_pop(Game::Lua::State, 2);*/
}
#endif



void Game::Lua::Destroy() {

	if (Game::Lua::State != NULL) {
		lua_close(Game::Lua::State);
		Game::Lua::State = NULL;
	}
}