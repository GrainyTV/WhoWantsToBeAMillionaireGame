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

    Vec2 fPointToSvl(const SDL_FPoint fPoint);

    SDL_FPoint svlToFpoint(const Vec2 svlVec2);

    void changeDrawColorTo(SDL_Renderer* renderer, const SDL_Color& color);

    const SDL_DisplayMode* displayInfo(SDL_Window* window);

    bool regexSearch(const std::string& input, std::vector<StringMatch>& allMatches, const std::regex& regexToUse);

    void drawTextureRegion(SDL_Renderer* renderer, const TextureRegion& textureRegion);

    void drawRectangle(SDL_Renderer* renderer, const SDL_FRect* rectangle);

    void drawVertices(SDL_Renderer* renderer, std::span<const SDL_Vertex> vertices);

    float areaOfTriangleByItsVertices(const SDL_FPoint& a, const SDL_FPoint& b, const SDL_FPoint& c);

    void requestEvent(SDL_Event&& newEvent);

    void requestUserEvent(SDL_UserEvent&& newEvent);

    void invalidate();

    template<typename T>
    void free(T* managedObj)
    {
        ASSERT(managedObj != nullptr, "Trying to free a null pointer");
        delete managedObj;
    }

    template<typename NewScene>
    void changeSceneTo()
    {
        auto newScene = new NewScene();
        void* newSceneAsVoid = static_cast<void*>(newScene);
        requestUserEvent({ .type = EVENT_CHANGE_SCENE, .data1 = newSceneAsVoid });
    }
}