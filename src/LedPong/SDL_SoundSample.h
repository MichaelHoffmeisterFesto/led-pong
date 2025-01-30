#pragma once
#include <string>
#include <memory>
#include <SDL.h>
#include <SDL_mixer.h>

class SDL_SoundSample
{
public:
    SDL_SoundSample(const std::string& path, int volume);
    void play();
    void play(int times);
    void set_volume(int volume);

private:
    std::unique_ptr<Mix_Chunk, void (*)(Mix_Chunk*)> chunk;
};

