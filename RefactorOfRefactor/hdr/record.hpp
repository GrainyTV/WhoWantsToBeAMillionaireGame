#pragma once
#include "invokable.hpp"
#include "SDL3/SDL.h"
#include "optionwrap.hpp"
#include "svl/SVL.h"
#include <cstdint>

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
    uint32_t VertexCount;
};

struct RectangleGpu
{
    uint32_t BufferId;
};

struct HexagonCpu
{
    std::array<Vec2, 6> Positions;
    SDL_FRect TextArea;
};

struct HexagonGpu
{
    TextureGpu RenderedText;
    PrimitiveGpu Background;
    PrimitiveGpu Border;
    Option::Inst<RectangleGpu> HorizontalLine = Option::None<RectangleGpu>();
};

struct HexagonInstance
{
    HexagonCpu CpuProperties;
    HexagonGpu GpuProperties;
};

struct TextBubble
{
    HexagonInstance Frontend;
    Invokable Backend;
};
