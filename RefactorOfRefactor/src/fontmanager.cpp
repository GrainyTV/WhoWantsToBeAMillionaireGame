#include "fontmanager.hpp"
#include "assert.hpp"
#include "colors.hpp"

FontManager::FontManager()
    : font(NULL)
{
}

FontManager::FontManager(const char* fontResource, const uint32_t fontSize)
    : font(TTF_OpenFont(fontResource, fontSize))
{
    ASSERT(font != NULL, "Failed to load the game's font ({})", TTF_GetError());
}

FontManager& FontManager::operator=(FontManager&& other) noexcept
{
    font = other.font;
    other.font = NULL;
    return *this;
}

void FontManager::free() const
{
    TTF_CloseFont(font);
}

SDL_Surface* FontManager::texturize(const std::string& text) const
{
    ASSERT(font != NULL, "Tried to create font based texture without a valid font");
    return TTF_RenderText_Blended(font, text.c_str(), col::WHITE);
}