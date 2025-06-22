#pragma once
#include "SDL3/SDL.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "opengl.hpp"
#include "toml.hpp"
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
        CorrectAnswer,
        WrongAnswer,
        FinalAnswer,
        HELP_AUDIENCE,
        HELP_PHONE,
    };

    void queueToLoad(Identifier);

    void beginLoadProcess();

    OpenGL::TextureGpu getTextureById(Identifier);

    Mix_Music* getMusicById(Identifier);

    Mix_Chunk* getSfxById(Identifier);

    Toml::Data getRandomData();
}
