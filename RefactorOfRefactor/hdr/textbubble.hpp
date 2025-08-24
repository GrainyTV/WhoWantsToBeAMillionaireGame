#pragma once
#include "SDL3/SDL.h"
#include "glm/glm.hpp"
#include "invokable.hpp"
#include "bezier.hpp"
#include "utility.hpp"
//#include "opengl.hpp"
#include "shader.hpp"
#include <span>
#include <string_view>
#include <array>

namespace TextBubbleModule
{
    class TextBubble
    {   
    private:
        Option<Invokable> Click;

        Shader::TextureInfo RenderedText;
        std::string Text;
        
        SDL_FRect TotalArea;
        SDL_FRect InnerTextArea;
        SDL_FColor BackgroundColor;

        std::array<glm::vec2, 4 * Bezier::POLY_COUNT> Points;
        
        bool IsEnabled;
        bool IsLineVisible;
        bool IsTextVisible;

        //std::array<glm::vec2, 4 * Bezier::POLY_COUNT + 1> TextBubbleVertices;
        Shader::VertexInfo TextBubbleBuffer;

        //std::array<glm::vec2, 4 * (4 * Bezier::POLY_COUNT)> StrokeVertices;
        Shader::VertexInfo StrokeBuffer;

        //Utility::RectCorners StrokeLine;
        Shader::VertexInfo StrokeLineBuffer;

    public:
        TextBubble(SDL_FRect, const Option<Invokable>& = OptionExtensions::None<Invokable>(), bool = true);

        void applyText(std::string_view);

        void draw(SDL_GPUCommandBuffer* const, SDL_GPURenderPass* const) const;

        const Shader::TextureInfo& renderedText() const;

        SDL_FRect whereToRenderTextAt() const;

        auto changeBackgroundColorTo(SDL_FColor) -> void;

        auto outerPoints() const -> const decltype(Points)&;

        auto click() -> void;

        auto hideText() -> void;

        auto showText() -> void;

        auto availableArea() const -> SDL_FRect;
    };
}
