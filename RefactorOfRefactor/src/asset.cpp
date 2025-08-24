#include "asset.hpp"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "countdownevent.hpp"
#include "debug.hpp"
#include "defer.hpp"
#include "fontmanager.hpp"
#include "seq/seq.hpp"
#include "globals.hpp"
//#include "opengl.hpp"
#include "result.hpp"
#include "utility.hpp"
#include <filesystem>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>
#include <random>

namespace Asset
{
    namespace
    {
        enum class AssetType : std::uint8_t
        {
            TEXTURE,
            SFX,
            MUSIC,
            TEXT,
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

        std::mt19937& generator()
        {
            static std::random_device randomDevice;
            static std::mt19937 generator(randomDevice());  
            return generator;
        }

        std::unordered_map<Identifier, MetaAsset> assetDescriptor =
        {
            { Logo, { .Type = TEXTURE, .Name = "logo.png" }},
            { Background, { .Type = TEXTURE, .Name = "background.png" }},
            { MusicEasy, { .Type = MUSIC, .Name = "music-easy-questions" }},
            { MusicMedium, { .Type = MUSIC, .Name = "music-medium-questions" }},
            { MusicHard, { .Type = MUSIC, .Name = "music-hard-questions" }},
            { InputFiles, { .Type = TEXT }},
            { CorrectAnswer, { .Type = SFX, .Name = "correct-answer" }},
            { WrongAnswer, { .Type = SFX, .Name = "wrong-answer" }},
            { FinalAnswer, { .Type = SFX, .Name = "final-answer" }},
            { HELP_AUDIENCE,  { .Type = TEXTURE, .Name = "people-group-solid.svg" }},
            { HELP_PHONE, { .Type = TEXTURE, .Name = "phone-volume-solid.svg" }},
        };

        std::vector<std::string> dataTopics;
        std::unordered_map<std::string, std::vector<Toml::Data>> dataCache;
        std::unordered_map<Identifier, Mix_Music*> musicCache;
        std::unordered_map<Identifier, Mix_Chunk*> sfxCache;
        std::unordered_map<Identifier, Shader::TextureInfo> textureCache;        

        std::vector<std::pair<Identifier, MetaAsset>> neededEntries;
        CountDownEvent countdown;

        //void convertSurfaceToTexture(SDL_Surface*& surface, const Identifier id)
        //{
        //    // DEFER(SDL_DestroySurface, surface);

        //    //auto texture = OpenGL::createTextureFromSurface(surface);
        //    auto texture = Shader::createTextureFromSurface(surface);
        //    textureCache.emplace(id, texture);
        //    countdown.signal();
        //}

        void parseInputFile(const fs::directory_entry& file)
        {
            const fs::path& filePath = file.path();

            if (filePath.extension().string() == ".toml")
            {
                std::vector<Toml::Data> loadedRecords = Toml::getAllFromFile(filePath.string());
                std::string topic = filePath.stem().string();

                dataTopics.emplace_back(topic);
                dataCache.emplace(topic, std::move(loadedRecords));
            }
        }

        void processIndividualAssets(const std::pair<Identifier, MetaAsset>& entry)
        {
            const Identifier id = entry.first;
            const MetaAsset metadata = entry.second;

            switch (metadata.Type)
            {
                case TEXTURE:
                {
                    constexpr const char* TEXTURE_PATH_FMT = "assets/textures/%s";
                    const std::unique_ptr<char[]> texturePath = Utility::formatPath(TEXTURE_PATH_FMT, metadata.Name);

                    const auto surfaceResult = Result(IMG_Load(texturePath.get()), "Failed to load surface");
                    Debug::assert(surfaceResult.isOk(), "{}", surfaceResult.toString());

                    SDL_Surface* const surface = surfaceResult.unwrap();

                    if ((*surface).format != SDL_PIXELFORMAT_RGBA32)
                    {
                        DEFER(SDL_DestroySurface, surface);
                        SDL_Surface* const convertedSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);

                        textureCache.emplace(id, Shader::createTextureFromSurface(convertedSurface));
                    }
                    else
                    {
                        textureCache.emplace(id, Shader::createTextureFromSurface(surface));
                    }

                    countdown.signal();

                    //const SDL_PixelFormat targetFormat = SDL_PIXELFORMAT_BGRA8888;

                    //auto* callback = new Invokable(convertSurfaceToTexture, surface, id);

                    // Invokable* callback = nullptr;

                    // if ((*surface).format != targetFormat)
                    // {
                    //     DEFER(SDL_DestroySurface, surface);
                        
                    //     SDL_Surface* convertedSurface = SDL_ConvertSurface(surface, targetFormat);
                    //     callback = new Invokable(convertSurfaceToTexture, convertedSurface, id);
                    // }
                    // else
                    // {
                    //     callback = new Invokable(convertSurfaceToTexture, surface, id);
                    // }

                    //Utility::requestUserEvent({ .type = EVENT_INVOKE_ON_UI_THREAD, .data1 = callback });
                    break;
                }

                case MUSIC:
                {
                    constexpr const char* AUDIO_PATH_FMT = "assets/audio/%s.mp3";
                    const std::unique_ptr<char[]> audioPath = Utility::formatPath(AUDIO_PATH_FMT, metadata.Name);

                    const auto music = Result(Mix_LoadMUS(audioPath.get()), "Failed to load music");
                    Debug::assert(music.isOk(), "{}", music.toString());
                    
                    musicCache.emplace(id, music.unwrap());
                    countdown.signal();
                    break;
                }

                case SFX:
                {
                    constexpr const char* AUDIO_PATH_FMT = "assets/audio/%s.mp3";
                    const std::unique_ptr<char[]> audioPath = Utility::formatPath(AUDIO_PATH_FMT, metadata.Name);

                    const auto sfx = Result(Mix_LoadWAV(audioPath.get()), "Failed to load sound effect");
                    Debug::assert(sfx.isOk(), "{}", sfx.toString());
                    
                    sfxCache.emplace(id, sfx.unwrap());
                    countdown.signal();
                    break;
                }

                case TEXT:
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
        Debug::assert(assetDescriptor.contains(id), "Asset Descriptor is missing provided indentifier {}", static_cast<int32_t>(id));
        
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

    Shader::TextureInfo getTextureById(const Identifier id)
    {
        Debug::assert(textureCache.contains(id), "Texture Cache is missing provided indentifier {}", static_cast<int32_t>(id));
        return textureCache[id];
    }

    Mix_Music* getMusicById(const Identifier id)
    {
        Debug::assert(musicCache.contains(id), "Music Cache is missing provided indentifier {}", static_cast<int32_t>(id));
        return musicCache[id];
    }

    Mix_Chunk* getSfxById(const Identifier id)
    {
        Debug::assert(sfxCache.contains(id), "SFX Cache is missing provided indentifier {}", static_cast<int32_t>(id));
        return sfxCache[id];
    }

    Toml::Data getRandomData()
    {
        std::uniform_int_distribution<size_t> topicDistrib(0, dataTopics.size() - 1);
        const std::string& topic = dataTopics.at(topicDistrib(generator()));

        std::uniform_int_distribution<size_t> questionDistrib(0, dataCache[topic].size() - 1);  
        const Toml::Data selected = dataCache[topic][questionDistrib(generator())];
        
        return selected;
    }
}
