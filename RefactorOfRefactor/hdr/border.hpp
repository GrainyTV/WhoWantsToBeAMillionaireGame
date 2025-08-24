#pragma once
#include "glm/glm.hpp"
#include "seq/seq.hpp"

namespace Border
{
    template<size_t N>
    constexpr auto createFrom(float thickness, const std::array<glm::vec2, N>& points)
    {
        std::array<glm::vec2, 4 * N> result;

        auto createQuadFromEndpoints = [&result, thickness](size_t i, const std::pair<glm::vec2, glm::vec2>& pair)
        {
            const glm::vec2& startVertex = std::get<0>(pair);
            const glm::vec2& endVertex = std::get<1>(pair);

            const glm::vec2 direction = endVertex - startVertex;
            glm::vec2 perpendicular(-direction.y, direction.x);
            perpendicular = glm::normalize(perpendicular);
            perpendicular *= thickness / 2.0f;

            const size_t stride = i * 4;

            result[stride + 0] = startVertex + perpendicular;
            result[stride + 1] = startVertex - perpendicular;
            result[stride + 2] = endVertex + perpendicular;
            result[stride + 3] = endVertex - perpendicular;
        };

        points
        | Seq::pairwiseWrap()
        | Seq::iteri(createQuadFromEndpoints);

        return result;
    }
}
