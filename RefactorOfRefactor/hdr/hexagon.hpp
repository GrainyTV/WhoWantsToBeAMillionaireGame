#pragma once
#include "SDL3/SDL.h"
#include "SDL3/SDL_rect.h"
#include "color.hpp"
#include "debug.hpp"
#include "functionals.hpp"
#include "globals.hpp"
#include "optionwrap.hpp"
#include "svl/SVL.h"
#include "utility.hpp"
#include <array>
#include <cstdint>
#include <numeric>
#include <string_view>

class Hexagon
{
private:
    std::string_view text;
    SDL_FRect middlePart;
    std::array<SDL_Vertex, 6> sideParts;
    std::array<SDL_Vertex, 48> stroke;
    Option::Inst<SDL_FRect> horizontalLine;
    static constexpr uint32_t THICKNESS = 5;

    std::array<SDL_Vertex, 6> calculateSidesFromMiddlePart()
    {
        // ┏━━━━━━━━━━━━━┓
        // ┃   1-----2   ┃
        // ┃  /|     |\  ┃
        // ┃ 0 |     | 3 ┃
        // ┃  \|     |/  ┃
        // ┃   5-----4   ┃
        // ┗━━━━━━━━━━━━━┛

        const SDL_FPoint pos1 = { .x = middlePart.x, .y = middlePart.y };
        const SDL_FPoint pos2 = { .x = middlePart.x + middlePart.w, .y = middlePart.y };
        const SDL_FPoint pos4 = { .x = middlePart.x + middlePart.w, .y = middlePart.y + middlePart.h };
        const SDL_FPoint pos5 = { .x = middlePart.x, .y = middlePart.y + middlePart.h };

        const float verticalDistance = pos5.y - pos1.y;
        const float midPoint = std::midpoint(pos5.y, pos1.y);
        SDL_FPoint pos0 = { .y = midPoint };
        SDL_FPoint pos3 = { .y = midPoint };

        // ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
        // ┃ Solving Pythagorean Theorem for (a) side ┃
        // ┃                                          ┃
        // ┃      /|                                  ┃
        // ┃     / |                                  ┃
        // ┃  c /  | b                                ┃
        // ┃   /   |                                  ┃
        // ┃  /____|                                  ┃
        // ┃    a                                     ┃
        // ┃                                          ┃
        // ┃ c = 2/3 * verticalDistance               ┃
        // ┃ b = 1/2 * verticalDistance               ┃
        // ┃ a = ?                                    ┃
        // ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

        const float c2 = sqr(2.0f / 3.0f * verticalDistance);
        const float b2 = sqr(verticalDistance) / 4.0f;
        const float a = sqrtf(c2 - b2);

        pos0.x = pos1.x - a;
        pos3.x = pos2.x + a;

        return { 
            SDL_Vertex({ .position = pos0, .color = col::NBLACK }),
            SDL_Vertex({ .position = pos1, .color = col::NBLACK }),
            SDL_Vertex({ .position = pos2, .color = col::NBLACK }),
            SDL_Vertex({ .position = pos3, .color = col::NBLACK }),
            SDL_Vertex({ .position = pos4, .color = col::NBLACK }),
            SDL_Vertex({ .position = pos5, .color = col::NBLACK })
        };
    }

    std::array<SDL_Vertex, 48> generateStroke()
    {
        std::array<SDL_Vertex, 48> result;

        const std::vector<SDL_Vertex> strokeVertices =
            sideParts 
            | Seq::pairwise()
            | Seq::map([](const std::pair<SDL_Vertex, SDL_Vertex>& pair)
                {
                    const Vec2 startVertex = Utility::fPointToSvl(std::get<0>(pair).position);
                    const Vec2 endVertex = Utility::fPointToSvl(std::get<1>(pair).position);

                    Vec2 direction = endVertex - startVertex;
                    normalise(direction);

                    Vec2 perpendicular(-direction[1], direction[0]);
                    perpendicular *= THICKNESS / 2.0f;

                    const std::array<SDL_FPoint, 4> strokeSegmentCorners = {
                        Utility::svlToFpoint(startVertex + perpendicular),
                        Utility::svlToFpoint(startVertex - perpendicular),
                        Utility::svlToFpoint(endVertex + perpendicular),
                        Utility::svlToFpoint(endVertex - perpendicular)
                    };

                    // ┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓
                    // ┃ Triangle Strip Imitation ┃
                    // ┃ <0, 1, 2> <1, 2, 3>      ┃
                    // ┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛

                    return std::array<SDL_Vertex, 6>{
                        SDL_Vertex({ .position = strokeSegmentCorners[0], .color = col::NBLUE }),
                        SDL_Vertex({ .position = strokeSegmentCorners[1], .color = col::NBLUE }),
                        SDL_Vertex({ .position = strokeSegmentCorners[2], .color = col::NBLUE }),
                        SDL_Vertex({ .position = strokeSegmentCorners[1], .color = col::NBLUE }),
                        SDL_Vertex({ .position = strokeSegmentCorners[2], .color = col::NBLUE }),
                        SDL_Vertex({ .position = strokeSegmentCorners[3], .color = col::NBLUE })
                    };
                })
            | Seq::flatten();

        ASSERT(strokeVertices.size() == 36, "Stroke should use 6 vertex per 6 sides");
        std::move(strokeVertices.begin(), strokeVertices.end(), result.begin());
        
        // ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
        // ┃ Magic Numbers For Sewing Stroke Segments Together ┃
        // ┃ <4, 5, 7> <10, 11, 13> <22, 23, 25> <28, 29, 31>  ┃
        // ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
        
        result[36] = result[4];
        result[37] = result[5];
        result[38] = result[7];

        result[39] = result[10];
        result[40] = result[11];
        result[41] = result[13];

        result[42] = result[22];
        result[43] = result[23];
        result[44] = result[25];

        result[45] = result[28];
        result[46] = result[29];
        result[47] = result[31];

        return result;
    }

public:
    Hexagon(const SDL_FRect availableArea, std::string_view text, const bool lineNeeded = true)
        : text(text)
        , middlePart(availableArea)
        , sideParts(calculateSidesFromMiddlePart())
        , stroke(generateStroke())
        , horizontalLine(lineNeeded
                         ? Option::Some(SDL_FRect({
                                .x = 0,
                                .y = sideParts[0].position.y - THICKNESS / 2.0f,
                                .w = static_cast<float>(Globals::Properties.value().ScreenWidth),
                                .h = THICKNESS 
                            }))
                         : Option::None<SDL_FRect>())
    {}

    void draw() const
    {
        SDL_Renderer* const renderer = Globals::Properties.value().Renderer;

        if (horizontalLine.isSome())
        {
            Utility::changeDrawColorTo(renderer, col::BLUE);
            SDL_RenderFillRect(renderer, &horizontalLine.value());
        }

        Utility::changeDrawColorTo(renderer, col::BLACK);
        SDL_RenderFillRect(renderer, &middlePart);
        
        static constexpr std::array<int32_t, 6> sidePartTriangleIndices = { 0, 1, 5, 2, 3, 4 };
        SDL_RenderGeometry(renderer, nullptr, sideParts.data(), sideParts.size(), sidePartTriangleIndices.data(), sidePartTriangleIndices.size());
        SDL_RenderGeometry(renderer, nullptr, stroke.data(), stroke.size(), nullptr, 0);
    }
};
