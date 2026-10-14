
test

# Cube Cavern++

originally (not) Mr Washee Washee's Super Cowboy USA Hot Dog Rocket Ship American Cleaners Number One Cube Cavern engine

This engine aims to have a flexible Lua modding api, with multiplayer.

***maybe someday this will be used for other cc fangames...*** *wink wink*

Engine being used for my game: Cube Cavern & Beyond

Engine written in C++, Luau for all the game logic and scripting.

OpenAL and SDL_mixer for sound.

Luau API for modding, OpenGL knowledge is recommended, but you could probably slip by through copying rendering code from the base scripts.

## TODO:

- networkinggggggg
- [port luau to the ps vita](https://github.com/legosukc/luau-psvita)
- everything :(

## BUILDING:

You will need CMake and a C++ compiler installed.

### Windows:

idk, maybe use wsl and follow the linux guide.

### Linux:

You need the SDL3 library family installed.

Run the compile-(environment)-(platform).sh scripts to compile.

The compile-(environment).sh scripts compile for your local platform.

When you build for the first time **IT WILL TAKE A WHILE.**

It how long it takes depends on your internet connection and your CPU strength, as it is compiling dependenices. (Luau)

**It's normal for there to be not much console output for some time**, *this is CMake downloading dependencies.*

### Mac:

idk 🖕


## COMPILING FOR PLATFORMS:

### PS VITA:

If you are on windows, install [WSL](https://learn.microsoft.com/en-us/windows/wsl/install) and continue the guide.

For Mac users, idk, probably possible with brew or MacPorts (🖕🖕🖕🖕🖕🖕🖕🖕🖕🖕🖕🖕🖕🖕🖕🖕🖕)

You will need to install vitasdk, [click here for the install guide.](https://vitasdk.org/)

After that, install the dependencies using this command:

todo: figure out how to use codeblocks formatting and list all of the dependecies (i forgot them!)

vdpm install SDL3 SDL3_image SDL3_ttf libwebp
