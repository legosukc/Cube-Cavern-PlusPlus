# Cube Cavern++

Mr Washee Washee's Super Cowboy USA Hot Dog Rocket Ship American Cleaners Number One Cube Cavern engine.

made by the same guy who made Mr Washee Washee's Cube Cavern on roblo-

![](/GithubAssets/imout.png)

trust me bro this engine is in goodddd hands

This engine aims to have a flexible Lua modding api, with multiplayer.

***maybe someday this will be used for other cc fangames...***

Engine being used for the game Cube Cavern++ (not to be confused with this engine, I still need a name for the game)

Engine written in C++, Luau for all the game logic and scripting.

OpenAL and SDL_mixer for sound.

Luau API for modding, OpenGL knowledge is recommended. (You could probably get away with copy and pasting rendering code from the base Lua scripts.)


## TODO:

- multiplayer
- colision
- migrate to luau
- poo

## BUILDING:

You will need CMake, make sure you got that installed.

### Linux:

Run this command to install required packages (adapt to your package manager)

todo: make this use codeblock

sudo pacman -Syu assimp sdl3 sdl3_image sdl3_mixer sdl3_ttf openal luau

SDL3_net doesn't seem to have a package, so you will need to compile and install it yerself.

You might also need to compile Luau yourself. (when i install the luau package it only seems to include terminal tools)

Run either "compile-(client|server).sh".

There's also a tasks.json file in the ".vscode" folder that includes tasks for compiling for client/server for vscode.

### Windows:

todo: make guide

use WSL and follow the linux guide ig

probably won't spit out an .exe and instead whatever linux uses so

### Mac:

idk 🖕