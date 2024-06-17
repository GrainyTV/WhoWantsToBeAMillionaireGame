#pragma once
#include "Eigen/Dense"
#include "SDL3/SDL.h"
#include <array>
#include <vector>

class TextBubble
{
private:
    static constexpr uint8_t THICKNESS{ 5 };
    SDL_FRect innerRectangle;
    std::array<SDL_FPoint, 6> coords;
    std::array<SDL_Vertex, 3> leftTriangle;
    std::array<SDL_Vertex, 3> rightTriangle;
    std::vector<SDL_Vertex> strokeSegments;
    SDL_FRect strokeLine;

private:
    std::array<SDL_FPoint, 6> retrievePositions();

    std::vector<SDL_Vertex> generateStroke();

    Eigen::Vector2f SDL_FPointToEigen(const SDL_FPoint& sdlPoint);

    SDL_FPoint EigenToSDL_FPoint(const Eigen::Vector2f& eigenVector);

public:
    TextBubble(const SDL_FRect& mainArea);

    void render() const;
};