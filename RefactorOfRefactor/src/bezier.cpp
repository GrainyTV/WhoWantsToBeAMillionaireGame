#include "bezier.hpp"
#include "functionals.hpp"

namespace Bezier
{
    namespace
    {
        constexpr std::array tValues = _details::divideUnitLengthIntoEqualChunks<8>();

        Vec2 evaluateAtT(const std::pair<Vec2, Vec2>& ab, const float t)
        {
            const Vec2& a = ab.first;
            const Vec2& b = ab.second;
            return (1.0f - t) * a + t * b;
        }

        Vec2 solveSegmentWithT(std::span<const Vec2> controls, const float t)
        {
            if (controls.size() == 2)
            {
                return evaluateAtT(std::make_pair(controls[0], controls[1]), t);
            }

            std::vector<Vec2> subSolutions =
                controls
                | Seq::pairwise()
                | Seq::map([&](const std::pair<Vec2, Vec2>& controlPair)
                    {
                        return evaluateAtT(controlPair, t);
                    });

            return solveSegmentWithT(subSolutions, t);
        }
    }

    std::vector<Vec2> generateFromControls(std::span<const Vec2> controls)
    {
        std::vector<Vec2> segments;
        segments.reserve(tValues.size());
        
        for (float t : tValues)
        {
            segments.emplace_back(solveSegmentWithT(controls, t));
        }

        return segments;
    }
}

