#pragma once
#include "SDL3/SDL.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "data.hpp"
#include <cstdint>

namespace Asset
{
    enum class Identifier : std::uint8_t
    {
        Logo,
        Background,
        MusicEasy,
        MusicMedium,
        MusicHard,
        InputFiles,
    };

    void queueToLoad(Identifier);

    void beginLoadProcess();

    SDL_Texture* getTextureById(Identifier);

    Mix_Music* getMusicById(Identifier);

    Toml::Data getRandomData();
}
