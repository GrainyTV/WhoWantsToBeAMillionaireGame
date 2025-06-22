#include "ellipse.hpp"
#include "seq/seq.hpp"
#include "utility.hpp"
#include <cmath>
#include <numbers>

#include "border.hpp"

namespace Ellipse
{
    using _impl_details::STEPS;
    using _impl_details::EllipseDescriptor;
    using glm::vec2;

    namespace
    {
        constexpr float N = 2.3f;
        constexpr float TWO_PI = 2.0f * std::numbers::pi_v<float>;

        template<typename T>
        float sgn(T val)
        {
            return static_cast<float>((T(0) < val) - (val < T(0)));
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
        
        std::array<vec2, STEPS + 1> generate(const EllipseDescriptor& desc)
        {
            auto calculateEllipseAtT = [&](const size_t i)
            {
                const float t = static_cast<float>(i) / STEPS * TWO_PI;
                return evaluateAtT(t, desc);
            };

            return Seq::init<STEPS + 1>([&](std::size_t i) { return calculateEllipseAtT(i); });
        }
    }

    EllipseButton init(const SDL_FRect area, float spaceReductionMultiplier)
    {
        const vec2 center = Utility::centerPointOfRectangle(area);
        const EllipseDescriptor desc({
            .Center = center,
            .RadiusHorizontal = area.w / 2.0f,
            .RadiusVertical = area.h / 2.0f,
        });

        const auto vertices = Ellipse::generate(desc);
        const auto vertexCount = static_cast<int32_t>(vertices.size() + 1);

        return {
            .CpuProperties = {
                .Vertices = vertices,
                .InnerArea = {
                    .x = center.x - desc.RadiusHorizontal * spaceReductionMultiplier,
                    .y = center.y - desc.RadiusVertical * spaceReductionMultiplier,
                    .w = 2.0f * desc.RadiusHorizontal * spaceReductionMultiplier,
                    .h = 2.0f * desc.RadiusVertical * spaceReductionMultiplier,
                },
            },
            .GpuProperties = {
                .Color = Color::NGRAY,
                .Ellipse = { .BufferId = OpenGL::createPrimitives(Seq::singleton(center) | Seq::append(vertices)), .VertexCount = vertexCount },
                .Stroke = { .BufferId = OpenGL::createPrimitives(Border::generateFrom(vertices)), .VertexCount = 4 * (vertexCount - 1) }
            }
        };
    }
}
