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

You will need CMake SDL3, SDL3_net, SDL3_mixer, SDL3_ttf, SDL3_image, and OpenAL installed.

### Windows:

idk, maybe use wsl and follow the linux guide.

### Linux:

Install dependencies, adapt this command to whatever you're using.

sudo pacman -Syu cmake sdl3 sdl3_mixer sdl3_ttf sdl3_image openal

If it fails, try compiling them yourself and installing them.

Can't seem to download the SDL3_net package, so install it manually through compiling it.

You might need to move the SDL3_net library files from /usr/local/lib/ to /usr/lib/.

After that, run compile-client.sh or compile-server.sh to spit out an executable.

Has build tasks out of the box for VSCode, open up the root folder (the one above src/).

Hit CTRL+SHIFT+B to build, and F5 to run or CTRL+F5 to run in debugging mode (these are the default VSCode binds, check your config if it doesn't work).

If for some reason you want to compile it without the scripts, here:

mkdir build
cmake -S./ -Bbuild/ -DBUILD_TARGET=("client" or "server") (optional)PLATFORM=("vita", or can be left out entirely)
cmake --build build/

### Mac:

idk 🖕