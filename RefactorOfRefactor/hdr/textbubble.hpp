#pragma once
#include "invokable.hpp"
#include "textureregion.hpp"
#include <array>
#include <string>
#include <vector>

class TextBubble
{
private:
    bool hover;
    std::string text;
    SDL_FRect innerRectangle;
    std::array<SDL_FPoint, 6> coords;
    std::array<SDL_Vertex, 3> leftTriangle;
    std::array<SDL_Vertex, 3> rightTriangle;
    std::vector<SDL_Vertex> strokeSegments;
    SDL_FRect strokeLine;
    TextureRegion label;
    Invokable onClick;

private:
    std::array<SDL_FPoint, 6> retrievePositions();

    std::vector<SDL_Vertex> generateStroke();

    bool isInsideTriangle(const SDL_FPoint& p, const std::array<SDL_Vertex, 3>& triangle) const;

public:
    TextBubble(const SDL_FRect& mainArea, const std::string& text, const Invokable& onClick);

    void draw() const;

    bool isInsideItsHitbox(const SDL_FPoint& location) const;

    void enableHover();

    void disableHover();

    TextureRegion* getLabel();

    SDL_FRect getHoldingArea() const;

    const std::string& getText() const;

    void click() const;
};