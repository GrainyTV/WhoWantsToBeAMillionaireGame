#pragma once

//#include "fontmanager.hpp"
// #include "debug.hpp"
//#include "colors.hpp"
//#include "functionals.hpp"
//#include "result.hpp"
// #include "SDL3_ttf/SDL_ttf.h"
// //#include "textbubble.hpp"
// #include <span>
// #include <string_view>

// export module FontManager;

// import Color;
// import Option;
// import Result;
// import Functionals;
// import TextBubble;

// static constexpr const char* DEFAULT_FONT = "assets/fonts/ArgentumSans-Bold.otf";
// static constexpr uint32_t DEFAULT_FONT_SIZE = 64;

// auto fontResource = Option<TTF_Font*>::None();

//#include "SDL3/SDL_surface.h"
#include "textbubble.hpp"
#include <cstdint>
#include <string_view>

namespace FontManager
{
    void init();
    // {
    //     const auto loadedFont = Result<TTF_Font*>(
    //         TTF_OpenFont(DEFAULT_FONT, DEFAULT_FONT_SIZE),
    //         "Failed to load the game's font"
    //     );
    //     loadedFont.assertOk();

    //     fontResource = Option<TTF_Font*>::Some(loadedFont.value());
    // }

    void deinit();
    // {
    //     TTF_CloseFont(fontResource.value());
    // }

    SDL_Surface* generateNew(std::string_view text);
    // {
    //     return TTF_RenderUTF8_Blended(fontResource.value(), text.data(), col::WHITE);
    // }

    template<size_t N>
    uint32_t findSuitableFontSize(const std::array<TextBubble, N>& sceneTextData);

    //std::span<const TextBubble> sceneTextData);
    // {
    //     TTF_Font* const font = fontResource.value();
    //     TTF_SetFontSize(font, size);

    //     std::span<const TextBubble> allFits = fut::filter(sceneTextData, [&](const TextBubble& data)
    //     {
    //         const std::string& text = data.getText();
    //         SDL_FRect area = data.getHoldingArea();
    //         int32_t width = 0;
    //         int32_t height = 0;
            
    //         TTF_SizeUTF8(font, text.c_str(), &width, &height);
    //         return width < 0.75f * area.w && height < 0.5f * area.h;
    //     });

    //     if (allFits.size() == sceneTextData.size())
    //     {
    //         return size;
    //     }

    //     // __TAILREC__
    //     return findSuitableFontSize(sceneTextData, size - 2);
    // }
}
