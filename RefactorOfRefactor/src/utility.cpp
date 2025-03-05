#include "utility.hpp"
#include "debug.hpp"
#include "result.hpp"

using enum Utility::CustomEvents;

// SDL_FPoint Utility::svlToFpoint(const Vec2 svlVec2)
// {
//     return { svlVec2[0], svlVec2[1] };
// }

// void Utility::changeDrawColorTo(SDL_Renderer* renderer, const SDL_Color& color)
// {
//     const auto colorChange = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
//     assert(colorChange, "Failed to change draw color ({})", SDL_GetError());
// }

const SDL_DisplayMode* Utility::displayInfo(SDL_Window* window)
{
    const uint32_t displayId = SDL_GetDisplayForWindow(window);
    assert(displayId > 0, "Failed to retrieve monitor ID of the window ({})", SDL_GetError());

    const SDL_DisplayMode* displayInfo = SDL_GetCurrentDisplayMode(displayId);
    assert(displayInfo != nullptr, "Failed to retrieve info from monitor[{}] ({})", displayId, SDL_GetError());

    return displayInfo;
}

// void Utility::drawTextureRegion(SDL_Renderer* renderer, TextureRegion&& textureRegion)
// {
//     if (textureRegion.Resource != nullptr)
//     {
//         const bool drawnTexture = SDL_RenderTexture(renderer, textureRegion.Resource, nullptr,
//             textureRegion.Area.isSome()
//             ? &textureRegion.Area.value()
//             : nullptr);
//         assert(drawnTexture, "Failed to draw texture onto the screen", SDL_GetError());
//     }
// }

void Utility::drawRectangle(SDL_Renderer* renderer, const SDL_FRect* rectangle)
{
    const auto drawRectangle = SDL_RenderFillRect(renderer, rectangle);
    assert(drawRectangle == 0, "Failed to draw rectangle onto the screen ({})", SDL_GetError());
}

void Utility::drawVertices(SDL_Renderer* renderer, std::span<const SDL_Vertex> vertices)
{
    const auto drawVertices = SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(), nullptr, 0);
    assert(drawVertices, "Failed to draw vertices onto the screen ({})", SDL_GetError());
}

float Utility::areaOfTriangleByItsVertices(const SDL_FPoint vertA, const SDL_FPoint vertB, const SDL_FPoint vertC)
{
    return 0.5f * std::abs(vertA.x * (vertB.y - vertC.y) + vertB.x * (vertC.y - vertA.y) + vertC.x * (vertA.y - vertB.y));
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

namespace Utility
{
    bool isPositive(const float value)
    {
        return value >= 0.0f;
    }

    Vec2 fPointToSvl(const SDL_FPoint fPoint)
    {
        return { fPoint.x, fPoint.y };
    }

    std::unique_ptr<char[]> formatPath(const char* format, const char* name)
    {
        int32_t byteCount = SDL_snprintf(nullptr, 0, format, name);
        assert(byteCount > 0, "Failed to determine required bytes for stack based string format");
        
        // ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
        // ┃ Add an extra byte for null delimiter ┃
        // ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

        byteCount += 1;
        auto buffer = std::make_unique<char[]>(byteCount);
        SDL_snprintf(buffer.get(), byteCount, format, name);

        return buffer;
    }

    std::array<Vec2, 4> cornersOfRectangle(const SDL_FRect rect)
    {
        return {
            Vec2(rect.x, rect.y),
            Vec2(rect.x +rect.w, rect.y),
            Vec2(rect.x, rect.y + rect.h),
            Vec2(rect.x + rect.w, rect.y + rect.h)
        };
    }
}
