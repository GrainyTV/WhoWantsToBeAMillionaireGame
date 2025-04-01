project(
    'cpptrace',
    'cpp',
    version: '0.7.5',
    default_options: ['warning_level=2']
)

sources = files(
    'src/binary/elf.cpp',
    'src/binary/mach-o.cpp',
    'src/binary/module_base.cpp',
    'src/binary/object.cpp',
    'src/binary/pe.cpp',
    'src/binary/safe_dl.cpp',
    'src/cpptrace.cpp',
    'src/ctrace.cpp',
    'src/exceptions.cpp',
    'src/from_current.cpp',
    'src/options.cpp',
    'src/utils.cpp',
    'src/demangle/demangle_with_cxxabi.cpp',
    'src/demangle/demangle_with_nothing.cpp',
    'src/demangle/demangle_with_winapi.cpp',
    'src/snippets/snippet.cpp',
    'src/symbols/dwarf/debug_map_resolver.cpp',
    'src/symbols/dwarf/dwarf_resolver.cpp',
    'src/symbols/symbols_core.cpp',
    'src/symbols/symbols_with_addr2line.cpp',
    'src/symbols/symbols_with_dbghelp.cpp',
    'src/symbols/symbols_with_dl.cpp',
    'src/symbols/symbols_with_libbacktrace.cpp',
    'src/symbols/symbols_with_libdwarf.cpp',
    'src/symbols/symbols_with_nothing.cpp',
    'src/unwind/unwind_with_dbghelp.cpp',
    'src/unwind/unwind_with_execinfo.cpp',
    'src/unwind/unwind_with_libunwind.cpp',
    'src/unwind/unwind_with_nothing.cpp',
    'src/unwind/unwind_with_unwind.cpp',
    'src/unwind/unwind_with_winapi.cpp',
    'src/utils/microfmt.cpp',
    'src/utils/utils.cpp',
    'src/platform/dbghelp_syminit_manager.cpp'
)

add_project_arguments('-DCPPTRACE_STATIC_DEFINE', language: 'cpp')
incdirs = include_directories('include', 'src')

config = []
dependencies = []

if host_machine.system() == 'windows'
    config += [
        '-DCPPTRACE_UNWIND_WITH_DBGHELP',
        '-DCPPTRACE_GET_SYMBOLS_WITH_DBGHELP',
        '-DCPPTRACE_DEMANGLE_WITH_NOTHING',
    ]

    windows_sdk_dir = run_command('cmd', '/c', 'echo', '%WindowsSdkDir%', check: true).stdout().strip()
    windows_sdk_version = run_command('cmd', '/c', 'echo', '%WindowsSDKVersion%', check: true).stdout().strip()
    windows_sdk_lib_path = windows_sdk_dir + 'Lib' / windows_sdk_version + 'um' / 'x64'

    cpp = meson.get_compiler('cpp')
    dependencies += cpp.find_library('dbghelp', dirs: windows_sdk_lib_path, static: true)

else
    config += [
        '-DCPPTRACE_UNWIND_WITH_UNWIND',
        '-DCPPTRACE_GET_SYMBOLS_WITH_LIBDWARF',
        '-DCPPTRACE_DEMANGLE_WITH_CXXABI',
        '-DLIBDWARF_STATIC'
    ]

    subproject('libdwarf', default_options: 'default_library=static')
    dependencies += dependency('libdwarf')

endif

cpptrace_lib = static_library(
    meson.project_name(),
    sources,
    include_directories: incdirs,
    cpp_args: [config],
    pic: true,
    dependencies: [dependencies]
)

cpptrace_dep = declare_dependency(
    include_directories: incdirs,
    link_with: cpptrace_lib
)

meson.override_dependency('cpptrace', cpptrace_dep)
