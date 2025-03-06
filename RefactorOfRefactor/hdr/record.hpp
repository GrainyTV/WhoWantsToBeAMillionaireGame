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
    uint32_t BufferId = 0;
};

struct HexagonCpu
{
    std::array<Vec2, 6> Positions;
    SDL_FRect AvailableArea;
    SDL_FRect TextArea;
};

struct HexagonGpu
{
    TextureGpu RenderedText;
    PrimitiveGpu Background;
    PrimitiveGpu Border;
    RectangleGpu HorizontalLine;
    //Option::Inst<RectangleGpu> HorizontalLine = Option::None<RectangleGpu>();
    //bool TextVisible;
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
    HexagonRenderProperties GeneralProperties;
};

struct TextBubble
{
    HexagonInstance Frontend;
    Invokable Backend;
};
