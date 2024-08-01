#pragma once
#include "SDL3_ttf/SDL_ttf.h"
#include "textbubble.hpp"
#include <span>
#include <string_view>

static constexpr const char* DEFAULT_FONT = "assets/fonts/ArgentumSans-Bold.otf";
static constexpr uint32_t DEFAULT_FONT_SIZE = 64;

class FontManager
{
private:
    TTF_Font* font;

public:
    void init();

    void deinit() const;

    SDL_Surface* generateNew(std::string_view text) const;

    uint32_t findSuitableFontSize(const std::span<TextBubble> sceneTextData, const uint32_t size = DEFAULT_FONT_SIZE) const;
};