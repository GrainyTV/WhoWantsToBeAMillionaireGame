project('sdl3', 'c', version: 'stable-3.2.0')

fs = import('fs')
cc = meson.get_compiler('c')
conf_data = configuration_data()

sources = []
incdirs = ['include', 'src']
dependencies = []

subdir('src')

conf_data.set10('HAVE_LIBC', true)

libc_headers = [
    'float.h',
    'iconv.h',
    'inttypes.h',
    'limits.h',
    'malloc.h',
    'math.h',
    'memory.h',
    'signal.h',
    'stdarg.h',
    'stdbool.h',
    'stddef.h',
    'stdint.h',
    'stdio.h',
    'stdlib.h',
    'string.h',
    'strings.h',
    'sys/types.h',
    'time.h',
    'wchar.h'
]

symbols_to_check = [
    'abs',
    'acos',
    'acosf',
    'asin',
    'asinf',
    'atan',
    'atan2',
    'atan2f',
    'atanf',
    'atof',
    'atoi',
    'bcopy',
    'ceil',
    'ceilf',
    'copysign',
    'copysignf',
    'cos',
    'cosf',
    '_Exit',
    'exp',
    'expf',
    'fabs',
    'fabsf',
    'floor',
    'floorf',
    'fmod',
    'fmodf',
    'fopen64',
    'fseeko',
    'fseeko64',
    'getenv',
    '_i64toa',
    'index',
    'isinf',
    'isinff',
    'isnan',
    'isnanf',
    'itoa',
    'log',
    'log10',
    'log10f',
    'logf',
    'lround',
    'lroundf',
    '_ltoa',
    'malloc',
    'memcmp',
    'memcpy',
    'memmove',
    'memset',
    'modf',
    'modff',
    'pow',
    'powf',
    'putenv',
    'rindex',
    'round',
    'roundf',
    'scalbn',
    'scalbnf',
    'setenv',
    'sin',
    'sinf',
    'sqr',
    'sqrt',
    'sqrtf',
    'sscanf',
    'strchr',
    'strcmp',
    'strlcat',
    'strlcpy',
    'strlen',
    'strncmp',
    'strnlen',
    'strpbrk',
    'strrchr',
    'strstr',
    'strnstr',
    'strtod',
    'strtok_r',
    'strtol',
    'strtoll',
    'strtoul',
    'strtoull',
    'tan',
    'tanf',
    'trunc',
    'truncf',
    'unsetenv',
    'vsnprintf',
    'vsscanf',
    'wcsnlen',
    'wcscmp',
    'wcsdup',
    'wcslcat',
    'wcslcpy',
    'wcslen',
    'wcsncmp',
    'wcsstr',
    'wcstol'
]

foreach header : libc_headers
    if cc.has_header(header)
        header_id = fs.stem(header).to_upper()
        conf_data.set10(f'HAVE_@header_id@_H', true)
    endif
endforeach

foreach symbol : symbols_to_check
    if cc.has_function(symbol)
        symbol_id = symbol.to_upper()
        conf_data.set10(f'HAVE_@symbol_id@', true)
    endif
endforeach

video_driver_wayland = true

wayland_deps = [
    dependency('wayland-client', version: '>=1.18'),
    dependency('wayland-egl'),
    dependency('wayland-cursor'),
    dependency('xkbcommon', version: '>=0.5.0')
]

foreach dep : wayland_deps
    if dep.found() == false
        video_driver_wayland = false
    endif
endforeach

