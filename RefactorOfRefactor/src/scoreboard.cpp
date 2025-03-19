#include "scoreboard.hpp"
#include "bezier.hpp"
#include "ellipse.hpp"
#include "functionals.hpp"
#include "utility.hpp"

namespace ScoreboardModule
{
    using _impl_details::ScoreboardGpu;
    using glm::vec2;

    Scoreboard init(std::span<const vec2> referencePositions, const float totalAvailableSpaceTopHalf)
    {
        const float ninetyPercantOfAvailableArea = 0.9f * totalAvailableSpaceTopHalf;
        const float halfWidthOfButton = (referencePositions[3][0] - referencePositions[0][0]) * 0.5f;

        SDL_FRect baseArea({
            .x = referencePositions[2][0] - halfWidthOfButton,
            .y = (totalAvailableSpaceTopHalf - ninetyPercantOfAvailableArea) / 2.0f,
            .w = halfWidthOfButton,
            .h = ninetyPercantOfAvailableArea
        });

        const std::array<vec2, 4> corners = Utility::cornersOfRectangle(baseArea);
        const float radius = std::min(baseArea.w, baseArea.h) * 0.1f;

        const vec2& a0 = corners[0];
        vec2 a1(a0[0] + radius, a0[1]);
        vec2 a2(a0[0], a0[1] + radius);
        
        const vec2& b0 = corners[1];
        vec2 b1(b0[0] - radius, b0[1]);
        vec2 b2(b0[0], b0[1] + radius);
        
        const vec2& c0 = corners[2];
        vec2 c1(c0[0] + radius, c0[1]);
        vec2 c2(c0[0], c0[1] - radius);
        
        const vec2& d0 = corners[3];
        vec2 d1(d0[0] - radius, d0[1]);
        vec2 d2(d0[0], d0[1] - radius);

        const std::array rect1 = { a1, b1, c1, d1 };
        const std::array rect2 = { a2, b2, c2, d2 };

        const std::array bezier1Controls = { a2, a0, a1 };
        std::vector<vec2> bezier1 = Bezier::generateFromControls(bezier1Controls);
        
        const std::array bezier2Controls = { b1, b0, b2 };
        std::vector<vec2> bezier2 = Bezier::generateFromControls(bezier2Controls);
        
        const std::array bezier3Controls = { c1, c0, c2 };
        std::vector<vec2> bezier3 = Bezier::generateFromControls(bezier3Controls);        

        const std::array bezier4Controls = { d2, d0, d1 };
        std::vector<vec2> bezier4 = Bezier::generateFromControls(bezier4Controls);

        std::vector<vec2> border;
        border.insert(border.end(), bezier1.begin(), bezier1.end());
        border.insert(border.end(), bezier2.begin(), bezier2.end());
        border.insert(border.end(), bezier4.begin(), bezier4.end());
        border.insert(border.end(), bezier3.begin(), bezier3.end());

        std::vector<vec2> borderVertices;

        border
        | Seq::pairwiseWrap()
        | Seq::iter([&](const std::pair<vec2, vec2>& neighbors)
            {
                const vec2& a = neighbors.first;
                const vec2& b = neighbors.second;

                vec2 dir = a - b;
                vec2 normal(dir[1], -dir[0]);
                normal = glm::normalize(normal);

                std::array corners = {
                    a + 5.0f / 2.0f * normal,
                    a - 5.0f / 2.0f * normal,
                    b + 5.0f / 2.0f * normal,
                    b - 5.0f / 2.0f * normal
                };

                borderVertices.insert(borderVertices.end(), corners.begin(), corners.end());
            });

        bezier1.insert(bezier1.begin(), a1 + a2 - a0);
        bezier2.insert(bezier2.begin(), b1 + b2 - b0);
        bezier3.insert(bezier3.begin(), c1 + c2 - c0);
        bezier4.insert(bezier4.begin(), d1 + d2 - d0);

        const SDL_FRect innerArea = { 
            .x = bezier1[0][0],
            .y = bezier1[0][1],
            .w = bezier2[0][0] - bezier1[0][0],
            .h = bezier3[0][1] - bezier1[0][1]
        };

        const float smallButtonHeight = innerArea.h * 0.1154f;
        const float smallButtonWidth = innerArea.w * 0.3f;

        const SDL_FRect smallButtonArea1 =
        {
            .x = innerArea.x,
            .y = innerArea.y,
            .w = smallButtonWidth,
            .h = smallButtonHeight
        };
        
        const SDL_FRect smallButtonArea2 =
        {
            .x = innerArea.x + smallButtonWidth + innerArea.w * 0.05f,
            .y = innerArea.y,
            .w = smallButtonWidth,
            .h = smallButtonHeight
        };
        
        const SDL_FRect smallButtonArea3 =
        {
            .x = innerArea.x + 2.0f * smallButtonWidth + innerArea.w * 0.1f,
            .y = innerArea.y,
            .w = smallButtonWidth,
            .h = smallButtonHeight
        };
        
        vec2 center1 = Utility::centerPointOfRectangle(smallButtonArea1);
        std::vector<vec2> ellipse1 = Ellipse::generate({
            .Center = center1,
            .RadiusHorizontal = smallButtonArea1.w / 2.0f,
            .RadiusVertical = smallButtonArea1.h / 2.0f
        });
        ellipse1.insert(ellipse1.begin(), center1);

        vec2 center2 = Utility::centerPointOfRectangle(smallButtonArea2);
        std::vector<vec2> ellipse2 = Ellipse::generate({
            .Center = center2,
            .RadiusHorizontal = smallButtonArea2.w / 2.0f,
            .RadiusVertical = smallButtonArea2.h / 2.0f
        });
        ellipse2.insert(ellipse2.begin(), center2);

        vec2 center3 = Utility::centerPointOfRectangle(smallButtonArea3);
        std::vector<vec2> ellipse3 = Ellipse::generate({
            .Center = center3,
            .RadiusHorizontal = smallButtonArea3.w / 2.0f,
            .RadiusVertical = smallButtonArea3.h / 2.0f
        });
        ellipse3.insert(ellipse3.begin(), center3);

        return {
            .CpuProperties = { 
                .InnerArea = innerArea,
            },
            .GpuProperties = {
                .BackgroundPart1 = OpenGL::createPrimitives(rect1),
                .BackgroundPart2 = OpenGL::createPrimitives(rect2),
                .BackgroundCorner1 = { .BufferId = OpenGL::createPrimitives(bezier1), .VertexCount = static_cast<int32_t>(bezier1.size()) },
                .BackgroundCorner2 = { .BufferId = OpenGL::createPrimitives(bezier2), .VertexCount = static_cast<int32_t>(bezier2.size()) },
                .BackgroundCorner3 = { .BufferId = OpenGL::createPrimitives(bezier3), .VertexCount = static_cast<int32_t>(bezier3.size()) },
                .BackgroundCorner4 = { .BufferId = OpenGL::createPrimitives(bezier4), .VertexCount = static_cast<int32_t>(bezier4.size()) },
                .BackgroundBorder = { .BufferId = OpenGL::createPrimitives(borderVertices), .VertexCount = static_cast<int32_t>(borderVertices.size()) },
                .FiftyFiftyHelpEllipse = { .BufferId = OpenGL::createPrimitives(ellipse1), .VertexCount = static_cast<int32_t>(ellipse1.size()) },
                .PhoneHelpEllipse = { .BufferId = OpenGL::createPrimitives(ellipse2), .VertexCount = static_cast<int32_t>(ellipse2.size()) },
                .AudienceHelpEllipse = { .BufferId = OpenGL::createPrimitives(ellipse3), .VertexCount = static_cast<int32_t>(ellipse3.size()) },
            }
        };
    }

