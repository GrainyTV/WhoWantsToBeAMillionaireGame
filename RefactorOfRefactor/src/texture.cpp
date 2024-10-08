// #include "texture.hpp"
// #include "defer.hpp"
// #include "extensions.hpp"
// //#include "fontmanager.hpp"
// #include "functionals.hpp"
// //#include "game.hpp"
// #include <algorithm>
// #include <cstdint>
// #include <filesystem>
// #include <format>
// #include <thread>
// #include "globals.hpp"
// #include <regex>
// #include "SDL3_image/SDL_image.h"
// #include "SDL3_ttf/SDL_ttf.h"

// import FontManager;

// using enum utility::CustomEvents;
// //using StringMatch = std::smatch;

// void texture::convertSurfaceToTexture(SDL_Surface* surface, SDL_Texture** outTexture)
// {
//     DEFER(SDL_DestroySurface, surface);

//     *outTexture = SDL_CreateTextureFromSurface(globals::Properties.value().Renderer, surface);
//     //ASSERT(*outTexture != nullptr, "Failed to convert surface to texture ({})", SDL_GetError());

//     --numberOfTexturesLoading;
//     observer.notify_one();
// }

// void texture::queueTextureLoad(LoadProcess&& process)
// {
//     ++numberOfTexturesLoading;
//     textureLoadTasks.emplace_back(process);
// }

// void texture::beginTextureLoadProcess()
// {
//     std::thread detachedWorker{
//         [&]() 
//         {
//             fut::forEach(textureLoadTasks, [&](const auto& task, const size_t /*i*/)
//             {
//                 SDL_Surface* surface = NULL;
                
//                 switch (task.Source)
//                 {
//                     case FromDisk:
//                     {
//                         surface = IMG_Load(task.Asset.c_str());
//                         ASSERT(surface != NULL, "Failed to load surface {} ({})", task.Asset, IMG_GetError());
//                         break;
//                     }

//                     case FromText:
//                     {
//                         surface = fontmanager::generateNew(task.Asset);
//                         ASSERT(surface != NULL, "Failed to create surface from text ({})", TTF_GetError());
//                         (*task.Output).Area = initializeAreaFromSurface(task.HoldingArea, (*surface).w, (*surface).h);
//                         break;
//                     }
//                 }

//                 Invokable* const callback = new Invokable(&texture::convertSurfaceToTexture, surface, &(*task.Output).Resource);
//                 //Game::EventHandler.requestEvent({ .user = { .type = EVENT_INVOKE_ON_UI_THREAD, .data1 = static_cast<void*>(callback), } });
//                 utility::requestEvent({ .user = { .type = EVENT_INVOKE_ON_UI_THREAD, .data1 = static_cast<void*>(callback), } });
//             });

//             {
//                 std::unique_lock<std::mutex> lock{ mutualExclusion };
//                 observer.wait(lock, [&]() { return numberOfTexturesLoading == 0; });
//                 textureLoadTasks.clear();
//             }

//             //Game::EventHandler.requestEvent({ .type = EVENT_ENABLE_SCENE });
//             //Game::EventHandler.invalidate();

//             utility::requestEvent({ .type = EVENT_ENABLE_SCENE });
//             utility::invalidate();
//         }
//     };
    
//     detachedWorker.detach();
// }

// const MultiSizeTexture texture::findTextureThatFitsIntoArea(uint32_t width, uint32_t height, const std::string& nameOfTexture)
// {
//     std::vector<MultiSizeTexture> resolutionsOfTexture;
//     const auto dir = std::filesystem::directory_iterator(std::format("assets/textures/{}", nameOfTexture));

//     fut::forEach(dir, [&](const auto& entry, size_t /*i*/)
//     {
//         const auto name = entry.path().stem().string();
//         std::vector<StringMatch> matches;
//         std::regex numericValues(R"(\d+)");

//         if (utility::regexGlobalMatches(name, matches, numericValues))
//         {
//             ASSERT(matches.size() == 2, "Resolution should have two components in its filename (Found {} in {})", matches.size(), name);
//             const uint32_t width = std::stoi(matches[0].str());
//             const uint32_t height = std::stoi(matches[1].str());
//             resolutionsOfTexture.push_back(MultiSizeTexture{ .Path = entry.path().string(), .Width = width, .Height = height });
//         }
//     });

//     std::sort(resolutionsOfTexture.begin(), resolutionsOfTexture.end(), [](const auto& a, const auto& b)
//     {
//         return a.Width > b.Width;
//     });

//     std::vector<MultiSizeTexture> acceptableTextures = fut::filter(resolutionsOfTexture, [&](const auto& entry)
//     {
//         return entry.Width <= width && entry.Height <= height;
//     });

//     ASSERT(acceptableTextures.empty() == false, "No acceptable sized texture found ({} -> {}x{})", nameOfTexture, width, height);
//     return acceptableTextures.front();
// }

