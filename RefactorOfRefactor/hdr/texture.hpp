#pragma once
#include "textureregion.hpp"
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <string>
#include <vector>

enum WhereToLoadTextureFrom
{
    FromDisk,
    FromText,
};

struct MultiSizeTexture
{
    std::string Path;
    uint32_t Width;
    uint32_t Height;
};

struct LoadProcess
{
    WhereToLoadTextureFrom Source;
    std::string Asset;
    TextureRegion* Output;
    SDL_FRect HoldingArea;
};

class Texture
{
private:
    std::atomic<uint8_t> numberOfTexturesLoading{ 0 };
    std::vector<LoadProcess> textureLoadTasks;
    std::condition_variable observer;
    std::mutex mutualExclusion;

private:
    void convertSurfaceToTexture(SDL_Surface* surface, SDL_Texture** outTexture);

    SDL_FRect initializeAreaFromSurface(SDL_FRect area, int32_t width, int32_t height);

public:
    void queueTextureLoad(LoadProcess&& process);

    void beginTextureLoadProcess();

    const MultiSizeTexture findTextureThatFitsIntoArea(uint32_t width, uint32_t height, const std::string& nameOfTexture);
};