#pragma once
#include "debug.hpp"
#include "seq/seq.hpp"
#include "glm/glm.hpp"
#include <array>
#include <vector>

namespace Border
{
    using glm::vec2;
    constexpr float BORDER_THICKNESS = 5;

    template<size_t VertexCount, size_t OutputVertexCount = 4 * VertexCount>
    constexpr std::array<vec2, 4 * VertexCount> generateFrom(const std::array<vec2, VertexCount>& positions)
    {
        std::array<vec2, OutputVertexCount> result;

        auto createQuadFromEndpoints = [&](const std::pair<vec2, vec2>& pair, const size_t i)
        {
            const vec2& startVertex = pair.first;
            const vec2& endVertex = pair.second;

            const vec2 direction = endVertex - startVertex;
            vec2 perpendicular(-direction.y, direction.x);
            perpendicular = glm::normalize(perpendicular);
            perpendicular *= BORDER_THICKNESS / 2.0f;

            const size_t pairStride = i * 4;

            result[pairStride + 0] = startVertex + perpendicular;
            result[pairStride + 1] = startVertex - perpendicular;
            result[pairStride + 2] = endVertex + perpendicular;
            result[pairStride + 3] = endVertex - perpendicular;
        };

        positions 
        | Seq::pairwiseWrap()
        | Seq::iteri(createQuadFromEndpoints);

        return result;
    }

    inline std::vector<vec2> generateFrom(const std::vector<vec2>& positions)
    {
        std::vector<vec2> result(positions.size() * 4);

        auto createQuadFromEndpoints = [&](const std::pair<vec2, vec2>& pair, const size_t i)
        {
            const vec2& startVertex = pair.first;
            const vec2& endVertex = pair.second;

            const vec2 direction = endVertex - startVertex;
            vec2 perpendicular(-direction.y, direction.x);
            perpendicular = glm::normalize(perpendicular);
            perpendicular *= BORDER_THICKNESS / 2.0f;

            const size_t pairStride = i * 4;

            result[pairStride + 0] = startVertex + perpendicular;
            result[pairStride + 1] = startVertex - perpendicular;
            result[pairStride + 2] = endVertex + perpendicular;
            result[pairStride + 3] = endVertex - perpendicular;
        };

        positions 
        | Seq::pairwiseWrap()
        | Seq::iteri(createQuadFromEndpoints);

        return result;
    }
}
