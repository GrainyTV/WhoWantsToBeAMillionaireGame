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

        SDL_Texture* generateFromTextWithSize(std::string_view text, const size_t size, const std::pair<float, float> target)
        {
            const bool sizeChanged = TTF_SetFontSize(fontResource, static_cast<float>(size));
            ASSERT(sizeChanged, "Failed to change size of font");

            SDL_Surface* const surface = TTF_RenderText_Blended(fontResource, text.data(), text.size(), Color::WHITE);
            DEFER(SDL_DestroySurface, surface);

            float width = static_cast<float>((*surface).w);
            float height = static_cast<float>((*surface).h);
            float targetWidth = target.first;
            float targetHeight = target.second;

            if (width < targetWidth && height < targetHeight)
            {
                return SDL_CreateTextureFromSurface(Globals::Properties.value().Renderer, surface);
            }

            return generateFromTextWithSize(text, size - 2, target);
        }
    }

    void init()
    {
        constexpr const char* FONT_PATH = "assets/fonts/%s.otf";
        const size_t bufferSize = Utility::formattedSize(FONT_PATH, DEFAULT_FONT_NAME);
        std::unique_ptr<char[]> buffer = std::make_unique<char[]>(bufferSize);
        SDL_snprintf(buffer.get(), bufferSize, FONT_PATH, DEFAULT_FONT_NAME);

        const auto font = Result(TTF_OpenFont(buffer.get(), DEFAULT_FONT_SIZE), "Failed to load font");
        ASSERT(font.isOk(), "{}", font.cause());

        fontResource = font.value();
    }

    SDL_Texture* generateFromText(std::string_view text, const std::pair<float, float> target)
    {
        return generateFromTextWithSize(text, DEFAULT_FONT_SIZE, target);
    }

    SDL_FRect centerInsideArea(SDL_Texture* texture, SDL_FRect outerArea)
    {
        float innerWidth;
        float innerHeight;
        SDL_GetTextureSize(texture, &innerWidth, &innerHeight);

        return {
            outerArea.x + (outerArea.w - innerWidth) / 2.0f,
            outerArea.y + (outerArea.h - innerHeight) / 2.0f,
            innerWidth,
            innerHeight
        };
    }
}
