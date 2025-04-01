#include "scoreboard.hpp"
#include "bezier.hpp"
#include "border.hpp"
#include "ellipse.hpp"
#include "seq/seq.hpp"
#include "utility.hpp"

namespace ScoreboardModule
{
    using glm::vec2;

    Scoreboard init(const SDL_FRect referenceArea, const float totalAvailableSpaceTopHalf)
    {
        const float ninetyPercantOfAvailableArea = 0.9f * totalAvailableSpaceTopHalf;
        const float halfWidthOfButton = referenceArea.w * 0.6f;

        const SDL_FRect baseArea({
            .x = referenceArea.x + (1.0f - 0.6f) * referenceArea.w,
            .y = (totalAvailableSpaceTopHalf - ninetyPercantOfAvailableArea) / 2.0f,
            .w = halfWidthOfButton,
            .h = ninetyPercantOfAvailableArea
        });

        Scoreboard sb;
        sb.Rect = RoundedRectangle::init(baseArea);

        const auto buttonAreas = Seq::init<3>([props = sb.Rect.CpuProperties](std::size_t i)
        {
            const float padding = props.AvailableWidth * 0.1f / 3.0f;
            const float smallButtonWidth = props.AvailableWidth * 0.3f;
            const float smallButtonHeight = smallButtonWidth * 0.75f;

            return SDL_FRect({
                .x = props.TopLeft.x + i * (smallButtonWidth + padding),
                .y = props.TopLeft.y,
                .w = smallButtonWidth,
                .h = smallButtonHeight,
            });
        });

        sb.FiftyFiftyHelp = Ellipse::init(buttonAreas[0]);
        sb.PhoneHelp = Ellipse::init(buttonAreas[1]);
        sb.AudienceHelp = Ellipse::init(buttonAreas[2]);

        // const std::array<vec2, 4> corners = Utility::cornersOfRectangle(baseArea);
        // const float radius = std::min(baseArea.w, baseArea.h) * 0.1f;

        // const vec2& a0 = corners[0];
        // vec2 a1(a0.x + radius, a0.y);
        // vec2 a2(a0.x, a0.y + radius);
        
        // const vec2& b0 = corners[1];
        // vec2 b1(b0.x - radius, b0.y);
        // vec2 b2(b0.x, b0.y + radius);
        
        // const vec2& c0 = corners[2];
        // vec2 c1(c0.x + radius, c0.y);
        // vec2 c2(c0.x, c0.y - radius);
        
        // const vec2& d0 = corners[3];
        // vec2 d1(d0.x - radius, d0.y);
        // vec2 d2(d0.x, d0.y - radius);

        // const std::array rect1 = { a1, b1, c1, d1 };
        // const std::array rect2 = { a2, b2, c2, d2 };

        // const std::array bezier1Controls = { a2, a0, a1 };
        // std::array bezier11 = Bezier::generateFromControls(bezier1Controls);
        
        // const std::array bezier2Controls = { b1, b0, b2 };
        // std::array bezier22 = Bezier::generateFromControls(bezier2Controls);
        
        // const std::array bezier3Controls = { c1, c0, c2 };
        // std::array bezier33 = Bezier::generateFromControls(bezier3Controls);        

        // const std::array bezier4Controls = { d2, d0, d1 };
        // std::array bezier44 = Bezier::generateFromControls(bezier4Controls);

        // std::array<vec2, 68> border;
        // std::copy(bezier11.begin(), bezier11.end(), border.begin());
        // std::copy(bezier22.begin(), bezier22.end(), border.begin() + 17);
        // std::copy(bezier44.begin(), bezier44.end(), border.begin() + 2*17);
        // std::copy(bezier33.begin(), bezier33.end(), border.begin() + 3*17);
        // std::array borderVertices = Border::generateFrom(border);

        // std::vector<vec2> bezier1(bezier11.size() + 1);
        // std::vector<vec2> bezier2(bezier22.size() + 1);
        // std::vector<vec2> bezier3(bezier33.size() + 1);
        // std::vector<vec2> bezier4(bezier44.size() + 1);

        // bezier1.at(0) = a1 + a2 - a0;
        // bezier2.at(0) = b1 + b2 - b0;
        // bezier3.at(0) = c1 + c2 - c0;
        // bezier4.at(0) = d1 + d2 - d0;
        
        // std::copy(bezier11.begin(), bezier11.end(), bezier1.begin() + 1);
        // std::copy(bezier22.begin(), bezier22.end(), bezier2.begin() + 1);
        // std::copy(bezier44.begin(), bezier44.end(), bezier3.begin() + 1);
        // std::copy(bezier33.begin(), bezier33.end(), bezier4.begin() + 1);

        // const SDL_FRect innerArea = { 
        //     .x = bezier1[0].x,
        //     .y = bezier1[0].y,
        //     .w = bezier2[0].x - bezier1[0].x,
        //     .h = bezier3[0].y - bezier1[0].y
        // };

        // const float smallButtonWidth = innerArea.w * 0.3f;
        // const float smallButtonHeight = smallButtonWidth * 0.75f;

        // const SDL_FRect smallButtonArea1 =
        // {
        //     .x = innerArea.x,
        //     .y = innerArea.y,
        //     .w = smallButtonWidth,
        //     .h = smallButtonHeight
        // };
        
        // const SDL_FRect smallButtonArea2 =
        // {
        //     .x = innerArea.x + smallButtonWidth + innerArea.w * 0.05f,
        //     .y = innerArea.y,
        //     .w = smallButtonWidth,
        //     .h = smallButtonHeight
        // };
        
        // const SDL_FRect smallButtonArea3 =
        // {
        //     .x = innerArea.x + 2.0f * smallButtonWidth + innerArea.w * 0.1f,
        //     .y = innerArea.y,
        //     .w = smallButtonWidth,
        //     .h = smallButtonHeight
        // };
        
        // vec2 center1 = Utility::centerPointOfRectangle(smallButtonArea1);
        // std::vector<vec2> ellipse1 = Ellipse::generate({
        //     .Center = center1,
        //     .RadiusHorizontal = smallButtonArea1.w / 2.0f,
        //     .RadiusVertical = smallButtonArea1.h / 2.0f
        // });
        // ellipse1.insert(ellipse1.begin(), center1);

        // vec2 center2 = Utility::centerPointOfRectangle(smallButtonArea2);
        // std::vector<vec2> ellipse2 = Ellipse::generate({
        //     .Center = center2,
        //     .RadiusHorizontal = smallButtonArea2.w / 2.0f,
        //     .RadiusVertical = smallButtonArea2.h / 2.0f
        // });
        // ellipse2.insert(ellipse2.begin(), center2);

        // vec2 center3 = Utility::centerPointOfRectangle(smallButtonArea3);
        // std::vector<vec2> ellipse3 = Ellipse::generate({
        //     .Center = center3,
        //     .RadiusHorizontal = smallButtonArea3.w / 2.0f,
        //     .RadiusVertical = smallButtonArea3.h / 2.0f
        // });
        // ellipse3.insert(ellipse3.begin(), center3);
        
        return sb;
    }

