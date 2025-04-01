#include "game.hpp"
#include "SDL3_image/SDL_image.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "debug.hpp"
#include "event.hpp"
#include "globals.hpp"
#include "optionwrap.hpp"
#include "result.hpp"
#include "utility.hpp"
#include <cstdint>

#include "glad/gl.h"

void Game::init()
{
    // ┏━━━━━━━━━━┓
    // ┃ SDL Init ┃
    // ┗━━━━━━━━━━┛

    SDL_SetHint(SDL_HINT_VIDEO_DOUBLE_BUFFER, "true");

    const auto sdlInit = Result(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS), "Failed to initialize SDL");
    Debug::assert(sdlInit.isOk(), "{}", sdlInit.toString());

    // ┏━━━━━━━━━━━━┓
    // ┃ Audio Init ┃
    // ┗━━━━━━━━━━━━┛

    const auto sdlAudioInit = Result(Mix_Init(MIX_INIT_MP3), "Failed to initialize SDL_mixer", static_cast<uint32_t>(MIX_INIT_MP3));
    Debug::assert(sdlAudioInit.isOk(), "{}", sdlAudioInit.toString());

    const auto audioDeviceInit = Result(Mix_OpenAudio(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr), "Failed to open default audio device for playback");
    Debug::assert(audioDeviceInit.isOk(), "{}", audioDeviceInit.toString());

    // ┏━━━━━━━━━━━┓
    // ┃ Text Init ┃
    // ┗━━━━━━━━━━━┛

    const auto sdlTextInit = Result(TTF_Init(), "Failed to initialize SDL_ttf");
    Debug::assert(sdlTextInit.isOk(), "{}", sdlTextInit.toString());

    // ┏━━━━━━━━━━━━━━┓
    // ┃ General Init ┃
    // ┗━━━━━━━━━━━━━━┛

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    SDL_GL_SetSwapInterval(1);

    const auto windowInit = Result(
        SDL_CreateWindow("Who Wants To Be A Millionaire?", 0, 0, SDL_WINDOW_OPENGL),
        "SDL_CreateWindow() failed");
    Debug::assert(windowInit.isOk(), "{} SDL({})", windowInit.toString(), SDL_GetError());

    SDL_Window* const window = windowInit.unwrap();
    //SDL_GLContext glContext = SDL_GL_CreateContext(window);
    SDL_GL_CreateContext(window);
    int version = gladLoadGL(static_cast<GLADloadfunc>(SDL_GL_GetProcAddress));
    printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL Vendor: %s\n", glGetString(GL_VENDOR));

auto MessageCallback = []( GLenum /*source*/,
                 GLenum type,
                 GLuint /*id*/,
                 GLenum severity,
                 GLsizei /*length*/,
                 const GLchar* message,
                 const void* /*userParam*/ ) -> void
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
};

// During init, enable debug output
glEnable              ( GL_DEBUG_OUTPUT );
glDebugMessageCallback( MessageCallback, 0 );

    const SDL_DisplayMode* screenProperties = Utility::displayInfo(window);

    glViewport(0, 0, (*screenProperties).w, (*screenProperties).h);

    // SDL_PropertiesID rendererProperties = SDL_CreateProperties();
    // SDL_SetStringProperty(rendererProperties, SDL_PROP_RENDERER_CREATE_NAME_STRING, "opengl");
    // SDL_SetPointerProperty(rendererProperties, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window);
    // SDL_SetNumberProperty(rendererProperties, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);

    // const auto rendererInit = Result(
    //     SDL_CreateRendererWithProperties(rendererProperties),
    //     "SDL_CreateRenderer() failed");
    // Debug::assert(rendererInit.isOk(), "{}", rendererInit.toString());

    // SDL_Renderer* const renderer = rendererInit.unwrap();

    Globals::Properties = Option::Some<Globals::GameProperties>({
        .ScreenWidth = (*screenProperties).w,
        .ScreenHeight = (*screenProperties).h,
        .Window = window,
        //.Renderer = renderer,
    });
}

void Game::deinit()
{
    const auto properties = Globals::Properties.value();

    //SDL_DestroyRenderer(properties.Renderer);
    SDL_DestroyWindow(properties.Window);
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

void Game::launch()
{
    Event::startRunningOnMainUiThread();
}
