#pragma once
#include "SDL3/SDL.h"
#include "optionwrap.hpp"
#include "glm/glm.hpp"
#include <span>

namespace Shader
{
    namespace _details
    {
        struct SizedBuffer { SDL_GPUBuffer* Content; uint32_t Count; };
    }

    // todo: rename this to soemthing else
    struct TextureInfo
    {
        SDL_Surface* Metadata;
        SDL_GPUTexture* Texture;
        SDL_GPUBuffer* TexturePositions;
    };

    struct VertexInfo
    {
        _details::SizedBuffer VertexBuffer;
        _details::SizedBuffer IndexBuffer;
    };

    struct CopyOperation
    {
        SDL_GPUCommandBuffer* CommandBuffer;
        SDL_GPUCopyPass* Copy;
    };

	void init();

	SDL_GPURenderPass* beginRender(SDL_GPUCommandBuffer* const, SDL_GPUTexture* const);

	void changeDrawColorTo(SDL_GPUCommandBuffer* const, SDL_FColor);

    TextureInfo createTextureFromSurface(SDL_Surface* const);

    void renderTexture(SDL_GPUCommandBuffer* const, SDL_GPURenderPass* const, const TextureInfo&);

    void uploadTextureToGpuWithAreaIfNeeded(const TextureInfo&, const Option<SDL_FRect>& = OptionExtensions::None<SDL_FRect>());

    VertexInfo createAndUploadPrimitives(std::span<const glm::vec2>, bool = false);

    // VertexInfo createAndUploadPrimitives(std::initializer_list<glm::vec2>);

    void renderTriangles(SDL_GPURenderPass* const, const VertexInfo&);

    void finalizeRender(SDL_GPUCommandBuffer* const, SDL_GPUTexture* const);

    auto beginUploadToGpu() -> void;

    auto endUploadToGpu() -> void;
}