    void draw(const ScoreboardGpu& sb)
    {
        OpenGL::changeDrawColorTo(Color::NBLACK);
        
        OpenGL::renderQuad(sb.BackgroundPart1);
        OpenGL::renderQuad(sb.BackgroundPart2);
        
        OpenGL::renderTrianglesHub(sb.BackgroundCorner1.BufferId, sb.BackgroundCorner1.VertexCount);
        OpenGL::renderTrianglesHub(sb.BackgroundCorner2.BufferId, sb.BackgroundCorner2.VertexCount);
        OpenGL::renderTrianglesHub(sb.BackgroundCorner3.BufferId, sb.BackgroundCorner3.VertexCount);
        OpenGL::renderTrianglesHub(sb.BackgroundCorner4.BufferId, sb.BackgroundCorner4.VertexCount);
        
        OpenGL::changeDrawColorTo(Color::NBLUE);
        OpenGL::renderTrianglesPacked(sb.BackgroundBorder.BufferId, sb.BackgroundBorder.VertexCount);

        OpenGL::changeDrawColorTo(Color::NORANGE);
        OpenGL::renderTrianglesHub(sb.FiftyFiftyHelpEllipse.BufferId, sb.FiftyFiftyHelpEllipse.VertexCount);
        OpenGL::renderTrianglesHub(sb.PhoneHelpEllipse.BufferId, sb.PhoneHelpEllipse.VertexCount);
        OpenGL::renderTrianglesHub(sb.AudienceHelpEllipse.BufferId, sb.AudienceHelpEllipse.VertexCount);
    }
}
