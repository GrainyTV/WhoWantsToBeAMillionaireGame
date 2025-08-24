#pragma once
#include "SDL3/SDL.h"
#include "glm/glm.hpp"
// #include "opengl.hpp"
#include "shader.hpp"
#include <array>

namespace EllipseModule
{
    //namespace _details
    //{
    //    struct EllipseCpu
    //    {
    //        std::array<glm::vec2, STEPS + 1> Vertices;
    //        SDL_FRect InnerArea;
    //    };

    //    struct EllipseGpu
    //    {
    //        SDL_FColor Color;
    //        OpenGL::TextureGpu Icon;
    //        OpenGL::PrimitiveGpu Ellipse;
    //        OpenGL::PrimitiveGpu Stroke;
    //    };
    //}

    constexpr size_t POLY_COUNT = 48;

    class EllipseButton
    {
    private:
        std::array<glm::vec2, POLY_COUNT + 1> points;
        SDL_FRect innerArea;
        Shader::VertexInfo ellipseFillBuffer;
        Shader::VertexInfo strokeBuffer;
        Shader::TextureInfo optionalRenderedText;

    public:
        EllipseButton(SDL_FRect, float);

        auto draw(SDL_GPUCommandBuffer* const, SDL_GPURenderPass* const) const -> void;

        auto whereToRenderTextureAt(const Shader::TextureInfo&) const -> SDL_FRect;

        auto applyText(std::string_view) -> void;

        auto renderedText() const -> const Shader::TextureInfo&;
    };

    //struct EllipseButton
    //{
    //    _impl_details::EllipseCpu CpuProperties;
    //    _impl_details::EllipseGpu GpuProperties;
    //};

    //EllipseButton init(SDL_FRect, float);
}
