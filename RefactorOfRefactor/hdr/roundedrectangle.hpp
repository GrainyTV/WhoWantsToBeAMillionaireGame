#pragma once
#include "SDL3/SDL.h"
//#include "opengl.hpp"
#include <array>
//#include "glm/glm.hpp"
#include "shader.hpp"

namespace RoundedRectangleModule
{
    class RoundedRectangle
    {
    private:
        float radius;
        std::array<glm::vec2, 4 * 17> points;
        SDL_FRect area;
        Shader::VertexInfo roundedRectangleFillBuffer;
        Shader::VertexInfo strokeBuffer;

    public:
        RoundedRectangle(SDL_FRect);

        auto draw(SDL_GPUCommandBuffer* const, SDL_GPURenderPass* const) const -> void;

        auto areaInsidePadding() const -> SDL_FRect;
    };
    
    
    //namespace _impl_details
    //{
    //    struct RoundedRectangleCpu
    //    {
    //        glm::vec2 TopLeft;
    //        float AvailableWidth;
    //        float AvailableHeight;
    //    };

    //    struct RoundedRectangleGpu
    //    {
    //        OpenGL::RectangleGpu BackgroundSquare1;
    //        OpenGL::RectangleGpu BackgroundSquare2;
    //        
    //        OpenGL::PrimitiveGpu BackgroundCorner1;
    //        OpenGL::PrimitiveGpu BackgroundCorner2;
    //        OpenGL::PrimitiveGpu BackgroundCorner3;
    //        OpenGL::PrimitiveGpu BackgroundCorner4;

    //        OpenGL::PrimitiveGpu Stroke;
    //    };
    //}

    //struct RoundedRectangleInstance
    //{
    //    _impl_details::RoundedRectangleCpu CpuProperties;
    //    _impl_details::RoundedRectangleGpu GpuProperties;
    //};

    //RoundedRectangleInstance init(SDL_FRect);
}

