#include "texture.hpp"
#include "SDL3_image/SDL_image.h"
#include "defer.hpp"
#include "extensions.hpp"
#include "functionals.hpp"
#include "game.hpp"
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <format>
#include <thread>

void Texture::convertSurfaceToTexture(SDL_Surface* surface, SDL_Texture** outTexture)
{
    DEFER(SDL_DestroySurface, surface);

    *outTexture = SDL_CreateTextureFromSurface(Game::Renderer, surface);
    ASSERT(*outTexture != NULL, "Failed to convert surface to texture ({})", SDL_GetError());

    --numberOfTexturesLoading;
    observer.notify_one();
}

void Texture::queueTextureLoadFromFile(const LoadProcess& process)
{
    ++numberOfTexturesLoading;
    textureLoadTasks.emplace_back(process);
}

void Texture::beginTextureLoadProcess()
{
    std::thread detachedWorker{
        [&]() 
        {
            fut::forEach(textureLoadTasks, [&](const auto& task, size_t _)
            {
                SDL_Surface* surfaceFromPath = IMG_Load(task.Path.c_str());
                ASSERT(surfaceFromPath != NULL, "Failed to load surface ({})", task.Path);

                const auto callback = new Invokable(&Texture::convertSurfaceToTexture, this, surfaceFromPath, task.Output);
                Game::EventHandler.requestEvent({ .user = { .type = EVENT_INVOKE_ON_UI_THREAD, .data1 = static_cast<void*>(callback), } });
            });

            {
                std::unique_lock<std::mutex> lock{ mutualExclusion };
                observer.wait(lock, [&]() { return numberOfTexturesLoading == 0; });
                textureLoadTasks.clear();
            }

            Game::EventHandler.invalidate();
        }
    };
    detachedWorker.detach();
}

MultiSizeTexture Texture::findTextureThatFitsIntoArea(uint32_t width, uint32_t height, const std::string& nameOfTexture)
{
    std::vector<MultiSizeTexture> resolutionsOfTexture;
    const auto dir = std::filesystem::directory_iterator(std::format("assets/textures/{}", nameOfTexture));

    fut::forEach(dir, [&](const auto& entry, const size_t _) {
        const auto name = entry.path().stem().string();
        std::vector<StringMatch> matches;
        std::regex numericValues(R"(\d+)");

        if (ext::regexGlobalMatches(name, matches, numericValues))
        {
            ASSERT(matches.size() == 2, "Resolution should have two components in its filename (Found {} in {})", matches.size(), name);
            const uint32_t width = std::stoi(matches[0].str());
            const uint32_t height = std::stoi(matches[1].str());
            resolutionsOfTexture.push_back(MultiSizeTexture{ .Path = entry.path().string(), .Width = width, .Height = height });
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