cmake_minimum_required(VERSION 3.16)

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

message("Fetching luau...")
if (PLATFORM STREQUAL "vita")
FetchContent_Declare(
    luau
    GIT_REPOSITORY https://github.com/luau-lang/luau.git
    GIT_TAG        0.736
)
else()
FetchContent_Declare(
    luau
    GIT_REPOSITORY https://github.com/legosukc/luau-psvita/luau-psvita.git
    GIT_TAG        0.736
)
endif()

# ai told me to put this here...
# Prevent Luau from compiling its standalone CLI binaries and test suites
set(LUAU_BUILD_CLI OFF CACHE INTERNAL "")
set(LUAU_BUILD_TESTS OFF CACHE INTERNAL "")

if(PLATFORM STREQUAL "vita")
set(LUAU_BUILD_SHARED OFF CACHE INTERNAL "")
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


# ps vita shtuff (taken from documentation)
if(PLATFORM STREQUAL "vita")
  # add vita helper macros
  include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

  set(VITA_APP_NAME "Cube Cavern++ CLIENT")

  # Optional. You can specify more param.sfo flags this way.
  #set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE LUAU_ENABLE_CODEGEN=0)

if(BUILD_TARGET STREQUAL "client")

    message("building for client")

    target_sources(${PROJECT_NAME} PRIVATE src/Client/main_client.cpp)
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_CLIENT)

    set_target_properties(${PROJECT_NAME} PROPERTIES
      OUTPUT_NAME "CubeCavern"
    )

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

target_compile_definitions(Luau.CodeGen PRIVATE LUAU_ENABLE_CODEGEN=0)
target_compile_definitions(Luau.VM PRIVATE LUAU_ENABLE_CODEGEN=0)

if (NOT PLATFORM STREQUAL "vita")
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_net::SDL3_net SDL3_mixer::SDL3_mixer)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenAL::OpenAL)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
  SDL3::SDL3
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE

    Luau.VM Luau.Compiler

    -Wl,--start-group
    
    assimp
    minizip
    ppmd        # <--- ADD THIS HERE (must be after minizip)
    webp
    sharpyuv
    zstd
    lzma
    bz2
    z             # zlib
    crypto
    
    -Wl,--end-group
)

if (PLATFORM STREQUAL "vita")
  target_link_libraries(${PROJECT_NAME} PRIVATE

    -Wl,--whole-archive pthread -Wl,--no-whole-archive

    #SceKernelThreadMgr_stub
    SceLibKernel_stub
    SceDisplay_stub
    SceGxm_stub
  )
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
