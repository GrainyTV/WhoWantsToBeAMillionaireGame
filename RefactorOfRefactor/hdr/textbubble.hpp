#pragma once
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "globals.hpp"
#include "invokable.hpp"
#include "option.hpp"
#include "textureregion.hpp"
#include <cstdint>
#include <numeric>
#include <string>
#include <vector>
#include "utility.hpp"
#include "functionals.hpp"
#include "color.hpp"

static constexpr uint8_t THICKNESS{ 5 };

using namespace Functionals;

// =========== //
//   1-----2   //
//  /|     |\  //
// 0 |     | 3 //
//  \|     |/  //
//   5-----4   //
// =========== //

class TextBubble
{
private:
    bool hover;
    //std::string text;
    //SDL_FRect innerRectangle;
    std::array<SDL_FPoint, 6> coords;
    std::array<SDL_Vertex, 3> leftTriangle;
    std::array<SDL_Vertex, 3> rightTriangle;
    std::vector<SDL_Vertex> strokeSegments;
    //TextureRegion label;
    Invokable onClick;
    Option<SDL_FRect> strokeLine = Option<SDL_FRect>::None();

private:
    std::array<SDL_FPoint, 6> retrieveCoordinatesOfHexagon()
    {
        // const SDL_FPoint pos1 = { .x = innerRectangle.x, .y = innerRectangle.y };
        // const SDL_FPoint pos2 = { .x = innerRectangle.x + innerRectangle.w, .y = innerRectangle.y };
        // const SDL_FPoint pos4 = { .x = innerRectangle.x + innerRectangle.w, .y = innerRectangle.y + innerRectangle.h };
        // const SDL_FPoint pos5 = { .x = innerRectangle.x, .y = innerRectangle.y + innerRectangle.h };

        const SDL_FPoint pos1 = { .x = Area.x, .y = Area.y };
        const SDL_FPoint pos2 = { .x = Area.x + Area.w, .y = Area.y };
        const SDL_FPoint pos4 = { .x = Area.x + Area.w, .y = Area.y + Area.h };
        const SDL_FPoint pos5 = { .x = Area.x, .y = Area.y + Area.h };

        const float verticalDistance = pos5.y - pos1.y;
        const float midPoint = std::midpoint(pos5.y, pos1.y);
        SDL_FPoint pos0 = { .y = midPoint };
        SDL_FPoint pos3 = { .y = midPoint };

        // Solving Pythagorean Theorem for (a) side
        //
        //      /|
        //     / |
        //  c /  | b
        //   /   |
        //  /____|
        //     a
        //
        // c = 2/3 * verticalDistance
        // b = 1/2 * verticalDistance
        // a = ?

        const float c2 = sqr(2.0f / 3.0f * verticalDistance);
        const float b2 = sqr(verticalDistance) / 4.0f;
        const float a = sqrtf(c2 - b2);

        pos0.x = pos1.x - a;
        pos3.x = pos2.x + a;

        return std::array<SDL_FPoint, 6>{ pos0, pos1, pos2, pos3, pos4, pos5 };
    }

    std::vector<SDL_Vertex> generateStroke()
    {
        std::vector<SDL_Vertex> results;

        forEach(coords, [&](const auto& /*coord*/, const size_t i) {
            const size_t j = (i + 1) % coords.size();
            const Vec2 startVertex = Utility::fPointToSvl(coords[i]);
            const Vec2 endVertex = Utility::fPointToSvl(coords[j]);

            Vec2 direction = endVertex - startVertex;
            normalise(direction);

            Vec2 perpendicular(-direction[1], direction[0]);
            perpendicular *= THICKNESS / 2.0f;

            const std::array<Vec2, 4> areaCorners = {
                startVertex + perpendicular,
                startVertex - perpendicular,
                endVertex + perpendicular,
                endVertex - perpendicular,
            };

            const std::array<size_t, 6> indices = { 0, 1, 2, 1, 2, 3 };

            const std::vector<SDL_Vertex> strokeVertices = map(indices, [&](const size_t index) {
                ASSERT(index >= 0 && index < areaCorners.size(), "Index outside the bounds of the array");
                return SDL_Vertex({
                    .position = Utility::svlToFpoint(areaCorners[index]),
                    .color = col::NBLUE,
                });
            });

            results.insert(results.end(), strokeVertices.begin(), strokeVertices.end());
        });

        // ================================================================ //
        // Generate fillings for the small gaps in-between stroke sequences //
        // ================================================================ //

        forEach(coords, [&](const auto& /*coord*/, const size_t i) {
            const size_t j = (i + 1) % coords.size();

            // ======================== //
            // Triangle Strip Imitation //
            // ======================== //
            // [0] [1] [2] [1] [2] [3]  //
            // ======================== //

            constexpr size_t offset = 6;

            const size_t selectedVertex1 = j;
            const size_t selectedVertex2 = j * offset + 1;
            const size_t selectedVertex3 = i * offset + 5;

            results.push_back({ .position = coords[selectedVertex1], .color = col::NBLUE });
            results.push_back(/*strokeSegments*/results[selectedVertex2]);
            results.push_back(/*strokeSegments*/results[selectedVertex3]);
        });

        return results;
    }

