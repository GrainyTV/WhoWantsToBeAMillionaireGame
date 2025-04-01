#pragma once
#include "SDL3/SDL.h"
#include "opengl.hpp"
#include <array>
#include "glm/glm.hpp"

namespace RoundedRectangle
{
    namespace _impl_details
    {
        struct RoundedRectangleCpu
        {
            glm::vec2 TopLeft;
            float AvailableWidth;
            float AvailableHeight;
        };

        struct RoundedRectangleGpu
        {
            OpenGL::RectangleGpu BackgroundSquare1;
            OpenGL::RectangleGpu BackgroundSquare2;
            
            OpenGL::PrimitiveGpu BackgroundCorner1;
            OpenGL::PrimitiveGpu BackgroundCorner2;
            OpenGL::PrimitiveGpu BackgroundCorner3;
            OpenGL::PrimitiveGpu BackgroundCorner4;

            OpenGL::PrimitiveGpu Stroke;
        };
    }

    struct RoundedRectangleInstance
    {
        _impl_details::RoundedRectangleCpu CpuProperties;
        _impl_details::RoundedRectangleGpu GpuProperties;
    };

    RoundedRectangleInstance init(SDL_FRect);
}

