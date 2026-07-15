#pragma once

#include <random>
#include <fstream>

#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_filesystem.h>

#ifndef BUILD_SERVER
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif


#include <lua.hpp>

namespace Game::Lua {

	// Contains the correct directory paths depending on the build target (Client or Server)
	struct LuaDirectories {
#ifdef BUILD_CLIENT
		static inline const char* MiscClasses = "MiscClassesClient";
		static inline const char* GameClasses = "GameClassesClient";
		static inline const char* BaseClasses = "BaseClassesClient";
		static inline const char* ItemClasses = "ItemClassesClient";
		static inline const char* EnemyClasses = "EnemyClassesClient";
#else
		static inline const char* MiscClasses = "MiscClassesServer";
		static inline const char* GameClasses = "GameClassesServer";
		static inline const char* BaseClasses = "BaseClassesServer";
		static inline const char* ItemClasses = "ItemClassesServer";
		static inline const char* EnemyClasses = "EnemyClassesServer";
#endif
	};

	int CompileToLuaFunction(lua_State* State, const char* SourcePath);

	struct LuaThreadInfo {
		LuaThreadInfo() {

#ifndef BUILD_SERVER
			GLint MaxBoundTextures;
			Game::Graphics::OpenGLFunctions::glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MaxBoundTextures);

			this->ActiveTextures = static_cast<GLuint*>(std::calloc(0, MaxBoundTextures * sizeof(GLuint)));

			//this->BoundVertexArray = glGetVertexArrayiv()

			this->DepthTest = Game::Graphics::OpenGLFunctions::glIsEnabled(GL_DEPTH_TEST);
			this->StencilTest = Game::Graphics::OpenGLFunctions::glIsEnabled(GL_STENCIL_TEST);
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

	// If true, when lua code is compiled the bytecode will be stored to disk. Improves future loading times at the cost of disk space.
	bool CacheLuaBytecode = false;

	// Controls the max amount of storage space for lua bytecode caching.
	Uint16 MaxTotalCacheSizeMB = 20;

	void* LuaAllocationFunc(void*, void* ptr, size_t, size_t nsize);

	inline void Init();
	inline void Destroy();

	inline void Update();
#ifndef BUILD_SERVER
	void Draw();
#endif

	lua_State* State = NULL;
}

#include "FunctionHeaders/LuaHelper.hpp"
#include "FunctionHeaders/ConfigHandler.hpp"


// uhh keep this for "ClientDownloadsLuaBytecode" config comment
// If true, sends pre-compiled byte code for the client, no compilation on the client needed.
	// If false, sends the entire lua source code from mods for the client to compile.
	// Bytecode is often smaller than source code, however, all client code will have to be compiled on the server on start-up.
	// After compilation, bytecode will be cached, you can configure cache settings in "GlobalCfg.json"

namespace Game::Lua {
	LuaHelper::StackTableReference GameTable;
}

#include "LuaLibraries/LuaMath.hpp"
#include "LuaLibraries/LuaTable.hpp"
#include "LuaLibraries/LuaEnums.hpp"
#include "LuaLibraries/LuaTask.hpp"
#include "LuaLibraries/LuaTypeHelper.hpp"

#include "LuaLibraries/LuaVector.hpp"
#include "LuaLibraries/LuaMatrix.hpp"

#include "LuaLibraries/LuaBuffer.hpp"

#include "LuaLibraries/LuaModelLoader.hpp"

#include "LuaLibraries/LuaGraphics.hpp"
#include "LuaLibraries/LuaInput.hpp"

#include "LuaLibraries/LuaConsole.hpp"

#ifndef BUILD_SERVER

#include "Client/LuaLibraries/LuaNetwork_Client.hpp"
#include "Client/LuaLibraries/LuaSound_Client.hpp"
#else

#include "Server/LuaLibraries/LuaNetwork_Server.hpp"
#include "Server/LuaLibraries/LuaSound_Server.hpp"
#endif

namespace {

	static void _LoadLuaFileInTable(const std::filesystem::directory_entry& Entry, int TableIDX) {

		const int StartingStackIndex = lua_gettop(Game::Lua::State);

		const std::filesystem::path& Path = Entry.path();


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
			goto _Exit;
		}
		lua_settop(Game::Lua::State, StartingStackIndex);

		std::cout << "Loading " << Path.stem().generic_string().c_str() << std::endl;

		unlikely_branch
		if (LuaHelper::LoadFileLog(Game::Lua::State, Path.generic_string().c_str()) != LUA_OK) {
			std::clog << "Failed to load " << Path << std::endl;
			goto _Exit;
		}

