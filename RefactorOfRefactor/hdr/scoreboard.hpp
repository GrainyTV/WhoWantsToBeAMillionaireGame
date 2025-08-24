#pragma once
// #include "asset.hpp"
// #include "opengl.hpp"
#include "ellipse.hpp"
#include "roundedrectangle.hpp"
#include "shader.hpp"
// #include <string_view>

namespace ScoreboardModule
{    
    class Scoreboard
    {
    private:
        RoundedRectangleModule::RoundedRectangle roundedRectangle;
        std::array<SDL_FRect, 3> helpButtonAreas;

    public:
        EllipseModule::EllipseButton FiftyFiftyHelp;
        EllipseModule::EllipseButton PhoneHelp;
        EllipseModule::EllipseButton AudienceHelp;

    public:
        Scoreboard(SDL_FRect, float);

        auto draw(SDL_GPUCommandBuffer* const, SDL_GPURenderPass* const) const -> void;
    };

    //Scoreboard init(SDL_FRect, float);

    //void draw(const Scoreboard&);

    //void lateinit(Ellipse::EllipseButton&, Asset::Identifier);

    //void lateinit(Ellipse::EllipseButton&, std::string_view);
}

