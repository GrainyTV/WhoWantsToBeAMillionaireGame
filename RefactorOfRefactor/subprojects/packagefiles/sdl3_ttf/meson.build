project(
    'sdl3_ttf',
    'c',
    version: 'stable-3.1.0'
)

sdl3 = dependency('sdl3')
freetype = dependency('freetype2')
harfbuzz = dependency('harfbuzz')

sources = files(
    'src/SDL_hashtable.c',
    'src/SDL_hashtable_ttf.c',
    'src/SDL_gpu_textengine.c',
    'src/SDL_renderer_textengine.c',
    'src/SDL_surface_textengine.c',
    'src/SDL_ttf.c'
)

incdirs = [
    'src',
    'include'
]

sdl3_ttf_lib = static_library(
    meson.project_name(),
    sources,
    include_directories: incdirs,
    dependencies: [sdl3, freetype, harfbuzz]
)

sdl3_ttf_dep = declare_dependency(
    include_directories: incdirs,
    link_with: sdl3_ttf_lib
)

meson.override_dependency('sdl3_ttf', sdl3_ttf_dep)
