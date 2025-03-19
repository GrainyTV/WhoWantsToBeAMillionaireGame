#pragma once
#include "opengl.hpp"
#include <array>

namespace ScoreboardModule
{
    namespace _impl_details
    {
        struct ScoreboardCpu
        {
            SDL_FRect InnerArea;
        };

        struct ScoreboardGpu
        {
            OpenGL::RectangleGpu BackgroundPart1;
            OpenGL::RectangleGpu BackgroundPart2;
            OpenGL::PrimitiveGpu BackgroundCorner1;
            OpenGL::PrimitiveGpu BackgroundCorner2;
            OpenGL::PrimitiveGpu BackgroundCorner3;
            OpenGL::PrimitiveGpu BackgroundCorner4;
            OpenGL::PrimitiveGpu BackgroundBorder;

            OpenGL::PrimitiveGpu FiftyFiftyHelpEllipse;
            OpenGL::PrimitiveGpu PhoneHelpEllipse;
            OpenGL::PrimitiveGpu AudienceHelpEllipse;
        };
    }
    
    struct Scoreboard
    {
        _impl_details::ScoreboardCpu CpuProperties;
        _impl_details::ScoreboardGpu GpuProperties;
    };

    Scoreboard init(std::span<const glm::vec2>, float);

    void draw(const _impl_details::ScoreboardGpu&);
}

