# Cube Cavern++

Mr Washee Washee's Super Cowboy USA Hot Dog Rocket Ship American Cleaners Number One Cube Cavern engine.

made by the same guy who made Mr Washee Washee's Cube Cavern on roblo-

![](/GithubAssets/imout.png)

trust me bro this engine is in goodddd hands

This engine aims to have a flexible Lua modding api, with multiplayer.

***maybe someday this will be used for other cc fangames...***

Engine being used for the game Cube Cavern++ (i need a name for the game, so it is the same as the engine for now)

Engine written in C++, Luau for all the game logic and scripting.

OpenAL and SDL_mixer for sound.

Luau API for modding, OpenGL knowledge is recommended. (You could probably get away with copy and pasting rendering code from the base Lua scripts.)


## TODO:

- networkinggggggg
- model loading
- everything :(

## BUILDING:

You will need CMake and a C++ compiler installed. (i use clang, so maybe use that)

### Windows:

idk, maybe use wsl and follow the linux guide.

### Linux:

You need the SDL3 library family (except SDL3_net, that one is installed and compiled in the cmake scripts.) installed.

Run the compile-(environment)-(platform).sh scripts to compile.

The compile-(environment).sh scripts compile for your local platform.

When you build for the first time **IT WILL TAKE A WHILE.**

It how long it takes depends on your internet connection and your CPU power, as it is compiling all the dependenices. (SDL3_net and Luau)

**It's normal for there to be not much console output for some time**, *this is CMake downloading dependencies.*

### Mac:

idk 🖕


## COMPILING FOR PLATFORMS:

### PS VITA:

If you are on windows, install [WSL](https://learn.microsoft.com/en-us/windows/wsl/install) and continue the guide.

For Mac users, idk, probably possible with brew or MacPorts

You will need to install vitasdk, [click here for the install guide.](https://vitasdk.org/)