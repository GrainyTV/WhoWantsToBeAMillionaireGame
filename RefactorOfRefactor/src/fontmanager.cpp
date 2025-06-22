#include "fontmanager.hpp"
#include "SDL3_ttf/SDL_ttf.h"
#include "color.hpp"
#include "defer.hpp"
#include "globals.hpp"
#include "result.hpp"
#include "utility.hpp"
#include <memory>

namespace FontManager
{
    namespace
    {        
        constexpr const char* DEFAULT_FONT_NAME = "ArgentumSans-Bold";
        constexpr size_t DEFAULT_FONT_SIZE = 64;
        TTF_Font* fontResource;

        OpenGL::TextureGpu generateFromTextWithSize(std::string_view text, const size_t size, const float targetWidth, const float targetHeight, const bool multiLine)
        {
            const bool sizeChanged = TTF_SetFontSize(fontResource, static_cast<float>(size));
            Debug::assert(sizeChanged, "Failed to change size of font");

            SDL_Surface* surface = 
                multiLine
                ? TTF_RenderText_Blended_Wrapped(fontResource, text.data(), text.size(), Color::WHITE, 0)
                : TTF_RenderText_Blended(fontResource, text.data(), text.size(), Color::WHITE);
            
            DEFER(SDL_DestroySurface, surface);

            auto width = static_cast<float>((*surface).w);
            auto height = static_cast<float>((*surface).h);

            if (width < targetWidth && height < targetHeight)
            {
                SDL_FlipSurface(surface, SDL_FLIP_VERTICAL);
                SDL_Surface* convertedSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_ABGR8888);
                return OpenGL::createTextureFromSurface(convertedSurface);
            }

            return generateFromTextWithSize(text, size - 2, targetWidth, targetHeight, multiLine);
        }
    }

    void init()
    {
        constexpr const char* FONT_PATH_FMT = "assets/fonts/%s.otf";
        const std::unique_ptr<char[]> fontPath = Utility::formatPath(FONT_PATH_FMT, DEFAULT_FONT_NAME);

        const auto font = Result(TTF_OpenFont(fontPath.get(), DEFAULT_FONT_SIZE), "Failed to load font");
        Debug::assert(font.isOk(), "{} SDL: {}", font.toString(), SDL_GetError());

        fontResource = font.unwrap();
        TTF_SetFontWrapAlignment(fontResource, TTF_HORIZONTAL_ALIGN_CENTER);
    }

    OpenGL::TextureGpu generateFromText(std::string_view text, const float targetWidth, const float targetHeight, const bool multiLine)
    {
        return generateFromTextWithSize(text, DEFAULT_FONT_SIZE, targetWidth, targetHeight, multiLine);
    }

    SDL_FRect centerInsideArea(const OpenGL::TextureGpu texture, const SDL_FRect outerArea)
    {
        auto innerWidth = static_cast<float>(texture.Width);
        auto innerHeight = static_cast<float>(texture.Height);

        return {
            outerArea.x + (outerArea.w - innerWidth) / 2.0f,
            outerArea.y + (outerArea.h - innerHeight) / 2.0f,
            innerWidth,
            innerHeight
        };
    }
}
