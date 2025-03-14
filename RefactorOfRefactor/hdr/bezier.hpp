#pragma once
#include "svl/SVL.h"
#include <span>
#include <vector>

namespace Bezier
{
    namespace _details
    {
        template<size_t CHUNKS>
        requires (CHUNKS > 0)
        consteval std::array<float, CHUNKS + 1> divideUnitLengthIntoEqualChunks()
        {
            std::array<float, CHUNKS + 1> result;
            result[0] = 0;
            result[CHUNKS] = 1;
            
            constexpr float stepSize = 1.0f / CHUNKS;
            
            for (size_t i = 1; i < CHUNKS; ++i)
            {
                result[i] = i * stepSize;
            }

            return result;
        }
    }
    
    std::vector<Vec2> generateFromControls(std::span<const Vec2>);
}
