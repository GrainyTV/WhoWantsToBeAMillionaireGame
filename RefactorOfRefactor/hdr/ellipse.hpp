#pragma once
#include "SDL3/SDL.h"
#include "glm/glm.hpp"
#include "opengl.hpp"
#include <vector>

namespace Ellipse
{
    namespace _impl_details
    {
        struct EllipseDescriptor
        {
            glm::vec2 Center;
            float RadiusHorizontal;
            float RadiusVertical;
        };

        struct EllipseCpu
        {

        };

        struct EllipseGpu
        {
            OpenGL::PrimitiveGpu Ellipse;
        };
    }

    std::vector<glm::vec2> generate(const _impl_details::EllipseDescriptor&);

    struct EllipseButton
    {
        _impl_details::EllipseCpu CpuProperties;
        _impl_details::EllipseGpu GpuProperties;
    };

    EllipseButton init(SDL_FRect);
}
