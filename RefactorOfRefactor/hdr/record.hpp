#pragma once
#include "invokable.hpp"
#include "SDL3/SDL.h"
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
};

struct TextBubble
{
    HexagonInstance Frontend;
    Invokable Backend;
};

struct Scoreboard
{
    SDL_FRect BaseArea;
    RectangleGpu BackgroundPart1;
    RectangleGpu BackgroundPart2;
    PrimitiveGpu BackgroundPart3;
    PrimitiveGpu BackgroundPart4;
    PrimitiveGpu BackgroundPart5;
    PrimitiveGpu BackgroundPart6;
    PrimitiveGpu BackgroundBorder;
};
