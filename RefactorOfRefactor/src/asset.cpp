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
            { CorrectAnswer, { .Type = Sfx, .Name = "correct-answer" }},
            { WrongAnswer, { .Type = Sfx, .Name = "wrong-answer" }},
            { FinalAnswer, { .Type = Sfx, .Name = "final-answer" }},
        };

        std::unordered_map<std::string, std::vector<Toml::Data>> dataCache;
        std::unordered_map<Identifier, Mix_Music*> musicCache;
        std::unordered_map<Identifier, Mix_Chunk*> sfxCache;
        std::unordered_map<Identifier, TextureGpu> textureCache;        

        std::vector<std::pair<Identifier, MetaAsset>> neededEntries;
        CountDownEvent countdown;

        void convertSurfaceToTexture(SDL_Surface*& surface, const Identifier id)
        {
            DEFER(SDL_DestroySurface, surface);

            TextureGpu texture = OpenGL::createTextureFromSurface(surface);
            textureCache.emplace(id, texture);
            countdown.signal();
        }

        void parseInputFile(const fs::directory_entry& file)
        {
            const fs::path& filePath = file.path();

            if (filePath.extension().string() == ".toml")
            {
                std::vector<Toml::Data> loadedRecords = Toml::getAllFromFile(filePath.string());
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
                    constexpr const char* TEXTURE_PATH_FMT = "assets/textures/%s.png";
                    const std::unique_ptr<char[]> texturePath = Utility::formatPath(TEXTURE_PATH_FMT, metadata.Name);

                    const auto surfaceResult = Result(IMG_Load(texturePath.get()), "Failed to load surface");
                    assert(surfaceResult.isOk(), "{}", surfaceResult.toString());

                    SDL_Surface* surface(surfaceResult.unwrap());
                    const SDL_PixelFormat targetFormat = SDL_PIXELFORMAT_BGRA8888;

                    // ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
                    // ┃ Flip surface for OpenGL's inverted coordinate system ┃
                    // ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

                    SDL_FlipSurface(surface, SDL_FlipMode::SDL_FLIP_VERTICAL);
                    Invokable* callback = nullptr;

                    if ((*surface).format != targetFormat)
                    {
                        DEFER(SDL_DestroySurface, surface);
                        
                        SDL_Surface* convertedSurface = SDL_ConvertSurface(surface, targetFormat);
                        callback = new Invokable(convertSurfaceToTexture, convertedSurface, id);
                    }
                    else
                    {
                        callback = new Invokable(convertSurfaceToTexture, surface, id);
                    }

                    Utility::requestUserEvent({ .type = EVENT_INVOKE_ON_UI_THREAD, .data1 = callback });
                    break;
                }

                case Music:
                {
                    constexpr const char* AUDIO_PATH_FMT = "assets/audio/%s.mp3";
                    const std::unique_ptr<char[]> audioPath = Utility::formatPath(AUDIO_PATH_FMT, metadata.Name);

                    const auto music = Result(Mix_LoadMUS(audioPath.get()), "Failed to load music");
                    assert(music.isOk(), "{}", music.toString());
                    
                    musicCache.emplace(id, music.unwrap());
                    countdown.signal();
                    break;
                }

                case Sfx:
                {
                    constexpr const char* AUDIO_PATH_FMT = "assets/audio/%s.mp3";
                    const std::unique_ptr<char[]> audioPath = Utility::formatPath(AUDIO_PATH_FMT, metadata.Name);

                    const auto sfx = Result(Mix_LoadWAV(audioPath.get()), "Failed to load sound effect");
                    assert(sfx.isOk(), "{}", sfx.toString());
                    
                    sfxCache.emplace(id, sfx.unwrap());
                    countdown.signal();
                    break;
                }

                case Text:
                {        
                    fs::directory_iterator("assets/toml") | Seq::iter(parseInputFile);
                    countdown.signal();
                    break;
                }
            }
        }

        void daemonProcess()
        {
            countdown.reset(neededEntries.size());
            neededEntries | Seq::iter(processIndividualAssets);
            countdown.wait();

            neededEntries.clear();
            Utility::requestUserEvent({ EVENT_ENABLE_SCENE });    
        }
    }

    void queueToLoad(const Identifier id)
    {
        assert(assetDescriptor.contains(id), "Asset Descriptor is missing provided indentifier {}", static_cast<int32_t>(id));
        
        bool haveTexture = textureCache.contains(id);
        bool haveMusic = musicCache.contains(id);
        bool haveSfx = sfxCache.contains(id);
        bool haveText = id == InputFiles && dataCache.empty() == false;

        // ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
        // ┃ If ID is found in one of the caches, no need to reload that asset ┃
        // ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

        if (haveTexture || haveMusic || haveSfx || haveText)
        {
            return;
        }

        neededEntries.emplace_back(id, assetDescriptor[id]);
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
            Utility::requestUserEvent({ EVENT_ENABLE_SCENE });
        }
    }

    TextureGpu getTextureById(const Identifier id)
    {
        assert(textureCache.contains(id), "Texture Cache is missing provided indentifier {}", static_cast<int32_t>(id));
        return textureCache[id];
    }

    Mix_Music* getMusicById(const Identifier id)
    {
        assert(musicCache.contains(id), "Music Cache is missing provided indentifier {}", static_cast<int32_t>(id));
        return musicCache[id];
    }

    Mix_Chunk* getSfxById(const Identifier id)
    {
        assert(sfxCache.contains(id), "SFX Cache is missing provided indentifier {}", static_cast<int32_t>(id));
        return sfxCache[id];
    }

    Toml::Data getRandomData()
    {
        return dataCache["otaku"][0];
    }
}
