#pragma once
#include "Eigen/Dense"
#include "SDL3/SDL.h"
#include "assert.hpp"
#include "functionals.hpp"
#include "scenes.hpp"
#include <regex>
#include <span>

using StringMatch = std::smatch;

// ============================= //
// Extension Functions Namespace //
// ============================= //

namespace ext
{
    inline Eigen::Vector2f fPointToEigen(const SDL_FPoint& fPoint)
    {
        return Eigen::Vector2f{ fPoint.x, fPoint.y };
    }

    inline SDL_FPoint eigenToFpoint(const Eigen::Vector2f& eigenVector)
    {
        return SDL_FPoint{ eigenVector.x(), eigenVector.y() };
    }

    inline void changeDrawColorTo(SDL_Renderer* renderer, const SDL_Color& color)
    {
        const auto success = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        ASSERT(success == 0, "Failed to change draw color ({})", SDL_GetError());
    }

    inline const SDL_DisplayMode* displayInfo(SDL_Window* window)
    {
        const uint32_t displayId = SDL_GetDisplayForWindow(window);
        ASSERT(displayId > 0, "Failed to retrieve monitor ID of the window ({})", SDL_GetError());

        const SDL_DisplayMode* displayInfo = SDL_GetCurrentDisplayMode(displayId);
        ASSERT(displayInfo != NULL, "Failed to retrieve info from monitor[{}] ({})", displayId, SDL_GetError());

        return displayInfo;
    }

    inline bool regexGlobalMatches(const std::string& input, std::vector<StringMatch>& allMatches, const std::regex& regexToUse)
    {
        const auto begin = std::sregex_iterator(input.begin(), input.end(), regexToUse);
        const auto end = std::sregex_iterator();

        fut::forEach(std::ranges::subrange{ begin, end }, [&](const auto& match, size_t _) {
            allMatches.push_back(match);
        });

        return allMatches.empty() ? false : true;
    }

    inline void drawTextureRegion(SDL_Renderer* renderer, const TextureRegion& textureRegion)
    {
        if (textureRegion.Resource != NULL)
        {
            const auto textureFiltering = SDL_SetTextureScaleMode(textureRegion.Resource, SDL_SCALEMODE_BEST);
            ASSERT(textureFiltering == 0, "Failed to set texture filtering mode ({})", SDL_GetError());

            const auto drawTexture = SDL_RenderTexture(renderer, textureRegion.Resource, NULL, textureRegion.getArea());
            ASSERT(drawTexture == 0, "Failed to draw texture onto the screen ({})", SDL_GetError());
        }
    }

    inline void drawRectangle(SDL_Renderer* renderer, const SDL_FRect* rectangle)
    {
        const auto drawRectangle = SDL_RenderFillRect(renderer, rectangle);
        ASSERT(drawRectangle == 0, "Failed to draw rectangle onto the screen ({})", SDL_GetError());
    }

    inline void drawVertices(SDL_Renderer* renderer, std::span<const SDL_Vertex> vertices)
    {
        const auto drawVertices = SDL_RenderGeometry(renderer, NULL, vertices.data(), vertices.size(), NULL, 0);
        ASSERT(drawVertices == 0, "Failed to draw vertices onto the screen ({})", SDL_GetError());
    }
}