project('sdl3_image', 'c', version: '3.2.0')
subproject('zlib', default_options: 'default_library=static')
subproject('libpng', default_options: 'default_library=static')

sdl3 = dependency('sdl3')
libpng = dependency('libpng')

add_project_arguments(
    '-DSDL_IMAGE_USE_COMMON_BACKEND',
    '-DLOAD_PNG',
    language: 'c'
)

sources = files(
    'src/IMG.c',
    'src/IMG_WIC.c',
    'src/IMG_avif.c',
    'src/IMG_bmp.c',
    'src/IMG_gif.c',
    'src/IMG_jpg.c',
    'src/IMG_jxl.c',
    'src/IMG_lbm.c',
    'src/IMG_pcx.c',
    'src/IMG_png.c',
    'src/IMG_pnm.c',
    'src/IMG_qoi.c',
    'src/IMG_stb.c',
    'src/IMG_svg.c',
    'src/IMG_tga.c',
    'src/IMG_tif.c',
    'src/IMG_webp.c',
    'src/IMG_xcf.c',
    'src/IMG_xpm.c',
    'src/IMG_xv.c'
)

incdirs = include_directories('src', 'include')

sdl3_image_lib = library(
    meson.project_name(),
    sources,
    include_directories: incdirs,
    dependencies: [sdl3, libpng],
    install: true,
    install_rpath: '.'
)

sdl3_image_dep = declare_dependency(
    include_directories: incdirs,
    link_with: sdl3_image_lib
)

meson.override_dependency(meson.project_name(), sdl3_image_dep)
