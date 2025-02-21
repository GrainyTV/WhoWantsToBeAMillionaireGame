#include "asset.hpp"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "countdownevent.hpp"
#include "debug.hpp"
#include "defer.hpp"
#include "fontmanager.hpp"
#include "functionals.hpp"
#include "globals.hpp"
#include "opengl.hpp"
#include "result.hpp"
#include "utility.hpp"
#include <filesystem>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>

//#include "data.hpp"

namespace Asset
{
    namespace
    {
        enum class AssetType : std::uint8_t
        {
            Texture,
            Sfx,
            Music,
            Text,
        };

        using enum Identifier;
        using enum AssetType;
        using enum Utility::CustomEvents;
        namespace fs = std::filesystem;

        struct MetaAsset
        {
            AssetType Type;
            const char* Name;
        };

        std::unordered_map<Identifier, MetaAsset> assetDescriptor =
        {
            { Logo, { .Type = Texture, .Name = "logo" }},
            { Background, { .Type = Texture, .Name = "background" }},
            { MusicEasy, { .Type = Music, .Name = "music-easy-questions" }},
            { MusicMedium, { .Type = Music, .Name = "music-medium-questions" }},
            { MusicHard, { .Type = Music, .Name = "music-hard-questions" }},
            { InputFiles, { .Type = Text }},
        };

        std::unordered_map<std::string, std::vector<Data>> dataCache;
        std::unordered_map<Identifier, Mix_Music*> musicCache;
        std::unordered_map<Identifier, SDL_Texture*> textureCache;
        std::vector<std::pair<Identifier, MetaAsset>> neededEntries;
        CountDownEvent countdown;

        void convertSurfaceToTexture(SDL_Surface* surface, const Identifier id)
        {    
            const auto properties = Globals::Properties.value();

            const auto conversion = Result(
                SDL_CreateTextureFromSurface(properties.Renderer, surface),
                "Failed to convert surface to texture"
            );
            ASSERT(conversion.isOk(), "{}", conversion.cause());
            DEFER(SDL_DestroySurface, surface);

            SDL_Texture* texture = conversion.value();
            OpenGL::bindTexture(texture);
            OpenGL::applyTextureFiltering(SDL_SCALEMODE_LINEAR);

            textureCache.emplace(id, texture);
            countdown.signal();
        }

        void parseTomlFile(const fs::directory_entry& file)
        {
            const fs::path& filePath = file.path();

            if (filePath.extension().string() == ".toml")
            {
                const char* filePathStr = filePath.c_str();
                const toml::parse_result parse = toml::parse_file(filePathStr);
                
                ASSERT(parse.succeeded(), "Invalid TOML file encountered ({})", filePathStr);
                const toml::table& content = parse.table();
                
                ASSERT(content["entry"].is_array_of_tables(), "File not in 'array of tables' format prefixed with [[entry]] tags ({})", filePathStr);
                const toml::array& entries = *content["entry"].as_array();
                
                std::vector<Data> loadedRecords = entries | Seq::mapi(Data::convertFromToml);
                std::string topic = filePath.stem().string();
                dataCache.emplace(topic, std::move(loadedRecords));
            }
        }

        void processIndividualAssets(const std::pair<Identifier, MetaAsset>& entry)
        {
            const Identifier id = entry.first;
            const MetaAsset metadata = entry.second;

            switch (metadata.Type)
            {
                case Texture:
                {
                    constexpr const char* TEXTURE_PATH = "assets/textures/%s.png";
                    const size_t bufferSize = Utility::formattedSize(TEXTURE_PATH, metadata.Name);
                    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(bufferSize);
                    SDL_snprintf(buffer.get(), bufferSize, TEXTURE_PATH, metadata.Name);

                    const auto surface = Result(IMG_Load(buffer.get()), "Failed to load surface");
                    ASSERT(surface.isOk(), "{}", surface.cause());
                    
                    auto* const callback = new Invokable(convertSurfaceToTexture, surface.value(), id);
                    Utility::requestUserEvent({ .type = EVENT_INVOKE_ON_UI_THREAD, .data1 = callback });
                    break;
                }

                case Music:
                {
                    constexpr const char* AUDIO_PATH = "assets/audio/%s.mp3";
                    const size_t bufferSize = Utility::formattedSize(AUDIO_PATH, metadata.Name);
                    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(bufferSize);
                    SDL_snprintf(buffer.get(), bufferSize, AUDIO_PATH, metadata.Name);

                    const auto music = Result(Mix_LoadMUS(buffer.get()), "Failed to load music");
                    ASSERT(music.isOk(), "{}", music.cause());
                    musicCache.emplace(id, music.value());
                    countdown.signal();
                    break;
                }

                case Text:
                {        
                    fs::directory_iterator("assets/toml") | Seq::iter(parseTomlFile);
                    countdown.signal();
                    break;
                }

                default:
                    LOG("Not yet implemented!");
                    break;
            }
        }

        void daemonProcess()
        {
            countdown.reset(neededEntries.size());
            neededEntries | Seq::iter(processIndividualAssets);
            countdown.wait();

            neededEntries.clear();
            Utility::requestEvent({ EVENT_ENABLE_SCENE });
            Utility::invalidate();     
        }
    }

    void queueToLoad(const Identifier id)
    {
        ASSERT(assetDescriptor.contains(id), "Asset Descriptor is missing provided indentifier {}", static_cast<int32_t>(id));
        
        if (textureCache.contains(id) == false && musicCache.contains(id) == false)
        {
            neededEntries.emplace_back(id, assetDescriptor[id]);
        }   
    }

    void beginLoadProcess()
    {
        if (neededEntries.empty() == false)
        {
            std::thread worker(daemonProcess);
            worker.detach();
        }
        else
        {
            Utility::requestEvent({ EVENT_ENABLE_SCENE });
            Utility::invalidate();
        }
    }

    SDL_Texture* getTextureById(const Identifier id)
    {
        ASSERT(textureCache.contains(id), "Texture Cache is missing provided indentifier {}", static_cast<int32_t>(id));
        return textureCache[id];
    }

    Mix_Music* getMusicById(const Identifier id)
    {
        ASSERT(musicCache.contains(id), "Music Cache is missing provided indentifier {}", static_cast<int32_t>(id));
        return musicCache[id];
    }

    Data getRandomData()
    {
        return dataCache["otaku"][0];
    }
}
