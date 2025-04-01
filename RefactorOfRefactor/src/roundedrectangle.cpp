#include "roundedrectangle.hpp"
#include "border.hpp"
#include "glm/glm.hpp"
#undef assert
#include "utility.hpp"
#include <algorithm>
#include <array>
#include "bezier.hpp"
#include "seq/seq.hpp"

namespace RoundedRectangle
{
    using glm::vec2;
    constexpr float RADIUS_PERCENT = 0.1f;

    RoundedRectangleInstance init(const SDL_FRect area)
    {
        const float radius = std::min(area.w, area.h) * RADIUS_PERCENT;
        const std::array<vec2, 4> corners = Utility::cornersOfRectangle(area);
        
        const vec2& a0 = corners[0];
        const vec2 a1(a0.x + radius, a0.y);
        const vec2 a2(a0.x, a0.y + radius);
        const std::array bezierTopLeft = Bezier::generateFromControls({ a2, a0, a1 });
        const vec2 topLeftHub = a1 + a2 - a0;
        
        const vec2& b0 = corners[1];
        const vec2 b1(b0.x - radius, b0.y);
        const vec2 b2(b0.x, b0.y + radius);
        const std::array bezierTopRight = Bezier::generateFromControls({ b1, b0, b2 });
        const vec2 topRightHub = b1 + b2 - b0;
        
        const vec2& c0 = corners[2];
        const vec2 c1(c0.x + radius, c0.y);
        const vec2 c2(c0.x, c0.y - radius);
        const std::array bezierBottomLeft = Bezier::generateFromControls({ c1, c0, c2 });
        const vec2 bottomRightHub = c1 + c2 - c0;
        
        const vec2& d0 = corners[3];
        const vec2 d1(d0.x - radius, d0.y);
        const vec2 d2(d0.x, d0.y - radius);
        const std::array bezierBottomRight = Bezier::generateFromControls({ d2, d0, d1 });
        const vec2 bottomLeftHub = d1 + d2 - d0;

        // ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
        // ┃ All of the corners have the same vertex count ┃
        // ┃ Need +1 for the actual hub at the beginning   ┃
        // ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

        const std::size_t bezierVertexCount = bezierTopLeft.size() + 1;

        const auto strokeVertices =
            bezierTopLeft
            | Seq::append(bezierTopRight)
            | Seq::append(bezierBottomRight)
            | Seq::append(bezierBottomLeft);

        return {
            .CpuProperties = { 
                .TopLeft = topLeftHub,
                .AvailableWidth = topRightHub.x - topLeftHub.x,
                .AvailableHeight = bottomRightHub.y - topRightHub.y
            },
            .GpuProperties = {
                .BackgroundSquare1 = OpenGL::createPrimitives({ a1, b1, c1, d1 }),
                .BackgroundSquare2 = OpenGL::createPrimitives({ a2, b2, c2, d2 }),
                .BackgroundCorner1 = { .BufferId = OpenGL::createPrimitives(Seq::singleton(topLeftHub)     | Seq::append(bezierTopLeft)), .VertexCount = bezierVertexCount },
                .BackgroundCorner2 = { .BufferId = OpenGL::createPrimitives(Seq::singleton(topRightHub)    | Seq::append(bezierTopRight)), .VertexCount = bezierVertexCount },
                .BackgroundCorner3 = { .BufferId = OpenGL::createPrimitives(Seq::singleton(bottomRightHub) | Seq::append(bezierBottomRight)), .VertexCount = bezierVertexCount },
                .BackgroundCorner4 = { .BufferId = OpenGL::createPrimitives(Seq::singleton(bottomLeftHub)  | Seq::append(bezierBottomLeft)), .VertexCount = bezierVertexCount },
                .Stroke = { .BufferId = OpenGL::createPrimitives(Border::generateFrom(strokeVertices)), .VertexCount = static_cast<int32_t>(strokeVertices.size() * 4) }
            }
        };
    }
}
