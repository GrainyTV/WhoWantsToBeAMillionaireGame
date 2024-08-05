#include "fontmanager.hpp"
#include "debug.hpp"
#include "colors.hpp"
#include "functionals.hpp"

void FontManager::init()
{
    font = TTF_OpenFont(DEFAULT_FONT, DEFAULT_FONT_SIZE);
    ASSERT(font != NULL, "Failed to load the game's font ({})", TTF_GetError());
}

void FontManager::deinit() const
{
    TTF_CloseFont(font);
}

SDL_Surface* FontManager::generateNew(std::string_view text) const
{
    return TTF_RenderUTF8_Blended(font, text.data(), col::WHITE);
}

uint32_t FontManager::findSuitableFontSize(const std::span<TextBubble> sceneTextData, const uint32_t size) const
{
    TTF_SetFontSize(font, size);

    std::span<const TextBubble> allFits = fut::filter(sceneTextData, [&](const TextBubble& data)
    {
        const std::string& text = data.getText();
        SDL_FRect area = data.getHoldingArea();
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