		unlikely_branch
		if (LuaHelper::PCallLog(Game::Lua::State, 0, 1) != LUA_OK) {
			goto _Exit;
		}
	
		if (!lua_istable(Game::Lua::State, -1)) {
			std::clog << Path
				<< " returned type '" << luaL_typename(Game::Lua::State, -1)
				<< "' instead of type 'table', is this intended behaviour? (Ignore this warning if it is)" << std::endl;
		}

		lua_setfield(Game::Lua::State, TableIDX, Path.stem().generic_string().c_str());

		_Exit:
		lua_settop(Game::Lua::State, StartingStackIndex);
	}

	static void _LoadLuaAssetDirectoryInTable(const char* DirectoryPath, int TableIDX) {

		SDL_PathInfo DirectoryInfo;
		
		unlikely_branch
		if (!SDL_GetPathInfo(DirectoryPath, &DirectoryInfo)) {

			char* WorkingDir = SDL_GetCurrentDirectory();

			std::string ErrorMsg = StringHelper::Combine("Directory ", DirectoryPath, " doesn't exist.\nWorking directory: ", WorkingDir);
			SDL_free(WorkingDir);

			std::cerr << ErrorMsg << std::endl;

			Exceptions::ThrowExceptionExpression<Exceptions::IOError>(ErrorMsg);
		}

		unlikely_branch
		if (DirectoryInfo.type != SDL_PATHTYPE_DIRECTORY) {

			char* WorkingDir = SDL_GetCurrentDirectory();

			std::string ErrorMsg = StringHelper::Combine("Path ", DirectoryPath, " isn't a directory.\nWorking directory: ", WorkingDir);
			SDL_free(WorkingDir);

			std::cerr << ErrorMsg << std::endl;

			Exceptions::ThrowExceptionExpression<Exceptions::IOError>(ErrorMsg);
		}

		SDL_EnumerateDirectory(DirectoryPath, [](void* Userdata, const char* DirectoryPath, const char* FileName) -> SDL_EnumerationResult {
			//std::cout << "dir: " << DirectoryPath << std::endl;
			::_LoadLuaFileInTable(std::filesystem::directory_entry(std::string(DirectoryPath) + FileName), *(int*)Userdata);
			return SDL_EnumerationResult::SDL_ENUM_CONTINUE;
			}, &TableIDX
		);
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

		this->Path = _Name;
		this->Path.push_back('\\');

		lua_pushlstring(Game::Lua::State, this->Path.c_str(), this->Path.size() + 1);
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
			if (!SDL_GetPathInfo(Path.c_str(), NULL)) {
				luaL_error(State, "Attempted to index a non-existant entry '", lua_tostring(State, 2), "' in table ", lua_tostring(State, lua_upvalueindex(1)));
			}

			unlikely_branch
			//if (luaL_loadfile(State, Path.c_str()) != LUA_OK) {
			if (Game::Lua::CompileToLuaFunction(State, Path.c_str()) != LUA_OK) {
				lua_error(State);
			}

			lua_call(State, 0, 1);
			lua_pushvalue(State, 2);
			lua_pushvalue(State, -2);
			lua_rawset(State, 1);
		}

		//lua_rotate(State, 1, 1);
		return 1;
	}
}


