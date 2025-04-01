#include "hexagon.hpp"
#include "bezier.hpp"
#include "border.hpp"
#include "fontmanager.hpp"
#include "seq/seq.hpp"
#include "globals.hpp"
#include "utility.hpp"
#include <cstddef>
#include <numeric>

namespace Hexagon
{
    using _impl_details::HexagonInstance;
    using _impl_details::HexagonGpu;
    using _impl_details::HexagonRenderProperties;
    using OpenGL::TextureGpu;
    using glm::vec2;

    namespace
    {
        constexpr int32_t UNDEFINED = 0xAA;

        enum class EstimatedSize : std::uint8_t
        {
            SMALL,
            BIG,
            BIG_ML,
        };

        constexpr uint32_t THICKNESS = 5;

        SDL_FRect reserveAreaForText(const SDL_FRect availableArea, const EstimatedSize size = EstimatedSize::SMALL)
        {
            using enum EstimatedSize;

            float widthLimit = UNDEFINED;
            float heightLimit = UNDEFINED;

            switch (size)
            {
                case SMALL:
                {
                    widthLimit = 0.75f;
                    heightLimit = 0.5f;
                    break;
                }

                case BIG:
                {
                    widthLimit = 0.95f;
                    heightLimit = 0.9f;
                    break;
                }

                case BIG_ML:
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

        std::array<vec2, 4> findTwoOppositeSideControls(const std::pair<vec2, vec2> ab)
        {
            const vec2& a = ab.first;
            const vec2& b = ab.second;

            const vec2 aTob(a.x, b.y);
            const vec2 bToa(b.x, a.y);

            return { a, bToa, aTob, b };
        }

        std::array<vec2, 68> calculatePositions(const SDL_FRect area)
        {
            const std::array<vec2, 4> middlePart = Utility::cornersOfRectangle(area);
            const float verticalDistance = middlePart[2].y - middlePart[0].y;
            const float midPoint = std::midpoint(middlePart[2].y, middlePart[0].y);

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

            const vec2 leftEdge(middlePart[0].x - aSide, midPoint);
            const vec2 rightEdge(middlePart[1].x + aSide, midPoint);

            std::array upperRightCorner = Bezier::generateFromControls(
                findTwoOppositeSideControls(std::make_pair(middlePart[1], rightEdge))
            );

            std::array lowerRightCorner = Bezier::generateFromControls(
                findTwoOppositeSideControls(std::make_pair(rightEdge, middlePart[3]))
            );

            std::array lowerLeftCorner = Bezier::generateFromControls(
                findTwoOppositeSideControls(std::make_pair(middlePart[2], leftEdge))
            );

            std::array upperLeftCorner = Bezier::generateFromControls(
                findTwoOppositeSideControls(std::make_pair(leftEdge, middlePart[0]))
            );
            
            std::array<vec2, 68> result;
            std::copy(upperRightCorner.begin(), upperRightCorner.end(), result.begin());
            std::copy(lowerRightCorner.begin(), lowerRightCorner.end(), result.begin() + 17);
            std::copy(lowerLeftCorner.begin(), lowerLeftCorner.end(), result.begin() + 2*17);
            std::copy(upperLeftCorner.begin(), upperLeftCorner.end(), result.begin() + 3*17);

            return result; 
        }

        std::array<vec2, 4> calculateStrokeLine(float thickness, const vec2 referencePoint)
        {
            const float yValue = referencePoint[1];

            const SDL_FRect line({
                .x = 0,
                .y = yValue - THICKNESS / 2.0f,
                .w = static_cast<float>(Globals::Properties.value().ScreenWidth),
                .h = thickness
            });

            return Utility::cornersOfRectangle(line);
        }

        TextureGpu defineText(const SDL_FRect textArea, std::string_view text, bool multiLine = false)
        {
            const TextureGpu textAsTexture = FontManager::generateFromText(text, textArea.w, textArea.h, multiLine);
            const SDL_FRect centerAlignedTextArea = FontManager::centerInsideArea(textAsTexture, textArea);
            OpenGL::defineTextureRenderLocation(textAsTexture, Option::Some(centerAlignedTextArea));

            return textAsTexture;
        }

        Option::Inst<int32_t> findBestCut(std::string_view line)
        {
            auto n = static_cast<int32_t>(line.length());
            int32_t mid = n / 2;
            std::vector<int32_t> whitespaceIndices;
            
            // Collect indices of whitespace characters
            
            for (int32_t i = 0; i < n; ++i)
            {
                if (std::isspace(line[i]) != 0)
                {
                    whitespaceIndices.push_back(i);
                }
            }
            
            // If no whitespace found, return -1 (no cut possible)
            
            if (whitespaceIndices.empty())
            { 
                return Option::None<int32_t>();
            }
            
            // Find the closest whitespace index to the middle
            
            int32_t bestCut = whitespaceIndices[0];
            int32_t minDiff = std::abs(bestCut - mid);
            
            for (int32_t idx : whitespaceIndices)
            {
                int32_t diff = std::abs(idx - mid);
                
                if (diff <= minDiff)
                {
                    bestCut = idx;
                    minDiff = diff;
                }
            }
            
            return Option::Some(bestCut);
        }
    }

    HexagonInstance init(const SDL_FRect area, bool needLine, std::string_view text)
    {
        const SDL_FRect textArea = reserveAreaForText(area);
        
        std::array hexagonPositions = calculatePositions(area);
        const vec2 centerPosition = Utility::centerPointOfRectangle(area);
        const std::array hexagonStroke = Border::generateFrom(hexagonPositions);
        const std::array hexagonStrokeLine = calculateStrokeLine(THICKNESS, centerPosition);

        std::array<vec2, hexagonPositions.size() + 2> hexagonRenderPositions;
        hexagonRenderPositions[0] = centerPosition;
        hexagonRenderPositions[hexagonRenderPositions.size() - 1] = hexagonPositions[0];
        std::copy(hexagonPositions.begin(), hexagonPositions.end(), hexagonRenderPositions.begin() + 1);

        return {
            .CpuProperties = {
                .Positions = hexagonPositions,
                .AvailableArea = area,
                .TextArea = textArea
            },
            .GpuProperties = {
                .RenderedText = (text.empty() == false) ? defineText(textArea, text) : TextureGpu(),
                .Background = { .BufferId = OpenGL::createPrimitives(hexagonRenderPositions), .VertexCount = static_cast<int32_t>(hexagonRenderPositions.size()) },
                .Border = { .BufferId = OpenGL::createPrimitives(hexagonStroke), .VertexCount = static_cast<int32_t>(hexagonStroke.size()) },
                .HorizontalLine = needLine ? OpenGL::createPrimitives(hexagonStrokeLine) : 0
            }
        };
    }

    void lateinit(HexagonInstance& hex, std::string_view text)
    {
        size_t characterCount = text.size();
        EstimatedSize size;

        if (characterCount < 50)
        {
            size = EstimatedSize::SMALL;
        }
        else if (characterCount < 70)
        {
            size = EstimatedSize::BIG;
        }
        else
        {
            size = EstimatedSize::BIG_ML;
        }

        hex.CpuProperties.TextArea = reserveAreaForText(hex.CpuProperties.AvailableArea, size);

        if (size == EstimatedSize::BIG_ML)
        {
            Option::Inst<int32_t> cutIndex = findBestCut(text);
            std::string modifiedText(text);
            
            if (cutIndex.isSome())
            {
                modifiedText.at(cutIndex.value()) = '\n';
            }

            hex.GpuProperties.RenderedText = defineText(hex.CpuProperties.TextArea, modifiedText, true);
        }
        else
        {
            hex.GpuProperties.RenderedText = defineText(hex.CpuProperties.TextArea, text, false);
        }
    }

    void draw(const HexagonGpu& hex, const HexagonRenderProperties& props)
    {
        if (hex.HorizontalLine != 0)
        {
            OpenGL::changeDrawColorTo(Color::NBLUE);
            OpenGL::renderQuad(hex.HorizontalLine);
        }

        OpenGL::changeDrawColorTo(props.ButtonColor);
        OpenGL::renderTrianglesHub(hex.Background.BufferId, hex.Background.VertexCount);

        OpenGL::changeDrawColorTo(Color::NBLUE);
        OpenGL::renderTrianglesPacked(hex.Border.BufferId, hex.Border.VertexCount);

        if (props.TextVisible)
        {
            OpenGL::renderTexture(hex.RenderedText);
        }
    }

    bool isCursorInside(std::span<const vec2> positions, const vec2 p0)
    {
        auto windingNumberDetection = [p0](const std::pair<vec2, vec2>& neighbor)
        {
            const vec2& a = neighbor.first;
            const vec2& b = neighbor.second;
            const bool atLeastOneXIsToTheRight = a.x >= p0.x || b.x >= p0.x;

            if (a.y <= p0.y && p0.y <= b.y && atLeastOneXIsToTheRight)
            {
                return 1;
            }
            
            if (b.y <= p0.y && p0.y <= a.y && atLeastOneXIsToTheRight)
            {
                return -1;
            }
            
            return 0;
        };

        const int32_t windingNumberSum =
            positions
            | Seq::pairwiseWrap()
            | Seq::map(windingNumberDetection)
            | Seq::sum();

        return windingNumberSum != 0;
    }
}
