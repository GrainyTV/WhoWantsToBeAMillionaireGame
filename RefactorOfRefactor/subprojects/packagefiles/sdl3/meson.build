project(
    'sdl3',
    'c',
    version: 'stable-3.2.0'
)

fs = import('fs')
cc = meson.get_compiler('c')
conf_data = configuration_data()
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

sources = files(
    'src/SDL.c',
    'src/SDL_assert.c',
    'src/SDL_error.c',
    'src/SDL_guid.c',
    'src/SDL_hashtable.c',
    'src/SDL_hints.c',
    'src/SDL_list.c',
    'src/SDL_log.c',
    'src/SDL_properties.c',
    'src/SDL_utils.c',
    'src/atomic/SDL_atomic.c',
    'src/atomic/SDL_spinlock.c',
    'src/audio/SDL_audio.c',
    'src/audio/SDL_audiocvt.c',
    'src/audio/SDL_audiodev.c',
    'src/audio/SDL_audioqueue.c',
    'src/audio/SDL_audioresample.c',
    'src/audio/SDL_audiotypecvt.c',
    'src/audio/SDL_mixer.c',
    'src/audio/SDL_wave.c',
    'src/camera/SDL_camera.c',
    'src/core/SDL_core_unsupported.c',
    'src/cpuinfo/SDL_cpuinfo.c',
    'src/dynapi/SDL_dynapi.c',
    'src/events/SDL_categories.c',
    'src/events/SDL_clipboardevents.c',
    'src/events/SDL_displayevents.c',
    'src/events/SDL_dropevents.c',
    'src/events/SDL_events.c',
    'src/events/SDL_keyboard.c',
    'src/events/SDL_keymap.c',
    'src/events/SDL_keysym_to_keycode.c',
    'src/events/SDL_keysym_to_scancode.c',
    'src/events/SDL_mouse.c',
    'src/events/SDL_pen.c',
    'src/events/SDL_quit.c',
    'src/events/SDL_scancode_tables.c',
    'src/events/SDL_touch.c',
    'src/events/SDL_windowevents.c',
    'src/events/imKStoUCS.c',
    'src/io/SDL_asyncio.c',
    'src/io/SDL_iostream.c',
    'src/io/generic/SDL_asyncio_generic.c',
    'src/filesystem/SDL_filesystem.c',
    'src/gpu/SDL_gpu.c',
    'src/joystick/SDL_gamepad.c',
    'src/joystick/SDL_joystick.c',
    'src/joystick/SDL_steam_virtual_gamepad.c',
    'src/joystick/controller_type.c',
    'src/haptic/SDL_haptic.c',
    'src/hidapi/SDL_hidapi.c',
    'src/locale/SDL_locale.c',
    'src/main/SDL_main_callbacks.c',
    'src/main/SDL_runapp.c',
    'src/misc/SDL_url.c',
    'src/power/SDL_power.c',
    'src/render/SDL_d3dmath.c',
    'src/render/SDL_render.c',
    'src/render/SDL_render_unsupported.c',
    'src/render/SDL_yuv_sw.c',
    'src/render/direct3d/SDL_render_d3d.c',
    'src/render/direct3d/SDL_shaders_d3d.c',
    'src/render/direct3d11/SDL_render_d3d11.c',
    'src/render/direct3d11/SDL_shaders_d3d11.c',
    'src/render/direct3d12/SDL_render_d3d12.c',
    'src/render/direct3d12/SDL_shaders_d3d12.c',
    'src/render/gpu/SDL_pipeline_gpu.c',
    'src/render/gpu/SDL_render_gpu.c',
    'src/render/gpu/SDL_shaders_gpu.c',
    'src/render/opengl/SDL_render_gl.c',
    'src/render/opengl/SDL_shaders_gl.c',
    'src/render/opengles2/SDL_render_gles2.c',
    'src/render/opengles2/SDL_shaders_gles2.c',
    'src/render/ps2/SDL_render_ps2.c',
    'src/render/psp/SDL_render_psp.c',
    'src/render/software/SDL_blendfillrect.c',
    'src/render/software/SDL_blendline.c',
    'src/render/software/SDL_blendpoint.c',
    'src/render/software/SDL_drawline.c',
    'src/render/software/SDL_drawpoint.c',
    'src/render/software/SDL_render_sw.c',
    'src/render/software/SDL_rotate.c',
    'src/render/software/SDL_triangle.c',
    'src/render/vitagxm/SDL_render_vita_gxm.c',
    'src/render/vitagxm/SDL_render_vita_gxm_memory.c',
    'src/render/vitagxm/SDL_render_vita_gxm_tools.c',
    'src/render/vulkan/SDL_render_vulkan.c',
    'src/render/vulkan/SDL_shaders_vulkan.c',
    'src/sensor/SDL_sensor.c',
    'src/stdlib/SDL_crc16.c',
    'src/stdlib/SDL_crc32.c',
    'src/stdlib/SDL_getenv.c',
    'src/stdlib/SDL_iconv.c',
    'src/stdlib/SDL_malloc.c',
    'src/stdlib/SDL_memcpy.c',
    'src/stdlib/SDL_memmove.c',
    'src/stdlib/SDL_memset.c',
    'src/stdlib/SDL_mslibc.c',
    'src/stdlib/SDL_murmur3.c',
    'src/stdlib/SDL_qsort.c',
    'src/stdlib/SDL_random.c',
    'src/stdlib/SDL_stdlib.c',
    'src/stdlib/SDL_string.c',
    'src/stdlib/SDL_strtokr.c',
    'src/storage/SDL_storage.c',
    'src/thread/SDL_thread.c',
    'src/time/SDL_time.c',
    'src/timer/SDL_timer.c',
    'src/video/SDL_RLEaccel.c',
    'src/video/SDL_blit.c',
    'src/video/SDL_blit_0.c',
    'src/video/SDL_blit_1.c',
    'src/video/SDL_blit_A.c',
    'src/video/SDL_blit_N.c',
    'src/video/SDL_blit_auto.c',
    'src/video/SDL_blit_copy.c',
    'src/video/SDL_blit_slow.c',
    'src/video/SDL_bmp.c',
    'src/video/SDL_clipboard.c',
    'src/video/SDL_egl.c',
    'src/video/SDL_fillrect.c',
    'src/video/SDL_pixels.c',
    'src/video/SDL_rect.c',
    'src/video/SDL_stretch.c',
    'src/video/SDL_surface.c',
    'src/video/SDL_video.c',
    'src/video/SDL_video_unsupported.c',
    'src/video/SDL_vulkan_utils.c',
    'src/video/SDL_yuv.c',
    'src/video/yuv2rgb/yuv_rgb_lsx.c',
    'src/video/yuv2rgb/yuv_rgb_sse.c',
    'src/video/yuv2rgb/yuv_rgb_std.c'
)

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

libm_dep = cc.find_library('m')

thread_dep = dependency('threads')
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

incdirs = [
    'include',
    'src'
]

sdl3_lib = library(
    meson.project_name(),
    sources,
    include_directories: incdirs,
    dependencies: [
        libm_dep,
        thread_dep
    ],
    install: true
)

sdl3_dep = declare_dependency(
    link_with: sdl3_lib,
    include_directories: incdirs
)

meson.override_dependency(meson.project_name(), sdl3_dep)
