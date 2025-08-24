#include "scoreboard.hpp"
#include "asset.hpp"
#include "bezier.hpp"
#include "border.hpp"
#include "ellipse.hpp"
#include "fontmanager.hpp"
#include "seq/seq.hpp"
#include "utility.hpp"

namespace ScoreboardModule
{
    namespace
    {
        using namespace OptionExtensions;

        auto getUsableAreaFrom(SDL_FRect referenceArea, float totalAvailableSpaceTopHalf) -> SDL_FRect
        {
            const float ninetyPercantOfAvailableHeight = totalAvailableSpaceTopHalf * 0.9f;
            const float availableWidth = referenceArea.w * 0.6f;

            return {
                .x = referenceArea.x + (1.0f - 0.6f) * referenceArea.w,
                .y = (totalAvailableSpaceTopHalf - ninetyPercantOfAvailableHeight) / 2.0f,
                .w = availableWidth,
                .h = ninetyPercantOfAvailableHeight
            };
        }

        auto divideSpaceIntoEqualAreas(SDL_FRect area) -> std::array<SDL_FRect, 3>
        {
            return Seq::init<3>([area](size_t i)
            {
                const float availableWidth = area.w;

                const float padding = availableWidth * 0.1f / 3.0f;
                const float smallButtonWidth = availableWidth * 0.3f;
                const float smallButtonHeight = smallButtonWidth * 0.75f;

                return SDL_FRect({
                    .x = area.x + i * (smallButtonWidth + padding),
                    .y = area.y,
                    .w = smallButtonWidth,
                    .h = smallButtonHeight,
                });
            });
        }
    }

    //using glm::vec2;
    //using enum Asset::Identifier;

    //Scoreboard init(const SDL_FRect referenceArea, const float totalAvailableSpaceTopHalf)
    //{
    //    const float ninetyPercantOfAvailableArea = 0.9f * totalAvailableSpaceTopHalf;
    //    const float halfWidthOfButton = referenceArea.w * 0.6f;

    //    const SDL_FRect baseArea({
    //        .x = referenceArea.x + (1.0f - 0.6f) * referenceArea.w,
    //        .y = (totalAvailableSpaceTopHalf - ninetyPercantOfAvailableArea) / 2.0f,
    //        .w = halfWidthOfButton,
    //        .h = ninetyPercantOfAvailableArea
    //    });

    //    Scoreboard sb;
    //    sb.Rect = RoundedRectangle::init(baseArea);

    //    const auto buttonAreas = Seq::init<3>([props = sb.Rect.CpuProperties](std::size_t i)
    //    {
    //        const float padding = props.AvailableWidth * 0.1f / 3.0f;
    //        const float smallButtonWidth = props.AvailableWidth * 0.3f;
    //        const float smallButtonHeight = smallButtonWidth * 0.75f;

    //        return SDL_FRect({
    //            .x = props.TopLeft.x + i * (smallButtonWidth + padding),
    //            .y = props.TopLeft.y,
    //            .w = smallButtonWidth,
    //            .h = smallButtonHeight,
    //        });
    //    });

    //    sb.FiftyFiftyHelp = Ellipse::init(buttonAreas[0], 0.8f);
    //    sb.PhoneHelp = Ellipse::init(buttonAreas[1], 0.58333f);
    //    sb.AudienceHelp = Ellipse::init(buttonAreas[2], 0.65f);
    //    
    //    return sb;
    //}

    Scoreboard::Scoreboard(SDL_FRect referenceArea, float totalAvailableSpaceTopHalf)
        : roundedRectangle(getUsableAreaFrom(referenceArea, totalAvailableSpaceTopHalf))
        , helpButtonAreas(divideSpaceIntoEqualAreas(roundedRectangle.areaInsidePadding()))
        , FiftyFiftyHelp(helpButtonAreas[0], 0.8f)
        , PhoneHelp(helpButtonAreas[1], 0.58333f)
        , AudienceHelp(helpButtonAreas[2], 0.65f)
    {}

