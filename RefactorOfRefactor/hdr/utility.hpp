#pragma once
#include "SDL3/SDL.h"
#include "svl/SVL.h"
#include "textureregion.hpp"
#include <regex>
#include <span>
#include "debug.hpp"

using StringMatch = std::smatch;

namespace Utility
{
    enum CustomEvents
    {
        EVENT_INVALIDATE = SDL_EVENT_USER,
        EVENT_ENABLE_SCENE,
        EVENT_INVOKE_ON_UI_THREAD,
        EVENT_CHANGE_SCENE,
    };
    // {
    //     EVENT_INVALIDATE = SDL_EVENT_USER,
    //     EVENT_ENABLE_SCENE,
    //     EVENT_INVOKE_ON_UI_THREAD,
    // };

    Vec2 fPointToSvl(const SDL_FPoint fPoint);
    // {
    //     return Vec2(fPoint.x, fPoint.y);
    // }

    SDL_FPoint svlToFpoint(const Vec2 svlVec2);
    // {
    //     return SDL_FPoint({
    //         .x = svlVec2[0],
    //         .y = svlVec2[1],
    //     });
    // }

    void changeDrawColorTo(SDL_Renderer* renderer, const SDL_Color& color);
    // {
    //     const auto success = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    //     ASSERT(success == 0, "Failed to change draw color ({})", SDL_GetError());
    // }

    const SDL_DisplayMode* displayInfo(SDL_Window* window);
    // {
    //     const uint32_t displayId = SDL_GetDisplayForWindow(window);
    //     ASSERT(displayId > 0, "Failed to retrieve monitor ID of the window ({})", SDL_GetError());

    //     const SDL_DisplayMode* displayInfo = SDL_GetCurrentDisplayMode(displayId);
    //     ASSERT(displayInfo != NULL, "Failed to retrieve info from monitor[{}] ({})", displayId, SDL_GetError());

    //     return displayInfo;
    // }

    bool regexSearch(const std::string& input, std::vector<StringMatch>& allMatches, const std::regex& regexToUse);
    // {
    //     const auto begin = std::sregex_iterator(input.begin(), input.end(), regexToUse);
    //     const auto end = std::sregex_iterator();

    //     fut::forEach(stdr::subrange{ begin, end }, [&](const auto& match, size_t /*i*/) {
    //         allMatches.push_back(match);
    //     });

    //     return allMatches.empty() ? false : true;
    // }

    void drawTextureRegion(SDL_Renderer* renderer, const TextureRegion& textureRegion);
    // {
    //     if (textureRegion.Resource != NULL)
    //     {
    //         const auto textureFiltering = SDL_SetTextureScaleMode(textureRegion.Resource, SDL_SCALEMODE_BEST);
    //         ASSERT(textureFiltering == 0, "Failed to set texture filtering mode ({})", SDL_GetError());

    //         const auto drawTexture = SDL_RenderTexture(renderer, textureRegion.Resource, NULL, textureRegion.getArea());
    //         ASSERT(drawTexture == 0, "Failed to draw texture onto the screen ({})", SDL_GetError());
    //     }
    // }

    void drawRectangle(SDL_Renderer* renderer, const SDL_FRect* rectangle);
    // {
    //     const auto drawRectangle = SDL_RenderFillRect(renderer, rectangle);
    //     ASSERT(drawRectangle == 0, "Failed to draw rectangle onto the screen ({})", SDL_GetError());
    // }

    void drawVertices(SDL_Renderer* renderer, std::span<const SDL_Vertex> vertices);
    // {
    //     const auto drawVertices = SDL_RenderGeometry(renderer, NULL, vertices.data(), vertices.size(), NULL, 0);
    //     ASSERT(drawVertices == 0, "Failed to draw vertices onto the screen ({})", SDL_GetError());
    // }

    float areaOfTriangleByItsVertices(const SDL_FPoint& a, const SDL_FPoint& b, const SDL_FPoint& c);
    // {
    //     return 0.5f * std::abs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
    // }

    void requestEvent(SDL_Event&& newEvent);
    // {
    //     SDL_PushEvent(&newEvent);
    // }

    void invalidate();
    // {
    //     requestEvent({ .type = EVENT_INVALIDATE });
    // }

    template<typename T>
    void free(T* managedObj)
    {
        ASSERT(managedObj != nullptr, "Trying to free a null pointer");
        delete managedObj;
    }
}