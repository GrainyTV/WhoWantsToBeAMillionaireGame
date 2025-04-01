project('sdl3_ttf', 'c', version: '3.1.0')
subproject('freetype2')

sdl3 = dependency('sdl3')
freetype2 = dependency('freetype2')

sources = files(
    'src/SDL_hashtable.c',
    'src/SDL_hashtable_ttf.c',
    'src/SDL_gpu_textengine.c',
    'src/SDL_renderer_textengine.c',
    'src/SDL_surface_textengine.c',
    'src/SDL_ttf.c'
)

incdirs = include_directories('src', 'include')

sdl3_ttf_lib = library(
    meson.project_name(),
    sources,
    include_directories: incdirs,
    dependencies: [sdl3, freetype2],
    install: true,
    install_rpath: '.'
)

sdl3_ttf_dep = declare_dependency(
    include_directories: incdirs,
    link_with: sdl3_ttf_lib
)

meson.override_dependency(meson.project_name(), sdl3_ttf_dep)
