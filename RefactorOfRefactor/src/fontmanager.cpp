#include "fontmanager.hpp"
#include "debug.hpp"
#include "color.hpp"
#include "functionals.hpp"
#include "option.hpp"
#include "result.hpp"
#include "SDL3_ttf/SDL_ttf.h"
#include "textbubble.hpp"
#include "functionals.hpp"

using namespace Functionals;

static constexpr const char* DEFAULT_FONT = "assets/fonts/ArgentumSans-Bold.otf";
static constexpr uint32_t DEFAULT_FONT_SIZE = 64;
static auto fontResource = Option<TTF_Font*>::None();

template<size_t N>
static uint32_t findSuitableFontSize(const std::array<TextBubble, N>& sceneTextData, const uint32_t size)
{
    TTF_Font* const font = fontResource.value();
    TTF_SetFontSize(font, size);

    std::span<const TextBubble> allFits = filter(sceneTextData, [&](const TextBubble& data)
    {
        const std::string& text = data.Text;
        const SDL_FRect area = data.Area;
        
        int32_t width = 0;
        int32_t height = 0;
        
        TTF_SizeUTF8(font, text.c_str(), &width, &height);
        return width < 0.75f * area.w && height < 0.5f * area.h;
    });

    if (allFits.size() == sceneTextData.size())
    {
        return size;
    }

    __TAILREC__
    return findSuitableFontSize(sceneTextData, size - 2);
}

void FontManager::init()
{
    const auto loadedFont = Result<TTF_Font*>(
        TTF_OpenFont(DEFAULT_FONT, DEFAULT_FONT_SIZE),
        "Failed to load the game's font"
    );
    loadedFont.assertOk();

    fontResource = Option<TTF_Font*>::Some(loadedFont.value());
}

void FontManager::deinit()
{
    TTF_CloseFont(fontResource.value());
}

SDL_Surface* FontManager::generateNew(std::string_view text)
{
    return TTF_RenderUTF8_Blended(fontResource.value(), text.data(), col::WHITE);
}

// uint32_t FontManager::findSuitableFontSize(const std::vector<TextBubble>& sceneTextData)
// {
//     return findSuitableFontSize(sceneTextData, DEFAULT_FONT_SIZE);

//     // TTF_Font* const font = fontResource.value();
//     // TTF_SetFontSize(font, size);

//     // std::span<const TextBubble> allFits = filter(sceneTextData, [&](const TextBubble& data)
//     // {
//     //     const std::string& text = data.getText();
//     //     SDL_FRect area = data.getHoldingArea();
//     //     int32_t width = 0;
//     //     int32_t height = 0;
        
//     //     TTF_SizeUTF8(font, text.c_str(), &width, &height);
//     //     return width < 0.75f * area.w && height < 0.5f * area.h;
//     // });

//     // if (allFits.size() == sceneTextData.size())
//     // {
//     //     return size;
//     // }

//     // __TAILREC__
//     // return findSuitableFontSize(sceneTextData, size - 2);
// }

template<size_t N>
uint32_t FontManager::findSuitableFontSize(const std::array<TextBubble, N>& sceneTextData)
{
    return findSuitableFontSize(sceneTextData, DEFAULT_FONT_SIZE);
}

template uint32_t FontManager::findSuitableFontSize(const std::array<TextBubble, 4>&);
