#include "bezier.hpp"
#include "seq/seq.hpp"
#include <array>

namespace Bezier
{
    namespace
    {
        using glm::vec2;

        vec2 evaluateAtT(const std::pair<vec2, vec2>& ab, const float t)
        {
            const vec2& a = ab.first;
            const vec2& b = ab.second;
            return (1.0f - t) * a + t * b;
        }

        vec2 solveSegment(std::span<const vec2> controls, const float t)
        {
            if (controls.size() == 2)
            {
                return evaluateAtT(std::make_pair(controls[0], controls[1]), t);
            }

            std::vector<vec2> subSolutions =
                controls
                | Seq::pairwise()
                | Seq::map([&](const std::pair<vec2, vec2>& controlPair)
                    {
                        return evaluateAtT(controlPair, t);
                    });

            return solveSegment(subSolutions, t);
        }
    }

    std::array<vec2, POLY_COUNT> generateFromControls(std::span<const vec2> controls)
    {
        auto solveForT = [&](const float t)
        {
            return solveSegment(controls, t);
        };

        std::array<vec2, POLY_COUNT> result;
        auto vRes = T_VALUES | Seq::map(solveForT);
        std::move(vRes.begin(), vRes.end(), result.begin());
        return result;
    }

    std::array<glm::vec2, POLY_COUNT> generateFromControls(std::initializer_list<const glm::vec2> controls)
    {
        return generateFromControls(std::span(controls));
    }
}

