#pragma once
#include "SDL3/SDL.h"
#include "glm/glm.hpp"
#undef assert
#include "optionwrap.hpp"
#include <initializer_list>

namespace OpenGL
{
    struct TextureGpu
    {
        uint32_t Id;
        uint32_t BufferId;
        int32_t Width;
        int32_t Height;
    };

    struct PrimitiveGpu
    {
        uint32_t BufferId;
        int32_t VertexCount;
    };

    using RectangleGpu = uint32_t;

    void compileShaders();

    void changeDrawColorTo(SDL_FColor);

    TextureGpu createTextureFromSurface(SDL_Surface*&);

    void renderTexture(TextureGpu);

    void renderQuad(uint32_t);

    void renderTriangles(uint32_t, int32_t);

    void renderTrianglesPacked(uint32_t, int32_t);

    void renderTrianglesHub(uint32_t, int32_t);

    uint32_t createPrimitives(std::span<const glm::vec2>);
    
    uint32_t createPrimitives(std::initializer_list<const glm::vec2>);

    uint32_t generateAndApplyVertexArray();
    
    void defineTextureRenderLocation(TextureGpu, const Option::Inst<SDL_FRect>& = Option::None<SDL_FRect>());

    void clearScreen();
}
