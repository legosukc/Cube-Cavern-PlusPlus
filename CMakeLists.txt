cmake_minimum_required(VERSION 3.16)

# include vita toolchain
if(PLATFORM STREQUAL "vita" AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  message("Platform: PSVita")
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()

  set(VITA_MEMENTRY_MEMSIZE "49152000" CACHE STRING "Memory size option")
endif()


project(CubeCavern LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ps vita shtuff (taken from documentation)
if(PLATFORM STREQUAL "vita")
  # add vita helper macros
  include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

  set(VITA_APP_NAME "Cube Cavern Beyond")
  set(VITA_TITLEID "WORM02011")
  set(VITA_VPKNAME "CUBECAVER")
  set(VITA_VERSION "01.00")

  set(VITA_MKSFOEX_FLAGS "-d ATTRIBUTE2=12")

  # Optional. You can specify more param.sfo flags this way.
  #set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")
endif()

include(FetchContent)

message("Fetching luau...")
if(PLATFORM STREQUAL "vita")
  FetchContent_Declare(
    luau
    GIT_REPOSITORY https://github.com/legosukc/luau-psvita.git
    GIT_TAG 0.738
  )
else()
  FetchContent_Declare(
    luau
    GIT_REPOSITORY https://github.com/luau-lang/luau.git
    GIT_TAG 0.742
  )
endif()

# ai told me to put this here...
# Prevent Luau from compiling its standalone CLI binaries and test suites
set(LUAU_BUILD_CLI OFF CACHE INTERNAL "")
set(LUAU_BUILD_TESTS OFF CACHE INTERNAL "")

if(PLATFORM STREQUAL "vita")
  set(LUAU_EXTERN_C OFF CACHE INTERNAL "")
  set(LUAU_BUILD_SHARED OFF CACHE INTERNAL "")
  set(LUAU_BUILD_STATIC ON CACHE INTERNAL "")
else()
  set(LUAU_EXTERN_C ON CACHE INTERNAL "")
  set(LUAU_BUILD_SHARED ON CACHE INTERNAL "")
  set(LUAU_BUILD_STATIC OFF CACHE INTERNAL "")
endif()

set(LUAU_BUILD_CODEGEN OFF CACHE BOOL "Disable Luau native codegen" FORCE)

FetchContent_MakeAvailable(luau)
message("Fetched luau!")

find_package(assimp REQUIRED)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)

if(NOT PLATFORM STREQUAL "vita")
  find_package(SDL3_mixer REQUIRED)
  find_package(SDL3_net REQUIRED)
endif()

find_package(OpenAL REQUIRED)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE

  src/MathClasses/Vector2.cpp

  src/FunctionHeaders/File.cpp
  src/FunctionHeaders/ConfigHandler.cpp

  src/Statistics.cpp
  src/ConsoleVars.cpp
  #src/Lua.cpp

  #src/LuaLibraries/LuaConsole.cpp
  #src/LuaLibraries/LuaEnums.cpp
  #src/LuaLibraries/LuaFont.cpp
  #src/LuaLibraries/LuaGraphics.cpp

  #src/FunctionHeaders/LuaHelper.cpp
)

if(BUILD_TARGET STREQUAL "client")

  message("building for client")

  target_sources(${PROJECT_NAME} PRIVATE
    src/Client/SDLClasses/Window.cpp
    src/Client/Network_Client.cpp
    src/Client/main_client.cpp
  )

  target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_CLIENT)

elseif(BUILD_TARGET STREQUAL "server")

  message("building for server")

  target_sources(${PROJECT_NAME} PRIVATE src/Server/main_server.cpp)
  target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_SERVER)
  
  set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "CubeCavern-DedicatedServer"
  )
else()
  message(FATAL_ERROR, "'BUILD_TARGET' unspecified! Add '-DBUILD_TARGET=client|server'.")
endif()

if(NOT PLATFORM STREQUAL "vita")

  target_link_libraries(${PROJECT_NAME} PRIVATE
    #SDL3_net::SDL3_net
    SDL3_mixer::SDL3_mixer

    OpenAL::OpenAL
  )
  target_compile_definitions(${PROJECT_NAME} PRIVATE NETWORKING_DISABLED)
else()
  target_compile_definitions(${PROJECT_NAME} PRIVATE NETWORKING_DISABLED)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
  SDL3::SDL3
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  Luau.VM Luau.Compiler
  assimp
)

if(PLATFORM STREQUAL "vita")
  target_link_libraries(${PROJECT_NAME} PRIVATE
    stdc++ mathneon
    c m
    zip vitashark SceShaccCgExt taihen_stub

    minizip
    ppmd
    webp
    sharpyuv
    zstd
    lzma
    bz2
    z # zlib
    crypto

    -Wl,--whole-archive pthread -Wl,--no-whole-archive

    SceKernelDmacMgr_stub
    SceCtrl_stub
    SceShaccCg_stub
    SceCommonDialog_stub
    SceSysmodule_stub
    SceKernelThreadMgr_stub
    SceAppMgr_stub
    SceAppUtil_stub
    SceLibKernel_stub
    SceDisplay_stub
    SceGxm_stub

    libIMGEGL_stub_weak
    libgpu_es4_ext_stub_weak
    libGLESv2_stub_weak
  )
endif()


if(PLATFORM STREQUAL "vita")

  message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})

  vita_create_self(eboot.bin ${PROJECT_NAME} UNSAFE)
  vita_create_vpk(${VITA_VPKNAME}.vpk ${VITA_TITLEID} eboot.bin
    NAME ${VITA_APP_NAME}
    VERSION ${VITA_VERSION}

    FILE vita-assets/ .
    FILE ccbeyond-assets/ .
  )
endif()
