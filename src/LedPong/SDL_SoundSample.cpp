#include "SDL_SoundSample.h"

SDL_SoundSample::SDL_SoundSample(const std::string& path, int volume)
    : chunk(Mix_LoadWAV(path.c_str()), Mix_FreeChunk) {
    if (chunk == nullptr || !chunk.get()) {
        printf("Couldn't load audio sample: %s", path.c_str());
        // LOG("Couldn't load audio sample: ", path);
        return;
    }

    Mix_VolumeChunk(chunk.get(), volume);
}

// -1 here means we let SDL_mixer pick the first channel that is free
// If no channel is free it'll return an err code.
void SDL_SoundSample::play() {
    Mix_PlayChannel(-1, chunk.get(), 0);
}

void SDL_SoundSample::play(int times) {
    Mix_PlayChannel(-1, chunk.get(), times - 1);
}

void SDL_SoundSample::set_volume(int volume) {
    Mix_VolumeChunk(chunk.get(), volume);
}