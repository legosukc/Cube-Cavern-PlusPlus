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

set(LUAU_BUILD_CLI OFF CACHE INTERNAL "")
set(LUAU_BUILD_TESTS OFF CACHE INTERNAL "")

if(PLATFORM STREQUAL "vita")
  set(LUAU_EXTERN_C OFF CACHE INTERNAL "")
  set(LUAU_BUILD_SHARED OFF CACHE INTERNAL "")
  set(LUAU_BUILD_STATIC ON CACHE INTERNAL "")

  add_compile_definitions(VM_HAS_NATIVE=0)
else()
  set(LUAU_EXTERN_C ON CACHE INTERNAL "")
  set(LUAU_BUILD_SHARED ON CACHE INTERNAL "")
  set(LUAU_BUILD_STATIC OFF CACHE INTERNAL "")
endif()

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

if(BUILD_TARGET STREQUAL "client")

  message("building for client")

  target_sources(${PROJECT_NAME} PRIVATE

    src/Client/SDLClasses/Window.cpp
    src/Client/Network_Client.cpp

    src/Client/GraphicsClasses/BufferBase.cpp
    src/Client/GraphicsClasses/VertexArray.cpp
    src/Client/GraphicsClasses/VertexBuffer.cpp
    src/Client/GraphicsClasses/IndexBuffer.cpp
    src/Client/GraphicsClasses/UniformBuffer.cpp

    src/Client/GraphicsClasses/ShaderBase.cpp
    src/Client/GraphicsClasses/VertexShader.cpp
    src/Client/GraphicsClasses/FragmentShader.cpp

    src/Client/GraphicsClasses/Program.cpp

    src/Client/GraphicsClasses/Texture.cpp
    src/Client/GraphicsClasses/Framebuffer.cpp

    src/Client/Graphics_Client.cpp
    src/Client/GraphicsOpenGL_Client.cpp

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

target_sources(${PROJECT_NAME} PRIVATE

  src/FunctionHeaders/File.cpp
  src/FunctionHeaders/ConfigHandler.cpp
  src/FunctionHeaders/LuaHelper.cpp

  src/Collisions.cpp
  src/Statistics.cpp
  src/ConsoleVars.cpp
  src/Game.cpp
  src/Time.cpp

  src/LuaLibraries/LuaConsole.cpp
  src/LuaLibraries/LuaEnums.cpp
  src/LuaLibraries/LuaFont.cpp
  src/LuaLibraries/LuaGraphics.cpp
  src/LuaLibraries/LuaHostUtil.cpp
  src/LuaLibraries/LuaInput.cpp
  src/LuaLibraries/LuaMatrix.cpp
  src/LuaLibraries/LuaModelLoader.cpp
  src/LuaLibraries/LuaNetwork.cpp
  src/LuaLibraries/LuaPatching.cpp
  src/LuaLibraries/LuaSurface.cpp
  src/LuaLibraries/LuaTask.cpp
  src/LuaLibraries/LuaTypeHelper.cpp
  src/LuaLibraries/LuaVector.cpp

  src/LuaLibraries/LuaGraphics_Classes/LuaGraphics_BufferBase.cpp
  src/LuaLibraries/LuaGraphics_Classes/LuaGraphics_GLObjectBase.cpp
  src/LuaLibraries/LuaGraphics_Classes/LuaGraphics_Program.cpp
  src/LuaLibraries/LuaGraphics_Classes/LuaGraphics_Shader.cpp
  src/LuaLibraries/LuaGraphics_Classes/LuaGraphics_Texture.cpp
  src/LuaLibraries/LuaGraphics_Classes/LuaGraphics_UniformBuffer.cpp
  src/LuaLibraries/LuaGraphics_Classes/LuaGraphics_VertexArray.cpp
  
  src/LuaLibraries/LuaGraphics_MiscHeaders/LuaGraphics_SampleModel.cpp

  src/Lua.cpp

  #src/FunctionHeaders/LuaHelper.cpp
)

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

  vita_create_self(eboot.bin ${PROJECT_NAME} UNSAFE)
  vita_create_vpk(${VITA_VPKNAME}.vpk ${VITA_TITLEID} eboot.bin
    NAME ${VITA_APP_NAME}
    VERSION ${VITA_VERSION}

    FILE vita-assets/ .
    FILE ccbeyond-assets/ .
  )
endif()