    auto Scoreboard::draw(SDL_GPUCommandBuffer* const commandBuffer, SDL_GPURenderPass* const render) const -> void
    {
        roundedRectangle.draw(commandBuffer, render);
        
        FiftyFiftyHelp.draw(commandBuffer, render);
        PhoneHelp.draw(commandBuffer, render);
        AudienceHelp.draw(commandBuffer, render);

        //const auto& rect = sb.Rect.GpuProperties;
        //OpenGL::changeDrawColorTo(Color::NBLACK);
        //
        //OpenGL::renderQuad(rect.BackgroundSquare1);
        //OpenGL::renderQuad(rect.BackgroundSquare2);    
        //
        //OpenGL::renderTrianglesHub(rect.BackgroundCorner1.BufferId, rect.BackgroundCorner1.VertexCount);
        //OpenGL::renderTrianglesHub(rect.BackgroundCorner2.BufferId, rect.BackgroundCorner2.VertexCount);
        //OpenGL::renderTrianglesHub(rect.BackgroundCorner3.BufferId, rect.BackgroundCorner3.VertexCount);
        //OpenGL::renderTrianglesHub(rect.BackgroundCorner4.BufferId, rect.BackgroundCorner4.VertexCount);
        //
        //OpenGL::changeDrawColorTo(Color::NBLUE);
        //OpenGL::renderTrianglesPacked(rect.Stroke.BufferId, rect.Stroke.VertexCount);

        //const auto& help1 = sb.FiftyFiftyHelp.GpuProperties;
        //const auto& help2 = sb.PhoneHelp.GpuProperties;
        //const auto& help3 = sb.AudienceHelp.GpuProperties;
        //
        //OpenGL::changeDrawColorTo(help1.Color);
        //OpenGL::renderTrianglesHub(help1.Ellipse.BufferId, help1.Ellipse.VertexCount);
        //
        //OpenGL::changeDrawColorTo(help2.Color);
        //OpenGL::renderTrianglesHub(help2.Ellipse.BufferId, help2.Ellipse.VertexCount);
        //
        //OpenGL::changeDrawColorTo(help3.Color);
        //OpenGL::renderTrianglesHub(help3.Ellipse.BufferId, help3.Ellipse.VertexCount);

        //OpenGL::changeDrawColorTo(Color::NBLUE);
        //OpenGL::renderTrianglesPacked(help1.Stroke.BufferId, help1.Stroke.VertexCount);
        //OpenGL::renderTrianglesPacked(help2.Stroke.BufferId, help2.Stroke.VertexCount);
        //OpenGL::renderTrianglesPacked(help3.Stroke.BufferId, help3.Stroke.VertexCount);

        //OpenGL::renderTexture(help1.Icon);
        //OpenGL::renderTexture(help2.Icon);
        //OpenGL::renderTexture(help3.Icon);
    }

    //void lateinit(Ellipse::EllipseButton& ellipse, Asset::Identifier textureId)
    //{
    //    const SDL_FRect& area = ellipse.CpuProperties.InnerArea;
    //    const auto& texture = Asset::getTextureById(textureId);
    //    const float textureWidth = texture.AspectRatio * area.h;

    //    ellipse.GpuProperties.Icon = texture;

    //    OpenGL::defineTextureRenderLocation(texture, Option::Some<SDL_FRect>({ 
    //        .x = area.x + (area.w - textureWidth) / 2.0f,
    //        .y = area.y,
    //        .w = textureWidth,
    //        .h = area.h,
    //    }));
    //}

    //void lateinit(Ellipse::EllipseButton& ellipse, std::string_view text)
    //{
    //    const float width = ellipse.CpuProperties.InnerArea.w;
    //    const float height = ellipse.CpuProperties.InnerArea.h;        
    //    
    //    ellipse.GpuProperties.Icon = FontManager::generateFromText(text, width, height, false);

    //    OpenGL::defineTextureRenderLocation(
    //        ellipse.GpuProperties.Icon,
    //        Option::Some(FontManager::centerInsideArea(ellipse.GpuProperties.Icon, ellipse.CpuProperties.InnerArea))
    //    );
    //}
}
