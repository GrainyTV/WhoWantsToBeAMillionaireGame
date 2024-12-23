#include "utility.hpp"
#include "debug.hpp"
#include "functionals.hpp"

namespace stdr = std::ranges;
using enum Utility::CustomEvents;
using namespace Functionals;

Vec2 Utility::fPointToSvl(const SDL_FPoint fPoint)
{
    return Vec2(fPoint.x, fPoint.y);
}

SDL_FPoint Utility::svlToFpoint(const Vec2 svlVec2)
{
    return SDL_FPoint({
        .x = svlVec2[0],
        .y = svlVec2[1],
    });
}

void Utility::changeDrawColorTo(SDL_Renderer* renderer, const SDL_Color& color)
{
    const auto success = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    ASSERT(success == 0, "Failed to change draw color ({})", SDL_GetError());
}

const SDL_DisplayMode* Utility::displayInfo(SDL_Window* window)
{
    const uint32_t displayId = SDL_GetDisplayForWindow(window);
    ASSERT(displayId > 0, "Failed to retrieve monitor ID of the window ({})", SDL_GetError());

    const SDL_DisplayMode* displayInfo = SDL_GetCurrentDisplayMode(displayId);
    ASSERT(displayInfo != NULL, "Failed to retrieve info from monitor[{}] ({})", displayId, SDL_GetError());

    return displayInfo;
}

// bool Utility::regexSearch(const std::string& input, std::vector<StringMatch>& allMatches, const std::regex& regexToUse)
// {
//     const auto begin = std::sregex_iterator(input.begin(), input.end(), regexToUse);
//     const auto end = std::sregex_iterator();

//     forEach(stdr::subrange(begin, end), [&](const auto& match, size_t /*i*/) {
//         allMatches.push_back(match);
//     });

//     return allMatches.empty() ? false : true;
// }

void Utility::drawTextureRegion(SDL_Renderer* renderer, const TextureRegion& textureRegion)
{
    if (textureRegion.Resource != NULL)
    {
        // if (textureRegion.Area.isSome())
        // {
        //     const auto textureFiltering = SDL_SetTextureScaleMode(textureRegion.Resource, textureRegion.Area.value().x == 0 ? SDL_SCALEMODE_NEAREST : SDL_SCALEMODE_LINEAR);
        //     ASSERT(textureFiltering == 0, "Failed to set texture filtering mode ({})", SDL_GetError());
        // }

        const auto drawTexture = SDL_RenderTexture(
            renderer,
            textureRegion.Resource,
            nullptr,
            textureRegion.Area.isSome()
                ? &textureRegion.Area.value()
                : nullptr);
        ASSERT(drawTexture == 0, "Failed to draw texture onto the screen ({})", SDL_GetError());
    }
}

void Utility::drawRectangle(SDL_Renderer* renderer, const SDL_FRect* rectangle)
{
    const auto drawRectangle = SDL_RenderFillRect(renderer, rectangle);
    ASSERT(drawRectangle == 0, "Failed to draw rectangle onto the screen ({})", SDL_GetError());
}

void Utility::drawVertices(SDL_Renderer* renderer, std::span<const SDL_Vertex> vertices)
{
    const auto drawVertices = SDL_RenderGeometry(renderer, NULL, vertices.data(), vertices.size(), NULL, 0);
    ASSERT(drawVertices == 0, "Failed to draw vertices onto the screen ({})", SDL_GetError());
}

float Utility::areaOfTriangleByItsVertices(const SDL_FPoint& a, const SDL_FPoint& b, const SDL_FPoint& c)
{
    return 0.5f * std::abs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
}

void Utility::requestEvent(SDL_Event&& newEvent)
{
    SDL_PushEvent(&newEvent);
}

void Utility::requestUserEvent(SDL_UserEvent&& newEvent)
{
    requestEvent({ .user = newEvent });
}

void Utility::invalidate()
{
    requestEvent({ .type = EVENT_INVALIDATE });
}