if video_driver_wayland
    conf_data.set10('SDL_VIDEO_DRIVER_WAYLAND', true)
    sources += [
        'src/video/wayland/SDL_waylandclipboard.c',
        'src/video/wayland/SDL_waylanddatamanager.c',
        'src/video/wayland/SDL_waylanddyn.c',
        'src/video/wayland/SDL_waylandevents.c',
        'src/video/wayland/SDL_waylandkeyboard.c',
        'src/video/wayland/SDL_waylandmessagebox.c',
        'src/video/wayland/SDL_waylandmouse.c',
        'src/video/wayland/SDL_waylandopengles.c',
        'src/video/wayland/SDL_waylandshmbuffer.c',
        'src/video/wayland/SDL_waylandvideo.c',
        'src/video/wayland/SDL_waylandvulkan.c',
        'src/video/wayland/SDL_waylandwindow.c'
    ]

    ws = find_program('wayland-scanner')

    wayland_protocols_xml = [
        'wayland-protocols/alpha-modifier-v1.xml',
        'wayland-protocols/cursor-shape-v1.xml',
        'wayland-protocols/fractional-scale-v1.xml',
        'wayland-protocols/frog-color-management-v1.xml',
        'wayland-protocols/idle-inhibit-unstable-v1.xml',
        'wayland-protocols/input-timestamps-unstable-v1.xml',
        'wayland-protocols/keyboard-shortcuts-inhibit-unstable-v1.xml',
        'wayland-protocols/pointer-constraints-unstable-v1.xml',
        'wayland-protocols/primary-selection-unstable-v1.xml',
        'wayland-protocols/relative-pointer-unstable-v1.xml',
        'wayland-protocols/tablet-v2.xml',
        'wayland-protocols/text-input-unstable-v3.xml',
        'wayland-protocols/viewporter.xml',
        'wayland-protocols/wayland.xml',
        'wayland-protocols/xdg-activation-v1.xml',
        'wayland-protocols/xdg-decoration-unstable-v1.xml',
        'wayland-protocols/xdg-dialog-v1.xml',
        'wayland-protocols/xdg-foreign-unstable-v2.xml',
        'wayland-protocols/xdg-output-unstable-v1.xml',
        'wayland-protocols/xdg-shell.xml',
        'wayland-protocols/xdg-toplevel-icon-v1.xml'
    ]

    foreach xml : wayland_protocols_xml
        proto = fs.stem(xml)
        proto_c = f'@proto@-protocol.c'
        proto_h = f'@proto@-client-protocol.h'

        sources += configure_file(
            command: [ws, 'private-code', '@INPUT@', '@OUTPUT@'],
            input: xml,
            output: proto_c
        )

        sources += configure_file(
            command: [ws, 'client-header', '@INPUT@', '@OUTPUT@'],
            input: xml,
            output: proto_h
        )

    endforeach

    find_dylib_name = find_program('./find-dylib-name.py')
    compiler_command = cc.cmd_array()

    libWayland = run_command(find_dylib_name, wayland_deps[0].name(), compiler_command, check: true)
    conf_data.set_quoted('SDL_VIDEO_DRIVER_WAYLAND_DYNAMIC', libWayland.stdout().strip())

    libWaylandEgl = run_command(find_dylib_name, wayland_deps[1].name(), compiler_command, check: true)
    conf_data.set_quoted('SDL_VIDEO_DRIVER_WAYLAND_DYNAMIC_EGL', libWaylandEgl.stdout().strip())

    libWaylandCursor = run_command(find_dylib_name, wayland_deps[2].name(), compiler_command, check: true)
    conf_data.set_quoted('SDL_VIDEO_DRIVER_WAYLAND_DYNAMIC_CURSOR', libWaylandCursor.stdout().strip())

    libXkbcommon = run_command(find_dylib_name, wayland_deps[3].name(), compiler_command, check: true)
    conf_data.set_quoted('SDL_VIDEO_DRIVER_WAYLAND_DYNAMIC_XKBCOMMON', libXkbcommon.stdout().strip())

endif

mesa = dependency('libglvnd')

if mesa.found()
    conf_data.set10('SDL_VIDEO_OPENGL', true)
    conf_data.set10('SDL_VIDEO_RENDER_OGL', true)
    conf_data.set10('SDL_VIDEO_OPENGL_ES2', true)
    conf_data.set10('SDL_VIDEO_RENDER_OGL_ES2', true)
    conf_data.set10('SDL_VIDEO_OPENGL_EGL', true)
    conf_data.set10('SDL_VIDEO_OPENGL_GLX', true)

endif

pipewire = dependency('libpipewire-0.3', version: '>=0.3.44')