//static std::ofstream _AllocLogFile;
void* Game::Lua::LuaAllocationFunc(void* ud, void* ptr, size_t osize, size_t nsize) {
	using namespace Game::Statistics;

	const Uint64 StartNS = SDL_GetTicksNS();

	if (nsize == 0) {

		if (ptr != NULL) {
			ptr = static_cast<char*>(ptr) - sizeof(size_t);
			//::_AllocLogFile << "free: " << reinterpret_cast<void*>(static_cast<char*>(ptr) + sizeof(size_t)) << ", " << ptr << std::endl;
			//std::cout << "free: " << reinterpret_cast<void*>(static_cast<char*>(ptr) + sizeof(size_t)) << ", " << ptr << std::endl;

			Statistics::Memory::UpdateLuaStatsDeallocation(static_cast<Uint64>(*static_cast<size_t*>(ptr)));
			std::free(ptr);
		}

		Statistics::Memory::LuaNSSpentOnHeapPerFrame += SDL_GetTicksNS() - StartNS;
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
	Statistics::Memory::LuaNSSpentOnHeapPerFrame += SDL_GetTicksNS() - StartNS;

	return static_cast<char*>(ptr) + sizeof(size_t);
}


void Game::Lua::Init() {

	const Uint64 StartMS = SDL_GetTicks();
	std::cout << "Initializing Lua." << std::endl;

	{
		const auto CacheLuaBytecodeValue = Game::Config::GlobalConfigJSON["Main"]["Lua"]["CacheLuaBytecode"];
		Game::Lua::CacheLuaBytecode = CacheLuaBytecodeValue.is_boolean() && CacheLuaBytecodeValue.get<bool>();
	}

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

	Lua::CLibraries::math::Init(Game::Lua::State);

	Lua::CLibraries::table::Init(Game::Lua::State);

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


	Lua::CLibraries::Enums::Init(Game::Lua::State);

	Lua::CLibraries::Task::Init(Game::Lua::State);

	Lua::CLibraries::TypeHelper::Init(Game::Lua::State);

	Lua::CLibraries::Vector::Init(Game::Lua::State);
	Lua::CLibraries::Matrix::Init(Game::Lua::State);

	Lua::CLibraries::Buffer::Init(Game::Lua::State);

	Lua::CLibraries::ModelLoader::Init(Game::Lua::State);

	Lua::CLibraries::Graphics::Init(Game::Lua::State);
	Lua::CLibraries::Input::Init(Game::Lua::State);


	::_LoadLuaAssetDirectoryInTable(Game::Lua::LuaDirectories::GameClasses, Game::Lua::GameTable.GetStackIndex());

	lua_createtable(Game::Lua::State, 0, 4); // Assets


	::_LuaDirectoryLibrary ItemClasses, EnemyClasses, MiscClasses, BaseClasses;
	int ParentTableIDX;

	ParentTableIDX = lua_gettop(Game::Lua::State);
	lua_pushvalue(Game::Lua::State, ParentTableIDX);
	lua_setglobal(Game::Lua::State, "Assets");

	ItemClasses = ::_LuaDirectoryLibrary(Game::Lua::LuaDirectories::ItemClasses, ParentTableIDX);
	EnemyClasses = ::_LuaDirectoryLibrary(Game::Lua::LuaDirectories::EnemyClasses, ParentTableIDX);
	MiscClasses = ::_LuaDirectoryLibrary("MiscClassesGlobal", ParentTableIDX);
	BaseClasses = ::_LuaDirectoryLibrary(Game::Lua::LuaDirectories::BaseClasses, ParentTableIDX);

	MiscClasses.Load();
	Lua::CLibraries::Console::PostMiscClassInit(Game::Lua::State);

	BaseClasses.Load();
	EnemyClasses.Load();
	ItemClasses.Load();

	std::cout << "Initalized Lua system, took " << SDL_GetTicks() - StartMS << " milliseconds." << std::endl;
}


int Game::Lua::CompileToLuaFunction(lua_State* State, const char* SourcePath) {

	struct BytecodeCacheStruct {
		char Header[14] = "BYTECODECACHE";
		Uint32 SourceCRC;
		size_t SourceSize;
		char Bytecode[];
	};

	SDL_PathInfo PathInfo;

	Uint32 SourceCodeCRC;
	size_t SourceCodeSize;
	char* SourceCode = static_cast<char*>(SDL_LoadFile(SourcePath, &SourceCodeSize));
	if (SourceCode == NULL) {
		return LUA_ERRFILE;
	}
	/*
	bool DoCache;
	{
		if (Game::Lua::CacheLuaBytecode) {
			SDL_GetPathInfo((Utils::File::PreferredPath + "LuaCache\\").c_str(), &PathInfo);
			
		}
	}*/

	std::string BytecodeCachePath;
	if (Game::Lua::CacheLuaBytecode) {

		SourceCodeCRC = SDL_murmur3_32(SourceCode, SourceCodeSize, 420);

		std::string TempPath;

		BytecodeCachePath = Utils::File::PreferredPath;
		BytecodeCachePath.append("LuaCache\\");

		if (!SDL_GetPathInfo(BytecodeCachePath.c_str(), &PathInfo) || PathInfo.type != SDL_PathType::SDL_PATHTYPE_DIRECTORY) {
			SDL_CreateDirectory(BytecodeCachePath.c_str());
			goto CheckForCacheBytecodeFail;
		}

		BytecodeCachePath.append(SourcePath);

		TempPath = std::filesystem::path(BytecodeCachePath).remove_filename().u8string();
		if (!SDL_GetPathInfo(TempPath.c_str(), &PathInfo) || PathInfo.type != SDL_PathType::SDL_PATHTYPE_FILE) {
			SDL_CreateDirectory(TempPath.c_str());
			goto CheckForCacheBytecodeFail;
		}

		size_t BytecodeCacheSize;
		BytecodeCacheStruct* BytecodeCache = static_cast<BytecodeCacheStruct*>(SDL_LoadFile(SourcePath, &BytecodeCacheSize));

		if (BytecodeCache == NULL) {
			goto CheckForCacheBytecodeFail;
		}

		if (strcmp(BytecodeCache->Header, "BYTECODECACHE") != 0 || SourceCodeCRC != BytecodeCache->SourceCRC) {
			goto CheckForCacheBytecodeFail_FreeBytecodeCache;
		}
		SDL_free(SourceCode);

		luaL_loadbuffer(State, BytecodeCache->Bytecode, BytecodeCacheSize - sizeof(Uint32), SourcePath);
		SDL_free(BytecodeCache);
		
		return LUA_OK;

	CheckForCacheBytecodeFail_FreeBytecodeCache:
		SDL_free(BytecodeCache);
	}
CheckForCacheBytecodeFail:

	//luaL_loadstring(State, SourceCode);
	
	//lua_getinfo
	const int CompileStatus = luaL_loadfile(State, SourcePath);
	if (CompileStatus != LUA_OK) {
		return CompileStatus;
	}

	SDL_free(SourceCode);

	if (Game::Lua::CacheLuaBytecode) {
		SDL_IOStream* BytecodeCacheIO = SDL_IOFromFile(BytecodeCachePath.c_str(), "wb");
		if (BytecodeCacheIO == NULL) {
			std::clog << "Failed to open lua bytecode cache file: " << BytecodeCachePath << " for writing." << std::endl;
			return LUA_OK;
		}
		BytecodeCacheStruct CacheInfo;
		CacheInfo.SourceCRC = SourceCodeCRC;
		CacheInfo.SourceSize = SourceCodeSize;
		SDL_WriteIO(BytecodeCacheIO, &CacheInfo, sizeof(CacheInfo.Header) + sizeof(CacheInfo.SourceCRC) + sizeof(CacheInfo.SourceSize));
		SDL_SeekIO(BytecodeCacheIO, sizeof(CacheInfo.Header) + sizeof(CacheInfo.SourceCRC) + sizeof(CacheInfo.SourceSize), SDL_IOWhence::SDL_IO_SEEK_SET);


		lua_dump(State, [](lua_State* State, const void* Chunk, size_t ChunkSize, void* BytecodeCacheIO) -> int {
			SDL_WriteIO(static_cast<SDL_IOStream*>(BytecodeCacheIO), Chunk, ChunkSize);
			SDL_SeekIO(static_cast<SDL_IOStream*>(BytecodeCacheIO), ChunkSize, SDL_IOWhence::SDL_IO_SEEK_CUR);
			return 0;
			},
			BytecodeCacheIO,
			false
		);

		SDL_CloseIO(BytecodeCacheIO);
	}

	return LUA_OK;
}


void Game::Lua::Update() {

	Game::Lua::GameTable.SetKey(Game::Lua::State, Game::DeltaTime, "DeltaTime");

#ifndef BUILD_SERVER
	CLibraries::Input::Update(Game::Lua::State);
#endif

	CLibraries::Task::Update(Game::Lua::State);

	lua_pushnil(Game::Lua::State);
	const int StackTop = lua_gettop(Game::Lua::State);
	while (lua_next(Game::Lua::State, Game::Lua::GameTable.GetStackIndex()) != 0) {

		if (lua_istable(State, -1)) {
			if (lua_getfield(Game::Lua::State, -1, "Update") == LUA_TFUNCTION) {
				lua_pushvalue(Game::Lua::State, -2);
				LuaHelper::PCallLog(Game::Lua::State, 1, 0);
			}
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

		if (lua_istable(State, -1)) {
			if (lua_getfield(Game::Lua::State, -1, "Draw") == LUA_TFUNCTION) {
				lua_pushvalue(Game::Lua::State, -2);
				LuaHelper::PCallLog(Game::Lua::State, 1, 0);
			}
		}

		lua_settop(Game::Lua::State, StackTop);
	}
	lua_settop(Game::Lua::State, StackTop - 2);
}
#endif



void Game::Lua::Destroy() {

	if (Game::Lua::State != NULL) {
		lua_close(Game::Lua::State);
		Game::Lua::State = NULL;
	}
}