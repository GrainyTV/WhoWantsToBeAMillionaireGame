#pragma once
#include "SDL3/SDL.h"
#include "textureregion.hpp"
#include <array>
#include <string>
#include <vector>

class TextBubble
{
private:
    bool hover;
    SDL_FRect innerRectangle;
    std::array<SDL_FPoint, 6> coords;
    std::array<SDL_Vertex, 3> leftTriangle;
    std::array<SDL_Vertex, 3> rightTriangle;
    std::vector<SDL_Vertex> strokeSegments;
    SDL_FRect strokeLine;
    TextureRegion text;

private:
    std::array<SDL_FPoint, 6> retrievePositions();

    std::vector<SDL_Vertex> generateStroke();

    bool isInsideTriangle(const SDL_FPoint& p, const std::array<SDL_Vertex, 3>& triangle) const;

public:
    TextBubble(const SDL_FRect& mainArea, const std::string& text);

    void draw() const;

    bool isInsideItsHitbox(const SDL_FPoint& location) const;

    void enableHover();

    void disableHover();
};