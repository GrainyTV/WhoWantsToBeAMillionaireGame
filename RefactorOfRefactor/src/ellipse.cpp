#include "ellipse.hpp"
#include "seq/seq.hpp"
#include "utility.hpp"
#include <cmath>
#include <numbers>

#include "border.hpp"
#include "fontmanager.hpp"

namespace EllipseModule
{
    // using _impl_details::STEPS;
    // using _impl_details::EllipseDescriptor;
    // using glm::vec2;

    namespace
    {
        using glm::vec2;

        constexpr float ELLIPSE_MODIFIER = 2.3f;
        constexpr float TWO_PI = 2.0f * std::numbers::pi_v<float>;

        struct EllipseDescriptor
        {
            vec2 Center;
            float RadiusHorizontal;
            float RadiusVertical;
        };

        //template<typename T>
        //float sgn(T val)
        //{
        //    return static_cast<float>((T(0) < val) - (val < T(0)));
        //}

        vec2 evaluateAtT(float t, const EllipseDescriptor& desc)
        {
            constexpr float POWER = 2.0f / ELLIPSE_MODIFIER;
            
            const float a = desc.RadiusHorizontal;
            const float b = desc.RadiusVertical;

            const float xShift = desc.Center.x;
            const float yShift = desc.Center.y;

            const float cosT = glm::cos(t);
            const float sinT = glm::sin(t);

            return {
                glm::pow(glm::abs(cosT), POWER) * a * glm::sign(cosT) + xShift,
                glm::pow(glm::abs(sinT), POWER) * b * glm::sign(sinT) + yShift
            };
        }
        
        std::array<vec2, POLY_COUNT + 1> createEllipse(const EllipseDescriptor& desc)
        {
            return Seq::init<POLY_COUNT + 1>([&desc](size_t i)
            {
                const float t = static_cast<float>(i) / POLY_COUNT * TWO_PI;
                return evaluateAtT(t, desc);
            });
        }

        auto createEllipseDescriptor(SDL_FRect area) -> EllipseDescriptor
        {
            return {
                .Center = Utility::centerPointOfRectangle(area),
                .RadiusHorizontal = area.w / 2.0f,
                .RadiusVertical = area.h / 2.0f,
            };
        }

        auto createAreaFrom(const EllipseDescriptor& desc, float spaceReductionMultiplier) -> SDL_FRect
        {
            return {
                .x = desc.Center.x - desc.RadiusHorizontal * spaceReductionMultiplier,
                .y = desc.Center.y - desc.RadiusVertical * spaceReductionMultiplier,
                .w = 2.0f * desc.RadiusHorizontal * spaceReductionMultiplier,
                .h = 2.0f * desc.RadiusVertical * spaceReductionMultiplier,
            };
        }
    }

    //EllipseButton init(const SDL_FRect area, float spaceReductionMultiplier)
    //{
    //    const vec2 center = Utility::centerPointOfRectangle(area);
    //    const EllipseDescriptor desc({
    //        .Center = center,
    //        .RadiusHorizontal = area.w / 2.0f,
    //        .RadiusVertical = area.h / 2.0f,
    //    });

    //    const auto vertices = Ellipse::generate(desc);
    //    const auto vertexCount = static_cast<int32_t>(vertices.size() + 1);

    //    return {
    //        .CpuProperties = {
    //            .Vertices = vertices,
    //            .InnerArea = {
    //                .x = center.x - desc.RadiusHorizontal * spaceReductionMultiplier,
    //                .y = center.y - desc.RadiusVertical * spaceReductionMultiplier,
    //                .w = 2.0f * desc.RadiusHorizontal * spaceReductionMultiplier,
    //                .h = 2.0f * desc.RadiusVertical * spaceReductionMultiplier,
    //            },
    //        },
    //        .GpuProperties = {
    //            .Color = Color::NGRAY,
    //            .Ellipse = { .BufferId = OpenGL::createPrimitives(Seq::singleton(center) | Seq::append(vertices)), .VertexCount = vertexCount },
    //            .Stroke = { .BufferId = OpenGL::createPrimitives(Border::generateFrom(vertices)), .VertexCount = 4 * (vertexCount - 1) }
    //        }
    //    };
    //}

    EllipseButton::EllipseButton(SDL_FRect area, float spaceReductionMultiplier)
        : points(createEllipse(createEllipseDescriptor(area)))
        , innerArea(createAreaFrom(createEllipseDescriptor(area), spaceReductionMultiplier))
        , ellipseFillBuffer(Shader::createAndUploadPrimitives(Seq::singleton(Utility::centerPointOfRectangle(area)) | Seq::append(points), true))
        , strokeBuffer(Shader::createAndUploadPrimitives(Border::createFrom(5, points)))
    {}

    auto EllipseButton::draw(SDL_GPUCommandBuffer* const commandBuffer, SDL_GPURenderPass* const render) const -> void
    {
        Shader::changeDrawColorTo(commandBuffer, Color::NGRAY);
        Shader::renderTriangles(render, ellipseFillBuffer);

        Shader::changeDrawColorTo(commandBuffer, Color::NBLUE);
        Shader::renderTriangles(render, strokeBuffer);

        if (optionalRenderedText.Texture != nullptr)
        {
            Shader::renderTexture(commandBuffer, render, optionalRenderedText);
        }
    }

    auto EllipseButton::whereToRenderTextureAt(const Shader::TextureInfo& textureInfo) const -> SDL_FRect
    {
        return FontManager::alignRenderedTextToCenter(textureInfo, innerArea);
    }

    auto EllipseButton::applyText(std::string_view text) -> void
    {
        optionalRenderedText = FontManager::generateFromText(text, innerArea.w, innerArea.h);
    }

    auto EllipseButton::renderedText() const -> const Shader::TextureInfo&
    {
        return optionalRenderedText;
    }
}