    void draw(const Scoreboard& sb)
    {
        const auto& rect = sb.Rect.GpuProperties;
        OpenGL::changeDrawColorTo(Color::NBLACK);
        
        OpenGL::renderQuad(rect.BackgroundSquare1);
        OpenGL::renderQuad(rect.BackgroundSquare2);    
        
        OpenGL::renderTrianglesHub(rect.BackgroundCorner1.BufferId, rect.BackgroundCorner1.VertexCount);
        OpenGL::renderTrianglesHub(rect.BackgroundCorner2.BufferId, rect.BackgroundCorner2.VertexCount);
        OpenGL::renderTrianglesHub(rect.BackgroundCorner3.BufferId, rect.BackgroundCorner3.VertexCount);
        OpenGL::renderTrianglesHub(rect.BackgroundCorner4.BufferId, rect.BackgroundCorner4.VertexCount);
        
        OpenGL::changeDrawColorTo(Color::NBLUE);
        OpenGL::renderTrianglesPacked(rect.Stroke.BufferId, rect.Stroke.VertexCount);

        const auto& help1 = sb.FiftyFiftyHelp.GpuProperties;
        const auto& help2 = sb.PhoneHelp.GpuProperties;
        const auto& help3 = sb.AudienceHelp.GpuProperties;
        OpenGL::changeDrawColorTo(Color::NORANGE);
        OpenGL::renderTrianglesHub(help1.Ellipse.BufferId, help1.Ellipse.VertexCount);
        OpenGL::renderTrianglesHub(help2.Ellipse.BufferId, help2.Ellipse.VertexCount);
        OpenGL::renderTrianglesHub(help3.Ellipse.BufferId, help3.Ellipse.VertexCount);
    }
}
