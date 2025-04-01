#include "ellipse.hpp"
#include "seq/seq.hpp"
#include "utility.hpp"
#include <cmath>

namespace Ellipse
{
    using _impl_details::EllipseDescriptor;
    using glm::vec2;

    namespace
    {
        constexpr int32_t STEPS = 48;
        constexpr float N = 2.3f;
        constexpr float TWO_PI = 2.0f * M_PIf;

        template<typename T>
        float sgn(T val)
        {
            return (T(0) < val) - (val < T(0));
        }

        vec2 evaluateAtT(const float t, const EllipseDescriptor& desc)
        {
            constexpr float POWER = 2.0f / N;
            
            const float a = desc.RadiusHorizontal;
            const float b = desc.RadiusVertical;

            const float xShift = desc.Center.x;
            const float yShift = desc.Center.y;

            const float cosT = cosf(t);
            const float sinT = sinf(t);

            return {
                powf(std::abs(cosT), POWER) * a * sgn(cosT) + xShift,
                powf(std::abs(sinT), POWER) * b * sgn(sinT) + yShift
            };
        }
    }

    std::vector<vec2> generate(const EllipseDescriptor& desc)
    {
        auto calculateEllipseAtT = [&](const size_t i)
        {
            const float t = static_cast<float>(i) / STEPS * TWO_PI;
            return evaluateAtT(t, desc);
        };

        return Seq::range<0, STEPS + 1>() | Seq::map(calculateEllipseAtT);
    }

    EllipseButton init(const SDL_FRect area)
    {
        const vec2 center = Utility::centerPointOfRectangle(area);

        const std::vector<vec2> vertices = Ellipse::generate({
            .Center = center,
            .RadiusHorizontal = area.w / 2.0f,
            .RadiusVertical = area.h / 2.0f
        });
        
        const auto vertexCount = static_cast<int32_t>(vertices.size() + 1);

        return {
            .GpuProperties = {
                .Ellipse = { .BufferId = OpenGL::createPrimitives(Seq::singleton(center) | Seq::append(vertices)), .VertexCount = vertexCount }
            }
        };
    }
}
