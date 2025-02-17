#include "asset.hpp"
#include "SDL3_image/SDL_image.h"
#include "countdownevent.hpp"
#include "debug.hpp"
#include "defer.hpp"
#include "functionals.hpp"
#include "globals.hpp"
#include "opengl.hpp"
#include "result.hpp"
#include "utility.hpp"
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>

namespace Asset
{
    namespace
    {
        enum class AssetType : std::uint8_t
        {
            Texture,
            Sfx,
            Music,
            Data,
        };

        using enum Identifier;
        using enum AssetType;
        using enum Utility::CustomEvents;

        struct MetaAsset
        {
            AssetType Type;
            const char* Name;
        };

        std::unordered_map<Identifier, MetaAsset> assetDescriptor =
        {
            { Logo, { .Type = Texture, .Name = "logo" }},
            { Background, { .Type = Texture, .Name = "background" }},
        };

        std::unordered_map<Identifier, SDL_Texture*> textureCache;
        std::vector<std::pair<Identifier, MetaAsset>> neededEntries;
        CountDownEvent countdown;

        void convertSurfaceToTexture(SDL_Surface* surface, const Identifier id)
        {
            DEFER(SDL_DestroySurface, surface);
            const auto properties = Globals::Properties.value();

            const auto conversion = Result(
                SDL_CreateTextureFromSurface(properties.Renderer, surface),
                "Failed to convert surface to texture"
            );
            ASSERT(conversion.isOk(), "{}", conversion.cause());

            SDL_Texture* texture = conversion.value();
            OpenGL::bindTexture(texture);
            OpenGL::applyTextureFiltering(SDL_SCALEMODE_LINEAR);

            textureCache.emplace(id, texture);

            countdown.signal();
        }

        void daemonProcess()
        {
            countdown.reset(neededEntries.size());

            neededEntries
            | Seq::iter([](const auto& entry)
                {
                    const Identifier id = entry.first;
                    const MetaAsset metadata = entry.second;

                    switch (metadata.Type)
                    {
                        case Texture:
                        {
                            char buffer[100];
                            SDL_snprintf(buffer, sizeof(buffer), "assets/textures/%s.png", metadata.Name);

                            const auto surface = Result(
                                IMG_Load(buffer),
                                "Failed to load surface"
                            );
                            ASSERT(surface.isOk(), "{}", surface.cause());
                            
                            Invokable* const callback = new Invokable(convertSurfaceToTexture, surface.value(), id);
                            Utility::requestUserEvent({ .type = EVENT_INVOKE_ON_UI_THREAD, .data1 = static_cast<void*>(callback), });
                            break;
                        }

                        default:
                            LOG("Not yet implemented!");
                            break;
                    }
                });

            countdown.wait();

            neededEntries.clear();
            Utility::requestEvent({ .type = EVENT_ENABLE_SCENE });
            Utility::invalidate();     
        }
    }

    void queueToLoad(const Identifier id)
    {
        ASSERT(assetDescriptor.contains(id), "Asset Descriptor is missing provided indentifier {}", static_cast<int32_t>(id));
        neededEntries.emplace_back(id, assetDescriptor[id]);
    }

    void beginLoadProcess()
    {
        std::thread worker(daemonProcess);
        worker.detach();
    }

    SDL_Texture* getTextureById(const Identifier id)
    {
        ASSERT(textureCache.contains(id), "Texture Cache is missing provided indentifier {}", static_cast<int32_t>(id));
        return textureCache[id];
    }
}
