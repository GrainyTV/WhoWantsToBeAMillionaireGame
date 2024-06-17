#include "textbubble.hpp"
#include "assert.hpp"
#include "colors.hpp"
#include "functionals.hpp"
#include "game.hpp"
#include <array>
#include <cstdint>
#include <numeric>
#include <vector>

TextBubble::TextBubble(const SDL_FRect& mainArea)
    : innerRectangle(mainArea)
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

void TextBubble::render() const
{
    const auto renderer = Game::Renderer;

    {
        SDL_SetRenderDrawColor(renderer, col::BLUE.r, col::BLUE.g, col::BLUE.b, col::BLUE.a);
        const auto render = SDL_RenderFillRect(renderer, &strokeLine);
        ASSERT(render == 0, "Failed to render stroke line ({})", SDL_GetError());
    }

    {
        SDL_SetRenderDrawColor(renderer, col::BLACK.r, col::BLACK.g, col::BLACK.b, col::BLACK.a);
        const auto render = SDL_RenderFillRect(renderer, &innerRectangle);
        ASSERT(render == 0, "Failed to render rectangle of text bubble ({})", SDL_GetError());
    }

    {
        const auto render = SDL_RenderGeometry(renderer, NULL, leftTriangle.data(), leftTriangle.size(), NULL, 0);
        ASSERT(render == 0, "Failed to render left triangle of text bubble ({})", SDL_GetError());
    }

    {
        const auto render = SDL_RenderGeometry(renderer, NULL, rightTriangle.data(), rightTriangle.size(), NULL, 0);
        ASSERT(render == 0, "Failed to render right triangle of text bubble ({})", SDL_GetError());
    }

    {
        SDL_SetRenderDrawColor(renderer, col::BLUE.r, col::BLUE.g, col::BLUE.b, col::BLUE.a);
        const auto render = SDL_RenderGeometry(renderer, NULL, strokeSegments.data(), strokeSegments.size(), NULL, 0);
        ASSERT(render == 0, "Failed to render stroke of text bubble ({})", SDL_GetError());
    }
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

Eigen::Vector2f TextBubble::SDL_FPointToEigen(const SDL_FPoint& sdlPoint)
{
    return Eigen::Vector2f{ sdlPoint.x, sdlPoint.y };
}

SDL_FPoint TextBubble::EigenToSDL_FPoint(const Eigen::Vector2f& eigenVector)
{
    return SDL_FPoint{ eigenVector.x(), eigenVector.y() };
}

std::vector<SDL_Vertex> TextBubble::generateStroke()
{
    std::vector<SDL_Vertex> results;

    fut::forEach(coords, [&](const auto& _, const size_t i)
    {
        const size_t j = (i + 1) % coords.size();
        const Eigen::Vector2f startVertex = (*this).SDL_FPointToEigen(coords[i]);
        const Eigen::Vector2f endVertex = (*this).SDL_FPointToEigen(coords[j]);

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
                .position = (*this).EigenToSDL_FPoint(areaCorners[index]),
                .color = col::NBLUE,
            };
        });

        results.insert(results.end(), strokeVertices.begin(), strokeVertices.end());
    });

    // ================================================================ //
    // Generate fillings for the small gaps in-between stroke sequences //
    // ================================================================ //

    fut::forEach(coords, [&](const auto& _, const size_t i)
    {
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