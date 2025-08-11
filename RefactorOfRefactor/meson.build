project(
    'WhoWantsToBeAMillionaire',
    'cpp',
    default_options: {
        'cpp_std': 'c++20',
        'buildtype': 'debug',
        'prefix': meson.current_source_dir() / 'bin',
        'libdir': 'lib64',
    },
    version: '1.0',
    meson_version: '>=1.3.0'
)

cpp = meson.get_compiler('cpp')
platform = host_machine.system()

global_args = cpp.get_supported_arguments('/utf-8')
add_global_arguments(global_args, language: 'cpp')

local_args = cpp.get_supported_arguments(
# Macro defines
    '-DTOML_EXCEPTIONS=0',
    '-DGLM_FORCE_XYZW_ONLY',
    '-DCPPTRACE_STATIC_DEFINE',
# Unix args
    '-Wconversion',
    '-Wfloat-conversion',
    '-Wno-missing-field-initializers',
# Windows args
    '/utf-8',
    '/Zc:preprocessor',
    '/wd4100',
    '/wd4530'
)
add_project_arguments(local_args, language: 'cpp')

dependencies = [
    dependency('tomlplusplus'),
    dependency('glm'),
    dependency('sdl3'),
    dependency('sdl3_image'),
    dependency('sdl3_mixer'),
    dependency('sdl3_ttf'),
    dependency('fmt'),
    dependency('cpptrace'),
]

sources = files(
    'src/ellipse.cpp',
    'src/bezier.cpp',
    'src/utility.cpp',
    'src/fontmanager.cpp',
    'src/opengl.cpp',
    'src/roundedrectangle.cpp',
    'src/scoreboard.cpp',
    'src/hexagon.cpp',
    'src/toml.cpp',
    'src/asset.cpp',
    'src/event.cpp',
    'src/game.cpp',
    'src/main.cpp'
)

copy_assets = run_command(
    find_program('tools' / 'copy_dir.py'), 'assets',
    check: true,
    capture: false
)

executable(
    meson.project_name(),
    sources,
    name_suffix: platform == 'windows' ? 'exe' : 'out',
    include_directories: include_directories('hdr'),
    dependencies: dependencies,
    override_options: {
        'warning_level': '3',
        'werror': true,
        'b_ndebug': 'if-release',
    },
    install: true,
    install_dir: get_option('prefix'),
    install_rpath: get_option('libdir')
)

install_subdir('assets',
    install_dir: get_option('prefix'),
    follow_symlinks: false
)
