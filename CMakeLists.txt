cmake_minimum_required(VERSION 3.10)

# include vita toolchain
if(PLATFORM STREQUAL "vita" AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  message("Platform: PSVita")
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()


project(CubeCavern LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    sdl3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.14
)

FetchContent_Declare(
    sdl3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG        release-3.4.4
)

FetchContent_Declare(
    sdl3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG        release-3.2.4
)

FetchContent_Declare(
    sdl3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG        release-3.2.2
)

FetchContent_Declare(
    sdl3_net
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_net.git
    GIT_TAG        release-3.2.0
)

FetchContent_MakeAvailable(sdl3)
FetchContent_MakeAvailable(sdl3_image)
FetchContent_MakeAvailable(sdl3_mixer)
FetchContent_MakeAvailable(sdl3_ttf)
FetchContent_MakeAvailable(sdl3_net)



FetchContent_Declare(
    luau
    GIT_REPOSITORY https://github.com/luau-lang/luau.git
    GIT_TAG        0.733
)

# ai told me to put this here... ()
# Prevent Luau from compiling its standalone CLI binaries and test suites
set(LUAU_BUILD_CLI OFF CACHE INTERNAL "")
set(LUAU_BUILD_TESTS OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(luau)


find_package(OpenAL REQUIRED)


# ps vita shtuff (taken from documentation)
if(PLATFORM STREQUAL "vita")
  # add vita helper macros
  include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

  set(VITA_APP_NAME "Cube Cavern++ CLIENT")

  # Optional. You can specify more param.sfo flags this way.
  set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(CubeCavernApp)


target_link_libraries(CubeCavernApp PRIVATE stdc++)
target_link_libraries(CubeCavernApp PRIVATE assimp)
target_link_libraries(CubeCavernApp PRIVATE Luau.VM Luau.Compiler)
target_link_libraries(CubeCavernApp PRIVATE SDL3 SDL3_net SDL3_image SDL3_mixer SDL3_ttf)
target_link_libraries(CubeCavernApp PRIVATE OpenAL::OpenAL)

if(BUILD_TARGET STREQUAL "client")

    message("building for client")

    target_sources(CubeCavernApp PRIVATE src/Client/main_client.cpp)
    target_compile_definitions(CubeCavernApp PRIVATE BUILD_CLIENT)

    set_target_properties(CubeCavernApp PROPERTIES
      OUTPUT_NAME "CubeCavern"
    )

elseif(BUILD_TARGET STREQUAL "server")

    message("building for server")

    target_sources(CubeCavernApp PRIVATE src/Server/main_server.cpp)
    target_compile_definitions(CubeCavernApp PRIVATE BUILD_SERVER)

    set_target_properties(CubeCavernApp PROPERTIES
      OUTPUT_NAME "CubeCavern-DedicatedServer"
    )

else()
    message(FATAL_ERROR, "'BUILD_TARGET' unspecified! Add '-DBUILD_TARGET=client|server'.")
endif()


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
