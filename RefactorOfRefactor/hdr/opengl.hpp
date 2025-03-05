#pragma once
#include "SDL3/SDL.h"
#include "optionwrap.hpp"
#include "record.hpp"
#include "svl/SVL.h"

namespace OpenGL
{
    void compileShaders();

    void changeDrawColorTo(SDL_FColor);

    TextureGpu createTextureFromSurface(SDL_Surface*&);

    void renderTexture(TextureGpu);

    void renderPrimitives(uint32_t, int32_t, int32_t);

    void renderQuad(uint32_t);

    uint32_t createPrimitives(std::span<const Vec2>);

    uint32_t generateAndApplyVertexArray();
    
    void defineTextureRenderLocation(TextureGpu, const Option::Inst<SDL_FRect>& = Option::None<SDL_FRect>());

    void clearScreen();
}