// SDL_FRect texture::initializeAreaFromSurface(SDL_FRect area, int32_t width, int32_t height)
// {
//     return SDL_FRect{ 
//         .x = area.x + (area.w - width) / 2.0f,
//         .y = area.y + (area.h - height) / 2.0f,
//         .w = static_cast<float>(width),
//         .h = static_cast<float>(height),
//     };
// }

#include "texture.hpp"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "debug.hpp"
#include "defer.hpp"
#include "fontmanager.hpp"
#include "globals.hpp"
#include "utility.hpp"
#include <atomic>
#include <condition_variable>
#include <filesystem>
#include "functionals.hpp"

using enum Utility::CustomEvents;
using namespace Functionals;

static std::atomic<uint8_t> numberOfTexturesLoading(0);
static std::vector<LoadProcess> textureLoadTasks;
static std::condition_variable observer;
static std::mutex mutualExclusion;

void texture::convertSurfaceToTexture(SDL_Surface* surface, SDL_Texture** outTexture)
{
    DEFER(SDL_DestroySurface, surface);

    *outTexture = SDL_CreateTextureFromSurface(Globals::Properties.value().Renderer, surface);
    ASSERT(*outTexture != nullptr, "Failed to convert surface to texture ({})", SDL_GetError());

    --numberOfTexturesLoading;
    observer.notify_one();
}

void texture::queueTextureLoad(LoadProcess&& process)
{
    ++numberOfTexturesLoading;
    textureLoadTasks.emplace_back(process);
}

void texture::beginTextureLoadProcess()
{
    std::thread detachedWorker{
        [&]() 
        {
            forEach(textureLoadTasks, [&](const auto& task, const size_t /*i*/)
            {
                SDL_Surface* surface = NULL;
                
                switch (task.Source)
                {
                    case FromDisk:
                    {
                        surface = IMG_Load(task.Asset.c_str());
                        ASSERT(surface != NULL, "Failed to load surface {} ({})", task.Asset, IMG_GetError());
                        break;
                    }

                    case FromText:
                    {
                        surface = FontManager::generateNew(task.Asset);
                        ASSERT(surface != NULL, "Failed to create surface from text ({})", TTF_GetError());
                        (*task.Output).Area = Option<SDL_FRect>::Some(initializeAreaFromSurface(task.HoldingArea, (*surface).w, (*surface).h));
                        break;
                    }
                }

                Invokable* const callback = new Invokable(texture::convertSurfaceToTexture, surface, &(*task.Output).Resource);
                Utility::requestEvent({ .user = { .type = EVENT_INVOKE_ON_UI_THREAD, .data1 = static_cast<void*>(callback), } });
            });

            {
                std::unique_lock<std::mutex> lock{ mutualExclusion };
                observer.wait(lock, [&]() { return numberOfTexturesLoading == 0; });
                textureLoadTasks.clear();
            }

            Utility::requestEvent({ .type = EVENT_ENABLE_SCENE });
            Utility::invalidate();
            LOG("All textures loaded!");
        }
    };
    
    detachedWorker.detach();
}

const MultiSizeTexture texture::findTextureThatFitsIntoArea(uint32_t width, uint32_t height, const std::string& nameOfTexture)
{
    std::vector<MultiSizeTexture> resolutionsOfTexture;
    const auto dir = std::filesystem::directory_iterator(std::format("assets/textures/{}", nameOfTexture));

    forEach(dir, [&](const auto& entry, size_t /*i*/)
    {
        const auto name = entry.path().stem().string();
        std::vector<StringMatch> matches;
        std::regex numericValues(R"(\d+)");

        if (Utility::regexSearch(name, matches, numericValues))
        {
            ASSERT(matches.size() == 2, "Resolution should have two components in its filename (Found {} in {})", matches.size(), name);
            const uint32_t width = std::stoi(matches[0].str());
            const uint32_t height = std::stoi(matches[1].str());
            resolutionsOfTexture.push_back(MultiSizeTexture{ .Path = entry.path().string(), .Width = width, .Height = height });
        }
    });

    std::sort(resolutionsOfTexture.begin(), resolutionsOfTexture.end(), [](const auto& a, const auto& b)
    {
        return a.Width > b.Width;
    });

    std::vector<MultiSizeTexture> acceptableTextures = filter(resolutionsOfTexture, [&](const auto& entry)
    {
        return entry.Width <= width && entry.Height <= height;
    });

    ASSERT(acceptableTextures.empty() == false, "No acceptable sized texture found ({} -> {}x{})", nameOfTexture, width, height);
    return acceptableTextures.front();
}

SDL_FRect texture::initializeAreaFromSurface(SDL_FRect area, int32_t width, int32_t height)
{
    return SDL_FRect{ 
        .x = area.x + (area.w - width) / 2.0f,
        .y = area.y + (area.h - height) / 2.0f,
        .w = static_cast<float>(width),
        .h = static_cast<float>(height),
    };
}
