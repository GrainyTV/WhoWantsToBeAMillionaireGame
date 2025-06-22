#pragma once
#include "SDL3/SDL.h"
#include "glm/glm.hpp"
#include "invokable.hpp"
#include "opengl.hpp"
#include <span>
#include <string_view>
#include <array>

namespace Hexagon
{
    namespace _impl_details
    {
        struct HexagonCpu
        {
            std::array<glm::vec2, 68> Positions;
            SDL_FRect AvailableArea;
            SDL_FRect TextArea;
        };

        struct HexagonGpu
        {
            OpenGL::TextureGpu RenderedText;
            OpenGL::PrimitiveGpu Background;
            OpenGL::PrimitiveGpu Border;
            OpenGL::RectangleGpu HorizontalLine;
        };

        struct HexagonRenderProperties
        {
            bool TextVisible;
            SDL_FColor ButtonColor;
        };

        struct HexagonInstance
        {
            HexagonCpu CpuProperties;
            HexagonGpu GpuProperties;
            HexagonRenderProperties RenderProperties;
        };
    }

    struct TextBubble
    {
        bool IsEnabled = true;
        _impl_details::HexagonInstance Frontend;
        Invokable Backend;
    };

    _impl_details::HexagonInstance init(SDL_FRect, bool, std::string_view = "");

    void lateinit(_impl_details::HexagonInstance&, std::string_view);

    void draw(const _impl_details::HexagonGpu&, const _impl_details::HexagonRenderProperties&);

    bool isCursorInside(std::span<const glm::vec2>, glm::vec2);
}
