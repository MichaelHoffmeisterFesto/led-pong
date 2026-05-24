# led-pong

## About

LedPong is an educational arcade game for LED walls

## Compiling

### Compiling in Windows

There is a solution file provided for Visual Studio.

When compiling, the "x86" platform needs to be selected

In project properties/ debugging / environment
* PATH=%PATH%;\$(ProjectDir)..\\SDL2\\lib\\x86
$(LocalDebuggerEnvironment)

In project properties/ configuration/ c/c++ / pre-processor
* WIN32;_DEBUG;_CONSOLE;%(PreprocessorDefinitions);Use_SDL_Grafix

In project properties/ configuration/ linker / input
* SDL2.lib;SDL2main.lib;SDL2_ttf.lib;SDL2_mixer.lib;%(AdditionalDependencies)

