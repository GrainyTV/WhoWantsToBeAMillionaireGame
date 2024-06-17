#include "texture.hpp"
#include "SDL3_image/SDL_image.h"
#include "defer.hpp"
#include "functionals.hpp"
#include "game.hpp"
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <format>
#include <numeric>

bool Texture::regexGlobalMatches(const std::string& input, std::vector<std::smatch>& allMatches, const std::regex& regexToUse)
{
    auto begin = std::sregex_iterator(input.begin(), input.end(), regexToUse);
    auto end = std::sregex_iterator();
    const auto foundEntryAmount = std::distance(begin, end);

    if (foundEntryAmount == 0)
    {
        return false;
    }
    else
    {
        for (std::sregex_iterator i = begin; i != end; ++i)
        {
            allMatches.push_back(*i);
        }

        return true;
    }
}

MultiSizeTexture Texture::findTextureThatFitsIntoArea(uint32_t width, uint32_t height, const std::string& nameOfTexture)
{
    std::vector<MultiSizeTexture> resolutionsOfTexture;

    fut::forEach(std::filesystem::directory_iterator("textures"), [&](const auto& entry, const size_t _) {
        const auto entryName = entry.path().stem().string();

        if (entry.is_regular_file() && entryName.starts_with(nameOfTexture))
        {
            std::regex numericValues(R"(\d+)");
            std::vector<std::smatch> matches;

            if (regexGlobalMatches(entryName, matches, numericValues))
            {
                ASSERT(matches.size() == 2, "Resolution should have two components in its filename (Found {} in {})", matches.size(), entryName);
                const uint32_t width = std::stoi(matches[0].str());
                const uint32_t height = std::stoi(matches[1].str());
                resolutionsOfTexture.push_back(MultiSizeTexture{ .Name = entryName, .Width = width, .Height = height });
            }
        }
    });

    std::sort(resolutionsOfTexture.begin(), resolutionsOfTexture.end(), [](const auto& a, const auto& b) {
        return a.Width > b.Width;
    });

    const auto acceptableTextures = fut::filter(resolutionsOfTexture, [&](const auto& entry) {
        return entry.Width < width && entry.Height < height;
    });

    ASSERT(acceptableTextures.empty() == false, "No acceptable sized texture found ({} -> {}x{})", nameOfTexture, width, height);
    return acceptableTextures[0];
}

void Texture::convertSurfaceToTexture(SDL_Surface* surface, SDL_Texture** outTexture)
{
    *outTexture = SDL_CreateTextureFromSurface(Game::Renderer, surface);
    ASSERT(*outTexture != NULL, "Failed to convert surface to texture ({})", SDL_GetError());
    DEFER(SDL_DestroySurface, surface);

    texturesToLoad.pop();
    --numberOfTexturesLoading;
    // observer.notify_all();
}

/*void Texture::loadSurfaceFromFile(const char* fileName, SDL_Texture** outTexture)
{
    const auto path = std::format("textures/{}.png", fileName);

    SDL_Surface* surface = IMG_Load(path);
    ASSERT(surface != NULL, "Failed to load texture {}", path);

    Game::EventHandler.invoke(&Texture::convertSurfaceToTexture, this, surface, outTexture);
}*/

void Texture::queueTextureLoadFromFile(const std::string& fileName, SDL_Texture** outTexture)
{
    ++numberOfTexturesLoading;
    const auto path = std::format("textures/{}.png", fileName);
    texturesToLoad.emplace(LoadProcess{ .Path = path, .Output = outTexture });

    /*std::thread workerThread([&]() {
        const auto path = std::format("textures/{}.png", fileName);

        SDL_Surface* surface = IMG_Load(path.c_str());
        ASSERT(surface != NULL, "Failed to load surface ({})", path);

        Game::EventHandler.invoke(&Texture::convertSurfaceToTexture, this, surface, outTexture);
    });
    workerThread.detach();*/
}

void Texture::allTexturesLoaded()
{
    /*std::thread workerThread([&]() {
        std::unique_lock<std::mutex> lock(cvMutex);
        cv.wait(lock, [&]() { return numberOfTexturesLoading.load() == 0; });
        Game::EventHandler.invalidate();
    });
    workerThread.detach();*/

    /*std::thread workerThread([&]() {
        // std::unique_lock<std::mutex> lock(mutuallyExclusiveFlag);
        // cv.wait(lock, [&]() { return numberOfTexturesLoading.load() == 0; });

        while (texturesToLoad.empty() == false)
        {
            const auto task = texturesToLoad.front();

            SDL_Surface* surface = IMG_Load(task.Path);
            ASSERT(surface != NULL, "Failed to load surface ({})", task.Path);

            Game::EventHandler->invoke(&Texture::convertSurfaceToTexture, this, surface, task.Output);
        }

        std::unique_lock<std::mutex> lock(mutuallyExclusiveFlag);
        observer.wait(lock, [&]() { return numberOfTexturesLoading.load() == 0; });
        Game::EventHandler->invalidate();
    });
    workerThread.detach();*/

    while (texturesToLoad.empty() == false)
    {
        const auto texture = texturesToLoad.front();

        SDL_Surface* surface = IMG_Load(texture.Path.c_str());
        ASSERT(surface != NULL, "Failed to load surface ({})", texture.Path);

        (*this).convertSurfaceToTexture(surface, texture.Output);
        // Game::EventHandler->invoke(&Texture::convertSurfaceToTexture, this, surface, texture.Output);
    }

    // Game::EventHandler->invalidate();
}