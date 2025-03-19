#include "bezier.hpp"
#include "functionals.hpp"

namespace Bezier
{
    using glm::vec2;

    namespace
    {
        template<size_t CHUNKS>
        requires (CHUNKS > 0)
        consteval std::array<float, CHUNKS + 1> divideUnitLengthIntoEqualChunks()
        {
            std::array<float, CHUNKS + 1> result;
            result[0] = 0;
            result[CHUNKS] = 1;
            
            constexpr float STEP_SIZE = 1.0f / CHUNKS;
            
            for (size_t i = 1; i < CHUNKS; ++i)
            {
                result[i] = i * STEP_SIZE;
            }

            return result;
        }

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

        constexpr std::array T_VALUES = divideUnitLengthIntoEqualChunks<8>();
    }

    std::vector<vec2> generateFromControls(std::span<const vec2> controls)
    {
        auto solveForT = [&](const float t)
        {
            return solveSegment(controls, t);
        };

        return T_VALUES | Seq::map(solveForT);
    }
}

