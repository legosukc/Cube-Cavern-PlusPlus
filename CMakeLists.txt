cmake_minimum_required(VERSION 3.5)

# include vita toolchain
if(PLATFORM STREQUAL "vita" AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(CubeCavern CXX)

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
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(BUILD_TARGET STREQUAL "client")
    add_executable(CubeCavernApp src/Client/main_client.cpp)
elseif(BUILD_TARGET STREQUAL "server")
    add_executable(CubeCavernApp src/Server/main_server.cpp)
else()
    message(FATAL_ERROR, "'BUILD_TARGET' unspecified! Add '-DBUILD_TARGET=client|server'.")
endif()

target_link_libraries(CubeCavernApp PRIVATE stdc++)

target_link_libraries(CubeCavernApp PRIVATE assimp)

# target_link_libraries(CubeCavernApp PRIVATE Luau.Compiler)
# target_link_libraries(CubeCavernApp PRIVATE Luau.VM)

target_link_libraries(CubeCavernApp PRIVATE SDL3)
target_link_libraries(CubeCavernApp PRIVATE SDL3_net)
target_link_libraries(CubeCavernApp PRIVATE SDL3_image)
target_link_libraries(CubeCavernApp PRIVATE SDL3_mixer)
target_link_libraries(CubeCavernApp PRIVATE SDL3_ttf)

target_link_libraries(CubeCavernApp PRIVATE OpenAL)

target_include_directories(CubeCavernApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)


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
