project(
    'Who Wants to Be a Millionaire?',
    'cpp',
    default_options: {
        'cpp_std': 'c++20',
        'buildtype': 'debug',
        'prefix': meson.current_build_dir() / 'bin',
        'libdir': 'lib64'
    },
    version: '1.0',
    meson_version: '>=1.3.0'
)

dependencies = [
    dependency('tomlplusplus'),
    dependency('svl'),
    dependency('sdl3'),
    dependency('sdl3_image'),
    dependency('sdl3_mixer'),
    dependency('sdl3_ttf')
]

if get_option('buildtype') == 'debug'
    dependencies += [
        dependency('cpptrace', static: true),
        dependency('fmt', static: true)
    ]
endif

sources = files(
    'src/utility.cpp',
    'src/fontmanager.cpp',
    'src/opengl.cpp',
    'src/hexagon.cpp',
    'src/toml.cpp',
    'src/asset.cpp',
    'src/ingamescene.cpp',
    'src/mainmenuscene.cpp',
    'src/event.cpp',
    'src/game.cpp',
    'src/main.cpp'
)

output_prefix = get_option('prefix')

executable(
    meson.project_name(),
    sources,
    include_directories: include_directories('hdr'),
    dependencies: dependencies,
    cpp_args: [
        '-Wno-missing-field-initializers',
        '-Wno-return-type',
        '-DTOML_EXCEPTIONS=0',
    ],
    override_options: {
        'warning_level': '3',
        'werror': true,
        'b_asneeded': false,
        'b_ndebug': 'if-release'
    },
    install: true,
    install_dir: output_prefix,
    install_rpath: get_option('libdir')
)

install_subdir('assets',
    install_dir: output_prefix,
    follow_symlinks: false
)
