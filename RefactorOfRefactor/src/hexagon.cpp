#include "hexagon.hpp"
#include "fontmanager.hpp"
#include "functionals.hpp"
#include "globals.hpp"
#include "utility.hpp"
#include <numeric>

namespace Hexagon
{
    using OpenGL::TextureGpu;
    using _impl_details::HexagonInstance;
    using _impl_details::HexagonGpu;
    using _impl_details::HexagonRenderProperties;
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

        std::array<vec2, 6> calculatePositions(const SDL_FRect area)
        {
            const std::array<vec2, 4> middlePart = Utility::cornersOfRectangle(area);

            const float verticalDistance = middlePart[2][1] - middlePart[0][1];
            const float midPoint = std::midpoint(middlePart[2][1], middlePart[0][1]);

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

            const float cSquared = glm::pow(2.0f / 3.0f * verticalDistance, 2.0f);
            const float bSquared = glm::pow(verticalDistance, 2.0f) / 4.0f;
            const float aSide = glm::sqrt(cSquared - bSquared);

            const vec2 leftEdge(middlePart[0][0] - aSide, midPoint);
            const vec2 rightEdge(middlePart[1][0] + aSide, midPoint);

            return { leftEdge, middlePart[0], middlePart[1], rightEdge, middlePart[3], middlePart[2]  };
        }

        std::array<vec2, 48> calculateStroke(float thickness, std::span<const vec2> hexagonPositions)
        {
            std::array<vec2, 48> result;

            auto createQuadFromEndpoints = [=](const std::pair<vec2, vec2>& pair)
            {
                const vec2 startVertex = std::get<0>(pair);
                const vec2 endVertex = std::get<1>(pair);

                vec2 direction = endVertex - startVertex;
                direction = glm::normalize(direction);

                vec2 perpendicular(-direction[1], direction[0]);
                perpendicular *= thickness / 2.0f;

                const std::array<vec2, 4> strokeSegmentCorners = {
                    startVertex + perpendicular,
                    startVertex - perpendicular,
                    endVertex + perpendicular,
                    endVertex - perpendicular
                };

                return std::array {
                    strokeSegmentCorners[0],
                    strokeSegmentCorners[1],
                    strokeSegmentCorners[2],
                    strokeSegmentCorners[1],
                    strokeSegmentCorners[2],
                    strokeSegmentCorners[3]
                };
            };

            const std::vector<vec2> strokeVertices =
                hexagonPositions 
                | Seq::pairwiseWrap()
                | Seq::map(createQuadFromEndpoints)
                | Seq::flatten();

            Debug::assert(strokeVertices.size() == 36, "Stroke should use 6 vertex per 6 sides");
            std::move(strokeVertices.begin(), strokeVertices.end(), result.begin());
            
            // ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
            // ┃ Magic Numbers For Sewing Stroke Segments Together ┃
            // ┃ <4, 5, 7> <10, 11, 13> <22, 23, 25> <28, 29, 31>  ┃
            // ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
            
            result[36] = result[4];
            result[37] = result[5];
            result[38] = result[7];

            result[39] = result[10];
            result[40] = result[11];
            result[41] = result[13];

            result[42] = result[22];
            result[43] = result[23];
            result[44] = result[25];

            result[45] = result[28];
            result[46] = result[29];
            result[47] = result[31];

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
        const std::array hexagonPositions = calculatePositions(area);
        const std::array hexagonStroke = calculateStroke(THICKNESS, hexagonPositions);
        const std::array hexagonStrokeLine = calculateStrokeLine(THICKNESS, hexagonPositions[0]);

        // ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
        // ┃ Reorder vertices for triangle strip render ┃
        // ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

        const std::array hexagonBackground = {
            hexagonPositions[0], hexagonPositions[1], hexagonPositions[5],
            hexagonPositions[2], hexagonPositions[4], hexagonPositions[3]
        };

        return {
            .CpuProperties = {
                .Positions = hexagonPositions,
                .AvailableArea = area,
                .TextArea = textArea
            },
            .GpuProperties = {
                .RenderedText = (text.empty() == false) ? defineText(textArea, text) : TextureGpu(),
                .Background = { .BufferId = OpenGL::createPrimitives(hexagonBackground), .VertexCount = hexagonBackground.size() },
                .Border = { .BufferId = OpenGL::createPrimitives(hexagonStroke), .VertexCount = hexagonStroke.size() },
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
        OpenGL::renderTrianglesPacked(hex.Background.BufferId, hex.Background.VertexCount);

        OpenGL::changeDrawColorTo(Color::NBLUE);
        OpenGL::renderTriangles(hex.Border.BufferId, hex.Border.VertexCount);

        if (props.TextVisible)
        {
            OpenGL::renderTexture(hex.RenderedText);
        }
    }

    bool isCursorInside(std::span<const vec2> positions, const vec2 p0)
    {
        // ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
        // ┃ Using cross product with all sides                     ┃
        // ┃ Same sign means the point is inside the convex hexagon ┃
        // ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

        auto crossProductOfEdge = [=](const std::pair<vec2, vec2>& neighbor)
        {
            const vec2 ab = neighbor.second - neighbor.first;
            const vec2 ap = p0 - neighbor.first;
            return ab[0] * ap[1] - ab[1] * ap[0];
        };

        return
            positions
            | Seq::pairwiseWrap()
            | Seq::map(crossProductOfEdge)
            | Seq::forall(Utility::isPositive);
    }
}
