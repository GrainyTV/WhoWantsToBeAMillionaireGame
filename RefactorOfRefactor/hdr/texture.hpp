#pragma once
#include "SDL3/SDL.h"
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <queue>
#include <regex>
#include <string>
#include <thread>

struct MultiSizeTexture
{
    std::string Name;
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
    static inline uint32_t threadCount{ std::thread::hardware_concurrency() };
    std::atomic<uint8_t> numberOfTexturesLoading{ 0 };
    std::queue<LoadProcess> texturesToLoad;
    std::condition_variable observer;
    std::mutex mutuallyExclusiveFlag;

    void convertSurfaceToTexture(SDL_Surface* surface, SDL_Texture** outTexture);

    // void loadSurfaceFromFile(const std::string& fileName, SDL_Texture** outTexture);

public:
    void queueTextureLoadFromFile(const std::string& fileName, SDL_Texture** outTexture);

    void allTexturesLoaded();

    static bool regexGlobalMatches(const std::string& input, std::vector<std::smatch>& allMatches, const std::regex& regexToUse);

    static MultiSizeTexture findTextureThatFitsIntoArea(uint32_t width, uint32_t height, const std::string& nameOfTexture);
};