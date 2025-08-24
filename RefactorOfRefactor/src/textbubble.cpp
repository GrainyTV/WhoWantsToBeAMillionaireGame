#include "textbubble.hpp"
#include "border.hpp"
#include "fontmanager.hpp"
#include "seq/seq.hpp"
#include "globals.hpp"
#include "utility.hpp"
#include <cstddef>
#include <numeric>

namespace TextBubbleModule
{
    //using _impl_details::HexagonInstance;
    //using _impl_details::HexagonGpu;
    //using _impl_details::HexagonRenderProperties;
    //using OpenGL::TextureGpu;
    using glm::vec2;
    using Utility::RectCorners;

    namespace
    {
        using namespace OptionExtensions;

        const auto& Properties() { return Globals::Properties.value(); }

        constexpr int32_t UNDEFINED = 0xAA;

        constexpr uint32_t BORDER_THICKNESS = 5;

        enum class EstimatedSize : std::uint8_t
        {
            SMALL,
            BIG,
            BIG_ML,
        };

        SDL_FRect reserveInnerAreaForText(SDL_FRect availableArea, EstimatedSize size)
        {
            float widthLimit = UNDEFINED;
            float heightLimit = UNDEFINED;

            switch (size)
            {
                case EstimatedSize::SMALL:
                {
                    widthLimit = 0.75f;
                    heightLimit = 0.5f;
                    break;
                }

                case EstimatedSize::BIG:
                {
                    widthLimit = 0.95f;
                    heightLimit = 0.9f;
                    break;
                }

                case EstimatedSize::BIG_ML:
                {
                    widthLimit = 0.75f;
                    heightLimit = 0.85f;
                    break;
                }
            }

            return { 
                availableArea.x + (1 - widthLimit) * 0.5f * availableArea.w, 
                availableArea.y + (1 - heightLimit) * 0.5f * availableArea.h, 
                widthLimit * availableArea.w, 
                heightLimit * availableArea.h
            };
        }

        std::array<vec2, 4> findInnerControlPointsForBezier(vec2 a, vec2 b)
        {
            const vec2 aTob(a.x, b.y);
            const vec2 bToa(b.x, a.y);

            return { a, bToa, aTob, b };
        }

        std::array<vec2, 4 * Bezier::POLY_COUNT> createTextBubbleFromRectangle(SDL_FRect area)
        {
            const RectCorners corners = Utility::cornersOfRectangle(area);
            const float verticalDistance = glm::abs(corners.TopLeft.y - corners.BottomLeft.y);
            const float midHeight = std::midpoint(corners.TopLeft.y, corners.BottomLeft.y);

            // ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
            // ┃ Solving Pythagorean Theorem for (a) side ┃
            // ┃                                          ┃
            // ┃      /|                                  ┃
            // ┃     / |                                  ┃
            // ┃  c /  | b                                ┃
            // ┃   /   |                                  ┃
            // ┃  /____|                                  ┃
            // ┃    a                                     ┃
            // ┃                                          ┃
            // ┃ c = 2/3 * verticalDistance               ┃
            // ┃ b = 1/2 * verticalDistance               ┃
            // ┃ a = ?                                    ┃
            // ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

            const float cSquared = glm::pow(2.0f * verticalDistance, 2.0f) / 9.0f;
            const float bSquared = glm::pow(verticalDistance, 2.0f) / 4.0f;
            const float aSide = glm::sqrt(cSquared - bSquared);

            const vec2 leftEdge(corners.TopLeft.x - aSide, midHeight);
            const vec2 rightEdge(corners.TopRight.x + aSide, midHeight);

            // We have a hexagon as a base shape now
            // Have to generate a bezier curve running through its points

            const auto segment1 = Bezier::generateFromControls(findInnerControlPointsForBezier(corners.TopRight, rightEdge));
            const auto segment2 = Bezier::generateFromControls(findInnerControlPointsForBezier(rightEdge, corners.BottomRight));
            const auto segment3 = Bezier::generateFromControls(findInnerControlPointsForBezier(corners.BottomLeft, leftEdge));
            const auto segment4 = Bezier::generateFromControls(findInnerControlPointsForBezier(leftEdge, corners.TopLeft));

            return
                Seq::empty<vec2>()
                | Seq::append(segment1)
                | Seq::append(segment2)
                | Seq::append(segment3)
                | Seq::append(segment4); 
        }

        RectCorners cornersOfStrokeLineFrom(float thickness, vec2 centerPoint)
        {
            const SDL_FRect line({
                .x = 0,
                .y = centerPoint.y - thickness / 2.0f,
                .w = static_cast<float>(Properties().ScreenWidth),
                .h = thickness
            });

            return Utility::cornersOfRectangle(line);
        }

