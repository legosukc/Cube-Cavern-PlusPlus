cmake_minimum_required(VERSION 3.5)

# include vita toolchain
if(PLATFORM STREQUAL "vita" AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()


include(FetchContent)

# 1. Declare the Luau repository and release tag
FetchContent_Declare(
    luau
    GIT_REPOSITORY https://github.com/luau-lang/luau.git
    GIT_TAG        0.729 # Replace with the latest stable version tag
)

# 2. Prevent Luau from compiling its standalone CLI binaries and test suites
set(LUAU_BUILD_CLI OFF CACHE INTERNAL "")
set(LUAU_BUILD_TESTS OFF CACHE INTERNAL "")

# 3. Make Luau targets available to your project
FetchContent_MakeAvailable(luau)


project(CubeCavern LANGUAGES CXX)

if(PLATFORM STREQUAL "vita")
  # This line adds Vita helper macros, must go after project definition in order
  # to build Vita specific artifacts (self/vpk).
  include("${VITASDK}/share/vita.cmake" REQUIRED)

  set(VITA_APP_NAME "Cube Cavern++ CLIENT")

  # Optional. You can specify more param.sfo flags this way.
  set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")
endif()

## Flags and includes for building
# Note that we make sure not to overwrite previous flags
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(BUILD_TARGET STREQUAL "client")
    add_executable(CubeCavernApp

      src/Client/main_client.cpp

      src/FunctionHeaders/File.cpp
      src/FunctionHeaders/ConfigHandler.cpp
      src/FunctionHeaders/LuaHelper.cpp

      src/Client/SDLClasses/Window.cpp

      src/Client/GraphicsClasses/BaseClassDefinitions.cpp
      src/Client/GraphicsClasses/BufferBase.cpp

      src/Client/GraphicsClasses/VertexArray.cpp
      src/Client/GraphicsClasses/VertexBuffer.cpp
      src/Client/GraphicsClasses/IndexBuffer.cpp

      src/Client/GraphicsClasses/ShaderBase.cpp
      src/Client/GraphicsClasses/VertexShader.cpp
      src/Client/GraphicsClasses/FragmentShader.cpp

      src/Client/GraphicsClasses/Program.cpp

      src/Client/GraphicsClasses/Texture.cpp
      src/Client/GraphicsClasses/Framebuffer.cpp

      src/Game.cpp
      src/Time.cpp
      src/Statistics.cpp
      src/Client/Graphics_Client.cpp
      src/Lua.cpp
    )
    target_compile_definitions(CubeCavernApp PRIVATE BUILD_CLIENT)

elseif(BUILD_TARGET STREQUAL "server")
    add_executable(CubeCavernApp

      src/Server/main_server.cpp

      src/FunctionHeaders/File.cpp
      src/FunctionHeaders/ConfigHandler.cpp
      src/FunctionHeaders/LuaHelper.cpp

      src/Game.cpp
      src/Time.cpp
      src/Statistics.cpp
      src/Lua.cpp
    )
    target_compile_definitions(CubeCavernApp PRIVATE BUILD_SERVER)

else()
    message(FATAL_ERROR, "'BUILD_TARGET' unspecified! Add '-DBUILD_TARGET=client|server'.")
endif()

target_link_libraries(CubeCavernApp PRIVATE stdc++)

target_link_libraries(CubeCavernApp PRIVATE assimp)

# 5. Link the specific modules your project requires
# - Luau.VM: Core runtime engine
# - Luau.Compiler: Compiles source text to bytecode
target_link_libraries(CubeCavernApp PRIVATE Luau.VM Luau.Compiler)

target_link_libraries(CubeCavernApp PRIVATE SDL3 SDL3_net SDL3_image SDL3_mixer SDL3_ttf)

#target_link_libraries(CubeCavernApp PRIVATE OpenAL)

target_include_directories(CubeCavernApp PRIVATE /include/ ${CMAKE_CURRENT_SOURCE_DIR}/include/)


if(PLATFORM STREQUAL "vita")

  vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
  # The FILE directive lets you add additional files to the VPK, the syntax is
  # FILE src_path dst_path_in_vpk. In this case, we add the LiveArea paths.
  vita_create_vpk(${PROJECT_NAME}.vpk "WORM02011" ${PROJECT_NAME}.self
    VERSION 01.00
    NAME "Cube Cavern (" PLATFORM ")"
    # FILE sce_sys/icon0.png sce_sys/icon0.png
    # FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
    # FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
    # FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
  )
endif()
