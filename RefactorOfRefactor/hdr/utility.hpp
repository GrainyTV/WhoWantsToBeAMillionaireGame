#pragma once
#include "SDL3/SDL.h"
#include "color.hpp"
#include "debug.hpp"
#include "opengl.hpp"
#include "svl/SVL.h"
#include "textureregion.hpp"
#include <memory>
#include <span>

namespace Utility
{
    enum CustomEvents
    {
        EVENT_INVALIDATE = SDL_EVENT_USER,
        EVENT_ENABLE_SCENE,
        EVENT_INVOKE_ON_UI_THREAD,
        EVENT_CHANGE_SCENE_MAINMENU,
        EVENT_CHANGE_SCENE_INGAME,
    };

    //SDL_FPoint svlToFpoint(const Vec2 svlVec2);

    //void changeDrawColorTo(SDL_Renderer* renderer, const SDL_Color& color);

    const SDL_DisplayMode* displayInfo(SDL_Window* window);

    //void drawTextureRegion(SDL_Renderer*, TextureRegion&&);

    void drawRectangle(SDL_Renderer* renderer, const SDL_FRect* rectangle);

    void drawVertices(SDL_Renderer* renderer, std::span<const SDL_Vertex> vertices);

    float areaOfTriangleByItsVertices(SDL_FPoint, SDL_FPoint, SDL_FPoint);

    void requestEvent(SDL_Event&& newEvent);

    void requestUserEvent(SDL_UserEvent&& newEvent);

    void invalidate();

    template<typename T>
    void free(T* managedObj)
    {
        assert(managedObj != nullptr, "Trying to free a null pointer");
        delete managedObj;
    }

    // REVISED API
    // |
    // |
    // V

    bool isPositive(float);

    Vec2 fPointToSvl(SDL_FPoint);

    std::unique_ptr<char[]> formatPath(const char*, const char*);

    // template<SDL_Color Color>
    // void changeDrawColorTo()
    // {
    //     OpenGL::changeDrawColorTo(Color::normalize(Color));
    // }

    std::array<Vec2, 4> cornersOfRectangle(SDL_FRect);
}