    bool isInsideTriangle(const SDL_FPoint p, const std::array<SDL_Vertex, 3>& triangle) const
    {
        const SDL_FPoint x1 = triangle[0].position;
        const SDL_FPoint x2 = triangle[1].position;
        const SDL_FPoint x3 = triangle[2].position;

        const float area = Utility::areaOfTriangleByItsVertices(x1, x2, x3);
        const float subArea1 = Utility::areaOfTriangleByItsVertices(p, x1, x2);
        const float subArea2 = Utility::areaOfTriangleByItsVertices(p, x1, x3);
        const float subArea3 = Utility::areaOfTriangleByItsVertices(p, x2, x3);

        return (subArea1 + subArea2 + subArea3) - area < 0.01f;
    }

public:
    SDL_FRect Area;
    std::string Text;
    TextureRegion Label;

    /*TextBubble(const SDL_FRect& mainArea, const std::string& text, const Invokable& onClick, const bool lineNeeded = true)
        : hover(false)
        //, text(text)
        //, innerRectangle(mainArea)
        , coords(retrievePositions())
        , leftTriangle({
              SDL_Vertex{ .position = coords[0], .color = col::NBLACK },
              SDL_Vertex{ .position = coords[1], .color = col::NBLACK },
              SDL_Vertex{ .position = coords[5], .color = col::NBLACK },
          })
        , rightTriangle({
              SDL_Vertex{ .position = coords[2], .color = col::NBLACK },
              SDL_Vertex{ .position = coords[4], .color = col::NBLACK },
              SDL_Vertex{ .position = coords[3], .color = col::NBLACK },
          })
        , strokeSegments(generateStroke())
        , onClick(onClick)
        , strokeLine(lineNeeded
                         ? Option<SDL_FRect>::Some({ .x = 0, .y = coords[0].y - THICKNESS / 2.0f, .w = static_cast<float>(Globals::Properties.value().ScreenWidth), .h = THICKNESS })
                         : Option<SDL_FRect>::None())
    {}*/

    void init(const std::string& text, const Invokable& onClick, const bool lineNeeded = true)
    {
        Text = text;
        (*this).onClick = onClick;

        coords = retrieveCoordinatesOfHexagon();
        
        leftTriangle = {
            SDL_Vertex{ .position = coords[0], .color = col::NBLACK },
            SDL_Vertex{ .position = coords[1], .color = col::NBLACK },
            SDL_Vertex{ .position = coords[5], .color = col::NBLACK },
        };

        rightTriangle = {
            SDL_Vertex{ .position = coords[2], .color = col::NBLACK },
            SDL_Vertex{ .position = coords[4], .color = col::NBLACK },
            SDL_Vertex{ .position = coords[3], .color = col::NBLACK },
        };

        strokeSegments = generateStroke();

        strokeLine = lineNeeded
            ? Option<SDL_FRect>::Some({ .x = 0, .y = coords[0].y - THICKNESS / 2.0f, .w = static_cast<float>(Globals::Properties.value().ScreenWidth), .h = THICKNESS })
            : Option<SDL_FRect>::None();

        hover = false;
    }

    void draw() const
    {
        SDL_Renderer* const renderer = Globals::Properties.value().Renderer;

        if (strokeLine.isSome())
        {
            Utility::changeDrawColorTo(renderer, col::BLUE);
            Utility::drawRectangle(renderer, &strokeLine.value());
        }

        Utility::changeDrawColorTo(renderer, hover ? col::ORANGE : col::BLACK);
        Utility::drawRectangle(renderer, &Area);
        Utility::drawVertices(renderer, leftTriangle);
        Utility::drawVertices(renderer, rightTriangle);

        Utility::changeDrawColorTo(renderer, col::BLUE);
        Utility::drawVertices(renderer, strokeSegments);

        Utility::drawTextureRegion(renderer, Label);
    }

    bool isInsideItsHitbox(const SDL_FPoint& location) const
    {
        // ================================================ //
        // To check if cursor is inside the hexagon,        //
        // we need to check its rectangle and two triangles //
        // ================================================ //

        return SDL_PointInRectFloat(&location, &Area) ||
               isInsideTriangle(location, leftTriangle) ||
               isInsideTriangle(location, rightTriangle);
    }

    void enableHover()
    {
        hover = true;

        forEach(leftTriangle, [&](const auto& /*vertex*/, const size_t i) {
            leftTriangle[i].color = col::NORANGE;
            rightTriangle[i].color = col::NORANGE;
        });
    }

    void disableHover()
    {
        hover = false;

        forEach(leftTriangle, [&](const auto& /*vertex*/, const size_t i) {
            leftTriangle[i].color = col::NBLACK;
            rightTriangle[i].color = col::NBLACK;
        });
    }

    // TextureRegion* getLabel()
    // {
    //     return &label;
    // }

    // SDL_FRect getHoldingArea() const
    // {
    //     return innerRectangle;
    // }

    // const std::string& getText() const
    // {
    //     return text;
    // }

    void click() const
    {
        onClick.execute();
    }
};
