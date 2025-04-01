#pragma once
#include "glm/glm.hpp"
#include <array>
#include <span>
#include <vector>
#include "seq/seq.hpp"

namespace Bezier
{
    namespace _impl_details
    {
        template<size_t CHUNKS>
        requires (CHUNKS > 0)
        consteval std::array<float, CHUNKS + 1> divideUnitLengthIntoEqualChunks()
        {
            std::array<float, CHUNKS + 1> result;
            result[0] = 0;
            result[CHUNKS] = 1;
            
            constexpr float STEP_SIZE = 1.0f / CHUNKS;
            
            Seq::range<1, CHUNKS>() | Seq::iter([&result](std::size_t i) { result[i] = static_cast<float>(i) * STEP_SIZE; });

            return result;
        }
    }

    constexpr std::array T_VALUES = _impl_details::divideUnitLengthIntoEqualChunks<16>();
    constexpr std::size_t N = T_VALUES.size();

    std::array<glm::vec2, N> generateFromControls(std::span<const glm::vec2>);
    
    std::array<glm::vec2, N> generateFromControls(std::initializer_list<const glm::vec2>);
}
