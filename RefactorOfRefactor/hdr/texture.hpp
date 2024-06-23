#pragma once
#include "SDL3/SDL.h"
#include "fontmanager.hpp"
#include "textureregion.hpp"
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <string>
#include <vector>

struct MultiSizeTexture
{
    std::string Path;
    uint32_t Width;
    uint32_t Height;
};

struct LoadProcess
{
    std::string Path;
    SDL_Texture** Output;
};

class Texture
{
private:
    std::atomic<uint8_t> numberOfTexturesLoading{ 0 };
    std::vector<LoadProcess> textureLoadTasks;
    std::condition_variable observer;
    std::mutex mutualExclusion;
    FontManager fontManager;

private:
    void convertSurfaceToTexture(SDL_Surface* surface, SDL_Texture** outTexture);

public:
    void queueTextureLoadFromFile(const LoadProcess& process);

    void beginTextureLoadProcess();

    MultiSizeTexture findTextureThatFitsIntoArea(uint32_t width, uint32_t height, const std::string& nameOfTexture);

    void initFontManager();

    void deinitFontManager() const;

    TextureRegion createTextureFromText(const SDL_FRect& area, const std::string& text);
};