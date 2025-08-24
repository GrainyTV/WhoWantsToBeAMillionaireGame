#include "fontmanager.hpp"
#include "SDL3_ttf/SDL_ttf.h"
#include "color.hpp"
#include "defer.hpp"
#include "globals.hpp"
#include "result.hpp"
#include "utility.hpp"
#include <memory>
#include "seq/seq.hpp"

namespace FontManager
{
    namespace
    {        
        constexpr const char* DEFAULT_FONT_NAME = "ArgentumSans-Bold";
        constexpr size_t DEFAULT_FONT_SIZE = 64;
        TTF_Font* fontResource;

        Shader::TextureInfo generateFromTextWithFontSize(std::string_view text, float targetWidth, float targetHeight, size_t fontSize, bool multiLine)
        {
            const bool sizeChanged = TTF_SetFontSize(fontResource, static_cast<float>(fontSize));
            Debug::assert(sizeChanged, "Failed to change size of font");

            SDL_Surface* surface = 
                multiLine
                ? TTF_RenderText_Blended_Wrapped(fontResource, text.data(), text.size(), Color::WHITE, 0)
                : TTF_RenderText_Blended(fontResource, text.data(), text.size(), Color::WHITE);
            DEFER(SDL_DestroySurface, surface);

            if ((*surface).w < targetWidth && (*surface).h < targetHeight)
            {
                if ((*surface).format != SDL_PIXELFORMAT_RGBA32)
                {
                    SDL_Surface* convertedSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
                    DEFER(SDL_DestroySurface, surface);

                    return Shader::createTextureFromSurface(convertedSurface);
                }

                return Shader::createTextureFromSurface(surface);
            }

            return generateFromTextWithFontSize(text, targetWidth, targetHeight, fontSize - 2, multiLine);
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

    Shader::TextureInfo generateFromText(std::string_view text, float targetWidth, float targetHeight)
    {
        return generateFromTextWithFontSize(text, targetWidth, targetHeight, DEFAULT_FONT_SIZE, text | Seq::contains('\n'));
    }

    SDL_FRect alignRenderedTextToCenter(const Shader::TextureInfo& texture, SDL_FRect outerArea)
    {
        const float innerWidth = static_cast<float>((*texture.Metadata).w);
        const float innerHeight = static_cast<float>((*texture.Metadata).h);

        return {
            outerArea.x + (outerArea.w - innerWidth) / 2.0f,
            outerArea.y + (outerArea.h - innerHeight) / 2.0f,
            innerWidth,
            innerHeight
        };
    }
}
