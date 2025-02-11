cmake = import('cmake')

sdl3_options = cmake.subproject_options()
sdl3_options.add_cmake_defines({
    'SDL_DBUS': false,
    'SDL_TEST_LIBRARY': false,
    'SDL_EXAMPLES': false,
    'CMAKE_POSITION_INDEPENDENT_CODE': true  
})
sdl3_proj = cmake.subproject('sdl3', options: sdl3_options)
meson.override_dependency('sdl3', sdl3_proj.dependency('SDL3-shared'))
