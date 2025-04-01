project(
    'Who Wants To Be A Millionaire',
    'cpp',
    default_options: {
        'cpp_std': 'c++20',
        'buildtype': 'debug',
        'prefix': meson.current_build_dir() / 'bin',
        'libdir': 'lib64',
    },
    version: '1.0',
    meson_version: '>=1.3.0'
)

cpp = meson.get_compiler('cpp')
platform = host_machine.system()

add_project_arguments(
    '-DTOML_EXCEPTIONS=0',
    '-DGLM_FORCE_XYZW_ONLY',
    '-DCPPTRACE_STATIC_DEFINE',
    language: 'cpp'
)

available_args = cpp.get_supported_arguments(
    '-Wno-missing-field-initializers',
    '/Zc:preprocessor',
)

subdir('meson' / 'wraps')

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

output_prefix = get_option('prefix')

executable(
    meson.project_name().replace(' ', ''),
    sources,
    name_suffix: platform == 'windows' ? 'exe' : 'out',
    include_directories: include_directories('hdr'),
    dependencies: dependencies,
    cpp_args: available_args,
    override_options: {
        'warning_level': '3',
        'werror': true,
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
 