if pipewire.found()
    conf_data.set10('SDL_AUDIO_DRIVER_PIPEWIRE', true)
    conf_data.set10('SDL_CAMERA_DRIVER_PIPEWIRE', true)

    sources += [
        'src/audio/pipewire/SDL_pipewire.c',
        'src/camera/pipewire/SDL_camera_pipewire.c'
    ]

    find_dylib_name = find_program('./find-dylib-name.py')
    compiler_command = cc.cmd_array()

    libPipewire = run_command(find_dylib_name, 'pipewire-0.3', compiler_command, check: true)
    conf_data.set_quoted('SDL_AUDIO_DRIVER_PIPEWIRE_DYNAMIC', libPipewire.stdout().strip())
    conf_data.set_quoted('SDL_CAMERA_DRIVER_PIPEWIRE_DYNAMIC', libPipewire.stdout().strip())

    dependencies += pipewire

endif

libm = cc.find_library('m')

if libm.found()
    dependencies += libm
endif

pthread = dependency('threads')

if pthread.found()
    conf_data.set10('SDL_THREAD_PTHREAD', true)
    conf_data.set10('HAVE_SEM_TIMEDWAIT', true)
    
    sources += [
        'src/thread/pthread/SDL_systhread.c',
        'src/thread/pthread/SDL_sysmutex.c',
        'src/thread/pthread/SDL_syscond.c',
        'src/thread/pthread/SDL_sysrwlock.c',
        'src/thread/pthread/SDL_systls.c',
        'src/thread/pthread/SDL_syssem.c'
    ]

    dependencies += pthread
endif

sources += [
    'src/core/unix/SDL_appid.c',
    'src/core/unix/SDL_poll.c'
]

sources += [
    'src/tray/SDL_tray_utils.c',
    'src/tray/unix/SDL_tray.c'
]

sources += 'src/locale/unix/SDL_syslocale.c'
sources += 'src/misc/unix/SDL_sysurl.c'
sources += [
    'src/dialog/SDL_dialog.c',
    'src/dialog/SDL_dialog_utils.c',
]

sources += [
    'src/dialog/unix/SDL_unixdialog.c',
    'src/dialog/unix/SDL_portaldialog.c',
    'src/dialog/unix/SDL_zenitydialog.c'
]

sources += [
    'src/core/linux/SDL_evdev_capabilities.c',
    'src/core/linux/SDL_threadprio.c'
]

sources += 'src/main/generic/SDL_sysmain_callbacks.c'

conf_data.set10('SDL_TIMER_UNIX', true)
sources += 'src/timer/unix/SDL_systimer.c'

conf_data.set10('HAVE_LINUX_INPUT_H', cc.has_header('linux/input.h'))

if conf_data.get('HAVE_LINUX_INPUT_H') == 1
    conf_data.set10('SDL_INPUT_LINUXEV', true)
    conf_data.set10('SDL_HAPTIC_LINUX', true)
    sources += 'src/haptic/linux/SDL_syshaptic.c'
endif

conf_data.set10('SDL_FILESYSTEM_UNIX', true)
sources += 'src/filesystem/unix/SDL_sysfilesystem.c'

conf_data.set10('SDL_FSOPS_POSIX', true)
sources += 'src/filesystem/posix/SDL_sysfsops.c'

conf_data.set10('SDL_TIME_UNIX', true)
sources += 'src/time/unix/SDL_systime.c'

conf_data.set10('SDL_STORAGE_STEAM', true)
sources += 'src/storage/generic/SDL_genericstorage.c'
sources += 'src/storage/steam/SDL_steamstorage.c'

conf_data.set10('SDL_PROCESS_POSIX', true)
sources += 'src/process/SDL_process.c'
sources += 'src/process/posix/SDL_posixprocess.c'

conf_data.set10('SDL_LOADSO_DLOPEN', true)
sources += 'src/loadso/dlopen/SDL_sysloadso.c'

configure_file(
    input: 'include/build_config/SDL_build_config.h.cmake',
    output: 'SDL_build_config.h',
    format: 'cmake@',
    configuration: conf_data
)

sdl3_lib = library(
    meson.project_name(),
    sources,
    include_directories: incdirs,
    dependencies: dependencies,
    install: true
)

sdl3_dep = declare_dependency(
    link_with: sdl3_lib,
    include_directories: incdirs
)

meson.override_dependency(meson.project_name(), sdl3_dep)
