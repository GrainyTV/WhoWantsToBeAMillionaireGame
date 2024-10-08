project('Who Wants to Be a Millionaire?', 'cpp',
    default_options: ['cpp_std=c++23', 'warning_level=3', 'werror=true'],
    version: '1.0'
)

sources = files(
    'src/utility.cpp',
    'src/fontmanager.cpp',
    'src/texture.cpp',
    'src/ingamescene.cpp',
    'src/event.cpp',
    'src/game.cpp',
    'src/main.cpp'
)

cxx = meson.get_compiler('cpp')
relative_libdir = 'lib'
absolute_libdir = meson.current_source_dir() / relative_libdir

needed_libraries = [
    cxx.find_library('SDL3',
        dirs: [absolute_libdir],
    ),
    cxx.find_library('SDL3_image',
        dirs: [absolute_libdir],
    ),
    cxx.find_library('SDL3_mixer',
        dirs: [absolute_libdir],
    ),
    cxx.find_library('SDL3_ttf',
        dirs: [absolute_libdir],
    )
]

libcpp_experimental = cxx.find_library('stdc++exp',
    static: true
)

output_prefix = meson.current_build_dir() / 'bin'

executable('a.out', sources,
    include_directories: ['hdr'],
    dependencies: [needed_libraries, libcpp_experimental],
    cpp_args: [
        '-Wno-missing-field-initializers',
        '-Wno-return-type',
        '-fmax-errors=5',
    ],
    install: true,
    install_dir: output_prefix,
    install_rpath: relative_libdir
)

install_subdir(relative_libdir,
    install_dir: output_prefix,
    follow_symlinks: false,
    exclude_files: ['dependencies.json']
)

install_subdir('assets',
    install_dir: output_prefix,
    follow_symlinks: false
)
