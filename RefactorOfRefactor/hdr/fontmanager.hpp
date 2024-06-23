#pragma once
#include "SDL3_ttf/SDL_ttf.h"
#include <string>

static constexpr const char* DEFAULT_FONT = "assets/fonts/WarowniaBlackExtended.otf";
static constexpr uint32_t DEFAULT_FONT_SIZE = 32;

class FontManager
{
private:
    TTF_Font* font;

public:
    FontManager();

    FontManager(const char* fontResource, const uint32_t fontSize);

    FontManager& operator=(FontManager&& other) noexcept;

    void free() const;

    SDL_Surface* texturize(const std::string& text) const;
};