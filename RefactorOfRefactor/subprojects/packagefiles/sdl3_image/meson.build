project(
    'sdl3_image',
    'c',
    version: 'stable-3.2.0'
)

sdl3 = dependency('sdl3')
libpng = dependency('libpng')

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

incdir = [
    'src',
    'include'
]

sdl3_image_lib = static_library(
    meson.project_name(),
    sources,
    include_directories: incdir,
    dependencies: [sdl3, libpng],
    c_args: [
        '-DSDL_IMAGE_USE_COMMON_BACKEND',
        '-DLOAD_PNG'
    ]
)

sdl3_image_dep = declare_dependency(
    include_directories: incdir,
    link_with: sdl3_image_lib
)

meson.override_dependency('sdl3_image', sdl3_image_dep)
