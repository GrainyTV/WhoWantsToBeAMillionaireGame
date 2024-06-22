#include "textbubble.hpp"
#include "colors.hpp"
#include "extensions.hpp"
#include "functionals.hpp"
#include "game.hpp"
#include <array>
#include <cstdint>
#include <numeric>
#include <vector>

TextBubble::TextBubble(const SDL_FRect& mainArea)
    : hover(false)
    , innerRectangle(mainArea)
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
    , strokeLine({ .x = 0, .y = coords[0].y - THICKNESS / 2.0f, .w = static_cast<float>(Game::ScreenWidth), .h = THICKNESS })
{
}

void TextBubble::draw() const
{
    const auto renderer = Game::Renderer;

    ext::changeDrawColorTo(renderer, col::BLUE);
    ext::drawRectangle(renderer, &strokeLine);

    ext::changeDrawColorTo(renderer, hover ? col::ORANGE : col::BLACK);
    ext::drawRectangle(renderer, &innerRectangle);
    ext::drawVertices(renderer, leftTriangle);
    ext::drawVertices(renderer, rightTriangle);

    ext::changeDrawColorTo(renderer, col::BLUE);
    ext::drawVertices(renderer, strokeSegments);
}

std::array<SDL_FPoint, 6> TextBubble::retrievePositions()
{
    // =========== //
    //   1-----2   //
    //  /|     |\  //
    // 0 |     | 3 //
    //  \|     |/  //
    //   5-----4   //
    // =========== //

    const SDL_FPoint pos1 = { .x = innerRectangle.x, .y = innerRectangle.y };
    const SDL_FPoint pos2 = { .x = innerRectangle.x + innerRectangle.w, .y = innerRectangle.y };
    const SDL_FPoint pos4 = { .x = innerRectangle.x + innerRectangle.w, .y = innerRectangle.y + innerRectangle.h };
    const SDL_FPoint pos5 = { .x = innerRectangle.x, .y = innerRectangle.y + innerRectangle.h };

    const float distance = pos5.y - pos1.y;
    const float midPoint = std::midpoint(pos5.y, pos1.y);
    SDL_FPoint pos0 = { .y = midPoint };
    SDL_FPoint pos3 = { .y = midPoint };

    // ============================== //
    // Solving a quadratic equation   //
    // to get the X coordinate of the //
    // point that is 2/3 length away  //
    // from the other two vertices.   //
    // ============================== //

    using Array1f = Eigen::Array<float, 1, 1>;

    const Array1f di{ distance };
    const Array1f x2{ pos1.x };
    const Array1f y1{ midPoint };
    const Array1f y2{ pos1.y };

    const auto b = 2 * x2;
    const auto c = x2.square() + (y1 - y2).square() - (di * 2 / 3).square();
    const auto x1 = (b - (b.square() - 4 * c).sqrt()) / 2;
    const auto solution = x2 - x1;

    pos0.x = pos1.x - solution.x();
    pos3.x = pos2.x + solution.x();

    return std::array<SDL_FPoint, 6>{ pos0, pos1, pos2, pos3, pos4, pos5 };
}

std::vector<SDL_Vertex> TextBubble::generateStroke()
{
    std::vector<SDL_Vertex> results;

    fut::forEach(coords, [&](const auto& _, const size_t i) {
        const size_t j = (i + 1) % coords.size();
        const Eigen::Vector2f startVertex = ext::fPointToEigen(coords[i]);
        const Eigen::Vector2f endVertex = ext::fPointToEigen(coords[j]);

        Eigen::Vector2f direction = endVertex - startVertex;
        direction.normalize();

        Eigen::Vector2f perpendicular{ -direction.y(), direction.x() };
        perpendicular *= THICKNESS / 2.0f;

        const Eigen::Vector2f areaCorners[] = {
            startVertex + perpendicular,
            startVertex - perpendicular,
            endVertex + perpendicular,
            endVertex - perpendicular,
        };

        const uint8_t indices[] = { 0, 1, 2, 1, 2, 3 };

        const auto strokeVertices = fut::map(indices, [&](const uint8_t index) {
            return SDL_Vertex{
                .position = ext::eigenToFpoint(areaCorners[index]),
                .color = col::NBLUE,
            };
        });

        results.insert(results.end(), strokeVertices.begin(), strokeVertices.end());
    });

    // ================================================================ //
    // Generate fillings for the small gaps in-between stroke sequences //
    // ================================================================ //

    fut::forEach(coords, [&](const auto& _, const size_t i) {
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
        results.push_back(strokeSegments[selectedVertex2]);
        results.push_back(strokeSegments[selectedVertex3]);
    });

    return results;
}

bool TextBubble::isInsideItsHitbox(const SDL_FPoint& location) const
{
    // ================================================ //
    // To check if cursor is inside the hexagon,        //
    // we need to check its rectangle and two triangles //
    // ================================================ //

    return SDL_PointInRectFloat(&location, &innerRectangle) ||
           isInsideTriangle(location, leftTriangle) ||
           isInsideTriangle(location, rightTriangle);
}

bool TextBubble::isInsideTriangle(const SDL_FPoint& p, const std::array<SDL_Vertex, 3>& triangle) const
{
    const SDL_FPoint x1 = triangle[0].position;
    const SDL_FPoint x2 = triangle[1].position;
    const SDL_FPoint x3 = triangle[2].position;

    const float area = ext::areaOfTriangleByItsVertices(x1, x2, x3);
    const float subArea1 = ext::areaOfTriangleByItsVertices(p, x1, x2);
    const float subArea2 = ext::areaOfTriangleByItsVertices(p, x1, x3);
    const float subArea3 = ext::areaOfTriangleByItsVertices(p, x2, x3);

    return (subArea1 + subArea2 + subArea3) - area < 0.01f;
}

void TextBubble::enableHover()
{
    hover = true;

    fut::forEach(leftTriangle, [&](const auto& _, size_t index) {
        leftTriangle[index].color = col::NORANGE;
        rightTriangle[index].color = col::NORANGE;
    });
}

void TextBubble::disableHover()
{
    hover = false;

    fut::forEach(leftTriangle, [&](const auto& _, size_t index) {
        leftTriangle[index].color = col::NBLACK;
        rightTriangle[index].color = col::NBLACK;
    });
}