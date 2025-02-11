#pragma once
#include "SDL3/SDL.h"
#include "textureregion.hpp"
#include <cstdint>
#include <string>

enum WhereToLoadTextureFrom
{
    FromDisk,
    FromText,
};

struct LoadProcess
{
    WhereToLoadTextureFrom Source;
    std::string Asset;
    TextureRegion* Output;
    SDL_ScaleMode Filter;
    SDL_FRect HoldingArea;
};

namespace Texture
{
    void convertSurfaceToTexture(SDL_Surface* surface, SDL_Texture** outTexture, SDL_ScaleMode filter);

    void queueToLoad(LoadProcess&& process);

    void beginLoadProcess();

    SDL_FRect initializeAreaFromSurface(SDL_FRect area, int32_t width, int32_t height);
}
