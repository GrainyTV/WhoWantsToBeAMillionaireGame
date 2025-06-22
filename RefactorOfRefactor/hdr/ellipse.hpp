#pragma once
#include "SDL3/SDL.h"
#include "glm/glm.hpp"
#include "opengl.hpp"
#include <array>

namespace Ellipse
{
    namespace _impl_details
    {
        constexpr std::size_t STEPS = 48;

        struct EllipseDescriptor
        {
            glm::vec2 Center;
            float RadiusHorizontal;
            float RadiusVertical;
        };

        struct EllipseCpu
        {
            std::array<glm::vec2, STEPS + 1> Vertices;
            SDL_FRect InnerArea;
        };

        struct EllipseGpu
        {
            SDL_FColor Color;
            OpenGL::TextureGpu Icon;
            OpenGL::PrimitiveGpu Ellipse;
            OpenGL::PrimitiveGpu Stroke;
        };
    }

    struct EllipseButton
    {
        _impl_details::EllipseCpu CpuProperties;
        _impl_details::EllipseGpu GpuProperties;
    };

    EllipseButton init(SDL_FRect, float);
}