        Option<size_t> findBestPlaceToWrapText(std::string_view line)
        {
            const size_t N = line.length();
            const size_t mid = N / 2;
            std::vector<size_t> whitespaceIndices;
            whitespaceIndices.reserve(N);
            
            for (size_t i = 0; i < N; ++i)
            {
                if (std::isspace(line[i]) != 0)
                {
                    whitespaceIndices.emplace_back(i);
                }
            }
            
            if (whitespaceIndices.empty())
            { 
                return None<size_t>();
            }
            
            size_t bestCut = whitespaceIndices[0];
            size_t minDiff = glm::abs(bestCut - mid);
            
            for (size_t idx : whitespaceIndices)
            {
                size_t diff = glm::abs(idx - mid);
                
                if (diff <= minDiff)
                {
                    bestCut = idx;
                    minDiff = diff;
                }
            }
            
            return Some(bestCut);
        }
    }

    TextBubble::TextBubble(SDL_FRect area, const Option<Invokable>& onClick, bool lineNeeded)
        : Click(onClick)
        , TotalArea(area)
        , BackgroundColor(Color::NBLACK)
        , Points(createTextBubbleFromRectangle(TotalArea))
        , IsLineVisible(lineNeeded)
        , IsTextVisible(true)
        //, TextBubbleVertices(createTextBubbleFromRectangle(TotalArea))
        , TextBubbleBuffer(Shader::createAndUploadPrimitives(Seq::singleton(Utility::centerPointOfRectangle(TotalArea)) | Seq::append(Points), true))
        //, StrokeVertices(Border::createFrom(BORDER_THICKNESS, TextBubbleVertices | Seq::tail()))
        , StrokeBuffer(Shader::createAndUploadPrimitives(Border::createFrom(BORDER_THICKNESS, Points)))
        //, StrokeLine(cornersOfStrokeLineFrom(BORDER_THICKNESS, Utility::centerPointOfRectangle(TotalArea)))
        , StrokeLineBuffer(Shader::createAndUploadPrimitives(cornersOfStrokeLineFrom(BORDER_THICKNESS, Utility::centerPointOfRectangle(TotalArea)).asSpan()))
    {}

    auto TextBubble::applyText(std::string_view input) -> void
    {
        size_t characterCount = input.size();
        EstimatedSize estimated =
            characterCount < 50 ? EstimatedSize::SMALL :
            characterCount < 70 ? EstimatedSize::BIG : EstimatedSize::BIG_ML;

        Text = input;
        InnerTextArea = reserveInnerAreaForText(TotalArea, estimated);

        if (estimated == EstimatedSize::BIG_ML)
        {
            Option<size_t> cutIndex = findBestPlaceToWrapText(input);

            if (cutIndex.isSome())
            {
                Text.at(cutIndex.value()) = '\n';
            }
        }
        
        RenderedText = FontManager::generateFromText(Text, InnerTextArea.w, InnerTextArea.h);
        InnerTextArea = FontManager::alignRenderedTextToCenter(RenderedText, InnerTextArea);
    }

    void TextBubble::draw(SDL_GPUCommandBuffer* const commandBuffer, SDL_GPURenderPass* const render) const
    {
        if (IsLineVisible)
        {
            Shader::changeDrawColorTo(commandBuffer, Color::NBLUE);
            Shader::renderTriangles(render, StrokeLineBuffer);
        }

        Shader::changeDrawColorTo(commandBuffer, BackgroundColor);
        Shader::renderTriangles(render, TextBubbleBuffer);

        Shader::changeDrawColorTo(commandBuffer, Color::NBLUE);
        Shader::renderTriangles(render, StrokeBuffer);

        if (IsTextVisible)
        {
            Shader::renderTexture(commandBuffer, render, RenderedText);
        }
    }

    const Shader::TextureInfo& TextBubble::renderedText() const
    {
        return RenderedText;
    }

    SDL_FRect TextBubble::whereToRenderTextAt() const
    {
        return InnerTextArea;
    }

    auto TextBubble::changeBackgroundColorTo(SDL_FColor color) -> void
    {
        BackgroundColor = color;
    }

    auto TextBubble::outerPoints() const -> const decltype(Points)&
    {
        return Points;
    }

    auto TextBubble::click() -> void
    {
        if (Click.isSome())
        {
            Click.value().execute();
        }
    }

    auto TextBubble::hideText() -> void
    {
        IsTextVisible = false;
    }

    auto TextBubble::showText() -> void
    {
        IsTextVisible = true;
    }

    auto TextBubble::availableArea() const -> SDL_FRect
    {
        return TotalArea;
    }
}
