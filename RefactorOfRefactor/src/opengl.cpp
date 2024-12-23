#include "opengl.hpp"
#include "defer.hpp"
#include "result.hpp"
#include <climits>

static constexpr uint32_t GL_TEXTURE_2D = 0x0DE1;
static constexpr uint32_t GL_TEXTURE_MIN_FILTER = 0x2801;
static constexpr uint32_t GL_TEXTURE_MAG_FILTER = 0x2800;
static constexpr uint32_t GL_NEAREST = 0x2600;
static constexpr int32_t GL_LINEAR = 0x2601;
static constexpr int32_t GL_LINEAR_MIPMAP_LINEAR = 0x2703;

void OpenGL::bindTexture(SDL_Texture* texture)
{
    const SDL_PropertiesID textureProperties = SDL_GetTextureProperties(texture);
    DEFER(SDL_DestroyProperties, textureProperties);

    const auto textureId = Result<int32_t>(
        SDL_GetNumberProperty(textureProperties, SDL_PROP_TEXTURE_OPENGL_TEXTURE_NUMBER, 0),
        "Failed to read ID of texture",
        1,
        INT_MAX);

    if (textureId.isOk())
    {
        const auto glBindTexture = (void (*)(uint32_t, uint32_t)) SDL_GL_GetProcAddress("glBindTexture");
        glBindTexture(GL_TEXTURE_2D, textureId.value());
    }
}

void OpenGL::applyTextureFiltering(SDL_ScaleMode filter)
{
    const auto glGenerateMipmap = (void (*)(uint32_t)) SDL_GL_GetProcAddress("glGenerateMipmap");
    const auto glTexParameteri = (void (*)(uint32_t, uint32_t, int32_t)) SDL_GL_GetProcAddress("glTexParameteri");

    switch (filter)
    {
        case SDL_SCALEMODE_BEST:
        case SDL_SCALEMODE_LINEAR:
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;

        case SDL_SCALEMODE_NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
    }
}